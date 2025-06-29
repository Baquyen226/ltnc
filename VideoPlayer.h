// VideoPlayer.h
#pragma once
#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <SDL.h>
#include <deque>
#include <mutex>
#include <thread>
#include <atomic>
#include <iostream>

#include "HelperDataType.h"

class VideoPlayer {
public:
    VideoPlayer(const char* filename, SDL_Renderer* renderer);
    ~VideoPlayer();
    void update();
    void render();
    void stop();
    bool isPlaying() const;

private:
    void init(const char* filename);
    void cleanup();
    void decodeLoop();
    void uploadFrameToTexture(AVFrame* frame);

    SDL_Renderer* renderer = nullptr;
    SDL_Texture* texture = nullptr;
    AVFormatContext* formatCtx = nullptr;
    AVCodecContext* codecCtx = nullptr;
    const AVCodec* codec = nullptr;
    AVPacket* packet = nullptr;
    AVFrame* frame = nullptr;
    SwsContext* swsCtx = nullptr;

    std::deque<VideoFrame> frameBuffer;
    std::mutex bufferMutex;
    std::atomic<bool> decodeLoopFlag;
    std::thread decodeThread;

    int videoStreamIndex = -1;
    double FPS = 0.0;
    Uint32 startTime = 0;
    bool playing = false;
};

#endif