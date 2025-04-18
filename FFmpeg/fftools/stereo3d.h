
#ifndef STEREO3D_H_
#define STEREO3D_H_

#include <SDL.h>
#include <SDL_ttf.h>
#include <stdbool.h>
#include "hc_ffplay.h"

int scaleIt2( AVFrame *frame_in, AVFrame *frame_out );

AVFrame* allocatFrame_3d(AVFrame *frame);

int video_thread_3d( VideoState *is, AVFrame *tmp, AVFrame *frame, AVFrame *frmFinal,  AVFilterContext *filt_in , AVFilterContext *filt_out,  AVRational frame_rate );

int queue_picture(VideoState *is, AVFrame *src_frame, double pts, double duration, int64_t pos, int serial);

#endif
