
#include "stereo3d.h"
#include "hc_common.h"
#include "hc_ffplay.h"


unsigned char *frame_buffer_out = NULL;
AVFilterContext *buffersink_ctx = NULL;
AVFilterContext *buffersrc_ctx = NULL;
AVFilterGraph *filter_graph = NULL;

int  scaleIt2( AVFrame *frame_in, AVFrame *frame_out )
{
    int ret;
    
    if(!filter_graph )
    {
        int in_width = frame_in->width;
        int in_height = frame_in->height;;

        //https://cpp.hotexamples.com/examples/-/-/avfilter_graph_parse_ptr/cpp-avfilter_graph_parse_ptr-function-examples.html  
        //https://stackoverflow.com/questions/27810161/display-filtered-ffmpeg-frame-with-gtk char *filter_descr = "crop=100:100,scale=640:360";
        //const char *filter_descr = "lutyuv='u=128:v=128'";
        
        const char filter_descr[] = "crop=in_w - in_w/2 :in_h - in_h/2,scale=in_w + in_w/2 :in_h + in_h/2";//"boxblur";
        
        //const char *filter_descr = "boxblur";//"boxblur";
        //const char *filter_descr = "hflip";
        //const char *filter_descr = "hue='h=60:s=-3'";
        //const char *filter_descr = "crop=2/3*in_w:2/3*in_h";
        //const char *filter_descr = "drawbox=x=100:y=100:w=100:h=100:color=pink@0.5";
        //const char *filter_descr = "drawtext=fontfile=arial.ttf:fontcolor=green:fontsize=30:text='Lei Xiaohua'";
        
        snprintf(filter_descr, sizeof (filter_descr), "crop=in_w - in_w/20 :in_h - in_h/20,scale=%d : %d", in_width,in_height );

        char args[512];
        AVFilter *buffersrc = avfilter_get_by_name("buffer");
        AVFilter *buffersink = avfilter_get_by_name("buffersink");
        AVFilterInOut *outputs = avfilter_inout_alloc();
        AVFilterInOut *inputs = avfilter_inout_alloc();
        enum AVPixelFormat pix_fmts[] = {AV_PIX_FMT_YUV420P, AV_PIX_FMT_NONE};
        AVBufferSinkParams *buffersink_params;

        filter_graph = avfilter_graph_alloc();

        /* buffer video source: the decoded frames from the decoder will be inserted here. */
        snprintf(args, sizeof (args),
                "video_size=%dx%d:pix_fmt=%d:time_base=%d/%d:pixel_aspect=%d/%d",
                in_width, in_height, AV_PIX_FMT_YUV420P,
                1, 25, 1, 1);

        ret = avfilter_graph_create_filter(&buffersrc_ctx, buffersrc, "in",
                args, NULL, filter_graph);
        if (ret < 0) {
            printf("Cannot create buffer source\n");
            return ret;
        }

        /* buffer video sink: to terminate the filter chain. */
        buffersink_params = av_buffersink_params_alloc();
        buffersink_params->pixel_fmts = pix_fmts;
        ret = avfilter_graph_create_filter(&buffersink_ctx, buffersink, "out",
                NULL, buffersink_params, filter_graph);
        av_free(buffersink_params);
        if (ret < 0) {
            printf("Cannot create buffer sink\n");
            return ret;
        }

        /* Endpoints for the filter graph. */
        outputs->name = av_strdup("in");
        outputs->filter_ctx = buffersrc_ctx;
        outputs->pad_idx = 0;
        outputs->next = NULL;

        inputs->name = av_strdup("out");
        inputs->filter_ctx = buffersink_ctx;
        inputs->pad_idx = 0;
        inputs->next = NULL;

        if ((ret = avfilter_graph_parse_ptr(filter_graph, filter_descr,
                &inputs, &outputs, NULL)) < 0)
            return ret;

        if ((ret = avfilter_graph_config(filter_graph, NULL)) < 0)
            return ret;

    //    frame_in = av_frame_alloc();
    //    frame_buffer_in = (unsigned char *) av_malloc(av_image_get_buffer_size(AV_PIX_FMT_YUV420P, in_width, in_height, 1));
    //    av_image_fill_arrays(frame_in->data, frame_in->linesize, frame_buffer_in,
    //            AV_PIX_FMT_YUV420P, in_width, in_height, 1);

       // frame_out = av_frame_alloc();
        frame_buffer_out = (unsigned char *) av_malloc(av_image_get_buffer_size(AV_PIX_FMT_YUV420P, in_width, in_height, 1));
        av_image_fill_arrays(frame_out->data, frame_out->linesize, frame_buffer_out,   AV_PIX_FMT_YUV420P, in_width, in_height, 1);
    }

    //frame_in->width = in_width;
    //frame_in->height = in_height;
   // frame_in->format = AV_PIX_FMT_YUV420P;

      
        //input Y,U,V

    if (av_buffersrc_add_frame(buffersrc_ctx, frame_in) < 0) {
        printf("Error while add frame.\n");
        exit(1);
    }

    /* pull filtered pictures from the filtergraph */
    ret = av_buffersink_get_frame(buffersink_ctx, frame_out);
    if (ret < 0)
         exit(1);

    //output Y,U,V
    if (frame_out->format == AV_PIX_FMT_YUV420P) {

    }
        //printf("Process 1 frame!\n");
    //   av_frame_unref(frame_out);
    //}

    //fclose(fp_in);
    //fclose(fp_out);

  //  av_frame_free(&frame_in);
  //  av_frame_free(&frame_out);
    avfilter_graph_free(&filter_graph);
    
    return 0;
}

