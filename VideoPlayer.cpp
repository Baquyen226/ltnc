#include "VideoPlayer.h"
#include "constants.h"

VideoPlayer::VideoPlayer(const char* filename, SDL_Renderer* renderer) : renderer(renderer) {
    init(filename);
}

VideoPlayer::~VideoPlayer() {
    cleanup();
}

void VideoPlayer::init(const char* filename) {
    avformat_network_init();
    if (avformat_open_input(&formatCtx, filename, nullptr, nullptr) < 0 ||
        avformat_find_stream_info(formatCtx, nullptr) < 0) {
        std::cerr << "[VideoPlayer::Init] Failed to open or parse input file\n";
        return;
    }

    for (unsigned int i = 0; i < formatCtx->nb_streams; ++i) {
        if (formatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStreamIndex = i;
            break;
        }
    }
    if (videoStreamIndex == -1) {
        std::cerr << "[VideoPlayer::Init] No video stream found\n";
        return;
    }

    codec = avcodec_find_decoder(formatCtx->streams[videoStreamIndex]->codecpar->codec_id);
    codecCtx = avcodec_alloc_context3(codec);
    avcodec_parameters_to_context(codecCtx, formatCtx->streams[videoStreamIndex]->codecpar);
    avcodec_open2(codecCtx, codec, nullptr);

    FPS = av_q2d(formatCtx->streams[videoStreamIndex]->avg_frame_rate);

    swsCtx = sws_getContext(codecCtx->width, codecCtx->height, codecCtx->pix_fmt,
        codecCtx->width, codecCtx->height, AV_PIX_FMT_YUV420P,
        SWS_BILINEAR, nullptr, nullptr, nullptr);

    packet = av_packet_alloc();
    frame = av_frame_alloc();
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_YV12, SDL_TEXTUREACCESS_STREAMING,
        codecCtx->width, codecCtx->height);

    startTime = SDL_GetTicks();
    playing = true;
    decodeLoopFlag = true;
    decodeThread = std::thread(&VideoPlayer::decodeLoop, this);
}

void VideoPlayer::decodeLoop() {
    while (decodeLoopFlag) {
        // Check buffer size safely
        bool bufferFull = false;
        {
            std::lock_guard<std::mutex> lock(bufferMutex);
            bufferFull = (frameBuffer.size() >= MAX_FRAME_BUFFER_SIZE);
        }

        if (bufferFull) {
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
            continue;
        }

        // Read one frame
        int ret = av_read_frame(formatCtx, packet);
        if (ret < 0) {
            if (ret == AVERROR_EOF) {
                // End of file - stop gracefully
                playing = false;
                decodeLoopFlag = false;
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            continue;
        }

        if (packet->stream_index == videoStreamIndex) {
            if (avcodec_send_packet(codecCtx, packet) == 0) {
                while (avcodec_receive_frame(codecCtx, frame) == 0) {
                    AVFrame* copy = av_frame_alloc();
                    if (copy && av_frame_ref(copy, frame) == 0) {
                        std::lock_guard<std::mutex> lock(bufferMutex);
                        frameBuffer.push_back({ copy, copy->pts });
                    }
                    else {
                        if (copy) av_frame_free(&copy);
                    }
                }
            }
        }

        av_packet_unref(packet);
    }
}

void VideoPlayer::uploadFrameToTexture(AVFrame* srcFrame) {
    AVFrame* yuvFrame = av_frame_alloc();
    yuvFrame->format = AV_PIX_FMT_YUV420P;
    yuvFrame->width = codecCtx->width;
    yuvFrame->height = codecCtx->height;
    av_frame_get_buffer(yuvFrame, 0);

    sws_scale(swsCtx, srcFrame->data, srcFrame->linesize, 0, codecCtx->height,
        yuvFrame->data, yuvFrame->linesize);

    SDL_Rect rect = { 0, 0, codecCtx->width, codecCtx->height };
    SDL_UpdateYUVTexture(texture, &rect,
        yuvFrame->data[0], yuvFrame->linesize[0],
        yuvFrame->data[1], yuvFrame->linesize[1],
        yuvFrame->data[2], yuvFrame->linesize[2]);

    av_frame_free(&yuvFrame);
}

void VideoPlayer::update() {
    if (!playing) return;
    if (startTime == 0 && frameBuffer.size() >= 2) {
        startTime = SDL_GetTicks();
    }

    double currentTime = (SDL_GetTicks() - startTime) / 1000.0;
    std::lock_guard<std::mutex> lock(bufferMutex);

    if (!frameBuffer.empty()) {
        VideoFrame next = frameBuffer.front();
        double framePTS = next.pts * av_q2d(formatCtx->streams[videoStreamIndex]->time_base);
        if (currentTime >= framePTS) {
            uploadFrameToTexture(next.frame);
            av_frame_free(&next.frame);
            frameBuffer.pop_front();
        }
    }
}

void VideoPlayer::render() {
	SDL_FRect rect = { 0, 0 , SCREEN_WIDTH, SCREEN_HEIGHT };
    if (texture)
        SDL_RenderTexture(renderer, texture, nullptr, &rect);
}

void VideoPlayer::stop() {
    playing = false;
}

bool VideoPlayer::isPlaying() const {
    return playing;
}

void VideoPlayer::cleanup() {
    decodeLoopFlag = false;
    if (decodeThread.joinable()) decodeThread.join();

    std::lock_guard<std::mutex> lock(bufferMutex);
    for (auto& vf : frameBuffer) av_frame_free(&vf.frame);
    frameBuffer.clear();

    if (texture) SDL_DestroyTexture(texture);
    if (frame) av_frame_free(&frame);
    if (packet) av_packet_free(&packet);
    if (codecCtx) avcodec_free_context(&codecCtx);
    if (formatCtx) avformat_close_input(&formatCtx);
    if (swsCtx) sws_freeContext(swsCtx);

    avformat_network_deinit();
}