AVFrame* allocatFrame_3d(AVFrame *frame)   
{
    AVFrame* frmFinal;
    
    frmFinal = av_frame_alloc();
    if (!frmFinal) {
        exit(1);
    }
    frmFinal->format = frame->format;
    frmFinal->width  = frame->width*2;
    frmFinal->height = frame->height;

    /* the image can be allocated by any means and av_image_alloc() is
     * just the most convenient way if av_malloc() is to be used */
   int ret = av_image_alloc(frmFinal->data, frmFinal->linesize, frame->width*2, frame->height,
                         frame->format, 16);
    if (ret < 0) {
        fprintf(stderr, "Could not allocate raw picture buffer\n");
        av_frame_free(&frmFinal);
        exit(1);
    }
    return frmFinal;
}

int video_thread_3d( VideoState *is, AVFrame *tmp, AVFrame *frame, AVFrame *frmFinal,  AVFilterContext *filt_in , AVFilterContext *filt_out,  AVRational frame_rate )
{
    static AVFrame* prvFrame = NULL;
    double pts;
    double duration;
    int ret;
    AVRational tb; //= is->video_st->time_base;

#if CONFIG_AVFILTER
        if (!prvFrame)
        {
            prvFrame = av_frame_alloc();

            prvFrame->format = frame->format;
            prvFrame->width  = frame->width;
            prvFrame->height = frame->height;

            /* the image can be allocated by any means and av_image_alloc() is
             * just the most convenient way if av_malloc() is to be used */
            int ret = av_image_alloc(prvFrame->data, prvFrame->linesize, frame->width, frame->height,
                                 frame->format, 16);
            if (ret < 0) {
                fprintf(stderr, "Could not allocate raw picture buffer\n");
                av_frame_free(&prvFrame);
                exit(1);
            }

            if ( av_frame_copy(prvFrame, frame) != 0) {
               exit(1); 
            }
            av_frame_free(&frmFinal);
            return 0;
        }
        scaleIt2(prvFrame,  tmp  );
        for (int i = 0; i < 3; ++i)
        {
            int dstStride1 = frmFinal->linesize[i];
            int srcStride1 = tmp->linesize[i];

            int dstStride2 = frmFinal->linesize[i];
            int srcStride2 = frame->linesize[i];

            int size = (i == 0) ? frame->height : frame->height / 2;

            for (int j = 0; j < size; ++j)
            {
                uint8_t* dst1 = frmFinal->data[i] + dstStride1 * j;
                uint8_t* src1 = tmp->data[i] + srcStride1 * j;
                memcpy(dst1, src1, srcStride1);

                uint8_t* dst2 = frmFinal->data[i] + dstStride2 * j + dstStride2/2;
                uint8_t* src2 = frame->data[i] + srcStride2 * j;
                memcpy(dst2, src2, srcStride2);

            }
        }
        ret = av_buffersrc_add_frame(filt_in, frmFinal);
        if (ret < 0)
            return -1;

        while (ret >= 0) {
            is->frame_last_returned_time = av_gettime_relative() / 1000000.0;

            ret = av_buffersink_get_frame_flags(filt_out, frmFinal, 0);
            if (ret < 0) {
                if (ret == AVERROR_EOF)
                    is->viddec.finished = is->viddec.pkt_serial;
                ret = 0;
                break;
            }
            
            //frmFinal->linesize[0] = frame->linesize[0];
           // frmFinal->width =  frame->width;
            //frmFinal->height =  frame->height;

            is->frame_last_filter_delay = av_gettime_relative() / 1000000.0 - is->frame_last_returned_time;
            if (fabs(is->frame_last_filter_delay) > AV_NOSYNC_THRESHOLD / 10.0)
                is->frame_last_filter_delay = 0;
            tb = av_buffersink_get_time_base(filt_out);
#endif
            duration = (frame_rate.num && frame_rate.den ? av_q2d((AVRational){frame_rate.den, frame_rate.num}) : 0);
            pts = (frame->pts == AV_NOPTS_VALUE) ? NAN : frame->pts * av_q2d(tb);
            ret = queue_picture(is, frmFinal, pts, duration, frame->pkt_pos, is->viddec.pkt_serial);
            //av_frame_unref(prvFrame);
             if ( av_frame_copy(prvFrame, frame) != 0) {
               exit(1); 
            }
            av_frame_unref(frame);
            av_frame_unref(frmFinal);
#if CONFIG_AVFILTER
            if (is->videoq.serial != is->viddec.pkt_serial)
                break;
        }
#endif
        av_frame_free(&frmFinal);
  
    return 0;
}
