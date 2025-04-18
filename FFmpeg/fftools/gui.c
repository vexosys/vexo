
#include "gui.h"
#include "hc_common.h"
#include "hc_ffplay.h"
#include "tinyfiledialogs.h"


ui_buttons_s *ui_buttons=NULL;
settings_ui_t *ui_settigs = NULL;

st_ui_subtitles ui_subtitles = { {255, 255, 255, 0},  {20,20} , NULL, 0 };

void playlist(VideoState *cur_stream, char * dropped_file, AVInputFormat *file_iformat , SDL_Window *window)
{
    printf("\nplaylist\n");

   // char dropline[256] = {'\0'};

    cur_stream->show_mode = SHOW_MODE_NONE ;
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "File dropped on window", dropped_file, window);

     //sprintf(dropline, "%s\n", dropped_file);

    addplaylist_front( &ui_settigs->fileplaylist, dropped_file );

    saveplaylist(ui_settigs->fileplaylist);

}


#if 0
static int fire_EQfilters(VideoState *cur_stream)
{


      for (int i = 0; i < cur_stream->agraph->nb_filters; i++) {
        AVFilterContext *filter =  cur_stream->agraph->filters[i];

       // printf( "arvind %s", filter->filter->name);

        if ( (filter->name && !strcmp("anequalizer", filter->name)) || !strcmp("anequalizer", filter->filter->name)) {

            char eqFilter[256];

            sprintf(eqFilter, "0|f=200|w=100|g=%d", cur_stream->gain );

            int r = avfilter_process_command(filter,"change",eqFilter, 0,0,0 );

            sprintf( afilters,"anequalizer=c0 f=200 w=100 g=%d t=0|c1 f=200 w=100 g=-5 t=0, anequalizer=c0 f=1000 w=100 g=3 t=0|c1 f=1000 w=100 g=3 t=0", cur_stream->gain  );

            if (r != AVERROR(ENOSYS)) {

                // printf( "arvind %s", filter->filter->name);

            }
        }
    }

    return 0;
}




 SDL_Texture *load_texture(const char* strbmp)
{


    SDL_Surface *surface = SDL_LoadBMP(strbmp);
    if (!surface) {
        return NULL;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;  // MAY BE NULL.
}

 void minimize_clicked(void)
{
    SDL_MinimizeWindow(window);
}



 void close_clicked(void)
{
    SDL_Event event;
    SDL_zero(event);
    event.type = SDL_QUIT;
    SDL_PushEvent(&event);
}

//static void previous_clicked(void)
//{
//    int rc = 1;
//    SDL_LockAudioDevice(audio_device);
//    if (current_sample) {
//        rc = Sound_Rewind(current_sample);
//    }
//    sample_available = 0;
//    sample_position = 0;
//    SDL_UnlockAudioDevice(audio_device);
//    if (!rc) {
//        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Couldn't rewind audio file!", Sound_GetError(), window);
//    }
//}


#endif


 static int openFileDialog(char const * filePath)
{

    char const * lFilterPatterns[2] = { "*.mp4", "*.mov" };
    char const * lTheOpenFileName;
    FILE * lIn;
    //char lBuffer[1024];

    lTheOpenFileName = tinyfd_openFileDialog(
            "Add mp4 movie",
            "",
            2,
            lFilterPatterns,
            NULL,
            0);

    if (!lTheOpenFileName) {
        tinyfd_messageBox(
                "Error",
                "Open file name is NULL",
                "ok",
                "error",
                1);
        return 1;
    }

    lIn = fopen(lTheOpenFileName, "r");

    if (!lIn) {
        tinyfd_messageBox(
                "Error",
                "Can not open this file in read mode",
                "ok",
                "error",
                1);
        return (1);
    }
    //lBuffer[0] = '\0';
    //fgets(lBuffer, sizeof (lBuffer), lIn);
    fclose(lIn);

    strcpy(filePath, lTheOpenFileName );
    return 0;
 }

#define DEBUG

bool LeftMouseButtonDown(VideoState *cur_stream, SDL_Event *event, int seek_by_bytes, SDL_Window *window, char *file)
{

    if (hcp_clickedOnButton(event, &ui_buttons->play_pause_button)) {
      //cur_stream->paused = !cur_stream->paused;
      ui_buttons->showplayIconOnCenter=0;

      toggle_pause(cur_stream);

      #ifdef DEBUG
      printf("\n play_pause_button (%d,%d) \n ", ui_buttons->play_pause_button.x,  ui_buttons->play_pause_button.y);
      #endif

      video_display(cur_stream);
      return true;

    }
    if(hcp_clickedOnButton(event, &ui_buttons->volume_button)) {
        cur_stream->muted = !cur_stream->muted;

        #ifdef DEBUG
        printf("\n volume_button (%d,%d) \n ", ui_buttons->volume_button.x,  ui_buttons->volume_button.y);
        #endif
        ui_buttons->delayTimer = 360;
        video_display(cur_stream);
        return true;

    }
    if(hcp_clickedOnButton(event, &ui_buttons->zoomout_button)) {
        ui_buttons->zoomout = !ui_buttons->zoomout;

        toggle_full_screen(cur_stream);
        cur_stream->force_refresh = 1;
        //video_display(cur_stream);

        #ifdef DEBUG
        printf("\n zoomout_button (%d,%d) \n ", ui_buttons->zoomout_button.x,  ui_buttons->zoomout_button.y);
        #endif
        return true;
    }


    if(hcp_clickedOnButton(event, &ui_buttons->settings_button)) {
        ui_settigs->setting = !ui_settigs->setting;
        //ui_settigs->x = event->button.x;
        //ui_settigs->y = event->button.y;

        #ifdef DEBUG
        printf("\n settings_button (%d,%d) \n ", ui_buttons->settings_button.x,  ui_buttons->settings_button.y);
        #endif
        if(ui_settigs->setting)
        ui_buttons->delayTimer = 360;
        video_display(cur_stream);
         return true;

    }
    for(int i=0; i < BUTTON_COUNT; i++) {
        if(hcp_clickedOnButton(event, &ui_settigs->buttons[i].rect)) {
           // ui_settigs->x = event->button.x;
           // ui_settigs->y = event->button.y;

            printf("\n settings_tab %s\n ",ui_settigs->buttons[i].label);
            ui_settigs->activeTab = i;
            video_display(cur_stream);
            return true;

        }
    }



    int inc = 0;
    stPlayList *list = ui_settigs->fileplaypage;
    if( ui_settigs->activeTab == 0)
    {
        while(list != NULL)
        {
            if( hcp_clickedOnButton(event, &ui_settigs->playlist_gridRow[inc]))
            {
                if(list == ui_settigs->fileplayactive)
                {
                   SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, list->file, "This movie is already playing",  window);
                   return true;
                }

                strcpy ( file,   list->path);

//                if(fileplaylist )
//                {
//                    printf("\n Add Playlist %s \n ",  file );
//
//                    addplaylist_front(&fileplaylist, file );
//                    saveplaylist(fileplaylist);
//                }

 //               file[strcspn(file, "\n")] = 0;
                printf("\n Playlist %s \n ",  file );

               // ui_settigs->setting = !ui_settigs->setting;
                ui_settigs->fileplayactive = list;
                video_display(cur_stream);

                return true;
            }
            else if( hcp_clickedOnButton(event, &ui_settigs->playlist_delete[inc]))
            {
                if(list == ui_settigs->fileplayactive)
                {
                   SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, list->file, "This movie is already playing",  window);
                   return true;
                }

                if(ui_settigs->fileplaylist )
                {
                    removeplayNode(&ui_settigs->fileplaylist, list , &ui_settigs->fileplaypage);
                    saveplaylist(ui_settigs->fileplaylist);
                }
                //ui_settigs->setting = !ui_settigs->setting;
                 ui_buttons->delayTimer = 360;
                video_display(cur_stream);

               return true;
            }
            else if( hcp_clickedOnButton(event, &ui_settigs->playlist_up[inc]))
            {

                if(ui_settigs->fileplaylist )
                {
                    if(swapplayNode( list, false, &ui_settigs->fileplayactive ))
                        saveplaylist(ui_settigs->fileplaylist);
                }
               // ui_settigs->setting = !ui_settigs->setting;
                ui_buttons->delayTimer = 360;
                video_display(cur_stream);

               return true;
            }
            else if( hcp_clickedOnButton(event, &ui_settigs->playlist_down[inc]))
            {

                if(ui_settigs->fileplaylist )
                {
                    if(swapplayNode(list, true, &ui_settigs->fileplayactive ))
                       saveplaylist(ui_settigs->fileplaylist);
                }
                //ui_settigs->setting = !ui_settigs->setting;
                ui_buttons->delayTimer = 360;
                video_display(cur_stream);

               return true;
            }

            list = list->next;
            if( PageListLen == ++inc)
            {
                break;
            }
        }

        for(int i = 0; i <  PLAYLIST_BUTTON ; ++i)
        {
            if(hcp_clickedOnButton(event, &ui_settigs->playlist_buttons[i].rect)) {
                ui_settigs->playlist_buttons[3].disable = false;
                ui_settigs->playlist_buttons[4].disable = false;
                printf("\n settings_tab %s \n", ui_settigs->playlist_buttons[i].label );
                ui_buttons->delayTimer = 360;
                if(!i)// add file buffton
                {
                    char filepath[256];
                    if(!openFileDialog(filepath))
                    {
                        printf("\n Add file %s in filelist \n", filepath );
                        addplaylist_last( &ui_settigs->fileplaylist, filepath );
                        saveplaylist(ui_settigs->fileplaylist);
                    }
                    ui_settigs->activePlaylistButton = i;
                    video_display(cur_stream);
                    return true;
                }
                else
                {
                    ui_settigs->activePlaylistButton = i;
                    if( i == 3 )
                    ui_settigs->playlist_buttons[3].disable =  !getPage( ui_settigs->fileplaylist , true,  &ui_settigs->fileplaypage);
                    
                    if( i == 4 )
                    ui_settigs->playlist_buttons[4].disable = !getPage( ui_settigs->fileplaylist , false,  &ui_settigs->fileplaypage);    

                    video_display(cur_stream);
                    return true;
                }
            }
        }
    }

    if( ui_settigs->activeTab == 1)
    {
        for(int i=0; i < SPIN_COUNT; i++) {
            if(hcp_clickedOnButton(event, &ui_settigs->subtilesSpin[i].plus)) {

                if( ui_settigs->subtilesSpin[i].value == 255)
                {
                    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Warning", "Valid Range 0 - 255  ", window);
                    return false;
                }
                ui_settigs->subtilesSpin[i].value +=1;

                setSubTitlesValue(&ui_subtitles, i, ui_settigs->subtilesSpin[i].value);


                printf("\n settings_tab \n  %s:%s:%d ", ui_settigs->buttons[ui_settigs->activeTab].label, ui_settigs->subtilesSpin[i].label , ui_settigs->subtilesSpin[i].value);

                ui_buttons->delayTimer = 360;
                video_display(cur_stream);
                return true;
            }

            if(hcp_clickedOnButton(event, &ui_settigs->subtilesSpin[i].minus)) {

               if( ui_settigs->subtilesSpin[i].value == 0)
                {
                    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Warning", "Valid Range 0 - 255  ", window);
                    return false;
                }

                ui_settigs->subtilesSpin[i].value -=1;

                setSubTitlesValue(&ui_subtitles, i,  ui_settigs->subtilesSpin[i].value);

                printf("\n settings_tab \n  %s:%s:%d ", ui_settigs->buttons[ui_settigs->activeTab].label, ui_settigs->subtilesSpin[i].label , ui_settigs->subtilesSpin[i].value);
                ui_buttons->delayTimer = 360;
                video_display(cur_stream);
                return true;

            }
        }

        for(int i = 0; i <  PALLETE_COUNT; ++i)
        {
            if(hcp_clickedOnButton(event, &ui_settigs->pallete.rect[i])) {

                 ui_settigs->pallete.selected = i;

                setSubTitlesFontColor(&ui_subtitles,  ui_settigs->pallete.color[i] );

                 printf("\n settings_tab \n  %s ", ui_settigs->pallete.label);
                 ui_buttons->delayTimer = 360;
                 video_display(cur_stream);
                 return true;
             }
        }

        for(int i=0; i < 1; i++) {
            if(hcp_clickedOnButton(event, &ui_settigs->checkbox[i].rect)) {

                ui_settigs->checkbox[i].checked  = !ui_settigs->checkbox[i].checked;

                if(ui_settigs->checkbox[i].checked )
                {
                    ui_subtitles.subtilesPos.y  = cur_stream->height - 50;
                    ui_subtitles.subtilesPos.x  = cur_stream->width/2 - 50;
                }
                else
                {
                    ui_subtitles.subtilesPos.y  =  10;
                    ui_subtitles.subtilesPos.x  = cur_stream->width/2 - 50;
                }
                ui_buttons->delayTimer = 360;
                video_display(cur_stream);
                return true;
            }
        }
    }
    else if( ui_settigs->activeTab == 3)
    {
      
        for(int i=1; i < CHECKBOX_COUNT; i++) {
            if(hcp_clickedOnButton(event, &ui_settigs->checkbox[i].rect)) {
               ui_settigs->checkbox[i].checked  = !ui_settigs->checkbox[i].checked;

                if(ui_settigs->checkbox[i].checked )
                {
                    cur_stream->stereo3D = true;
                    if(i==1)
                        strcpy( cur_stream->setero3Str , "stereo3d=sbs2l:agmd");        
                    else
                       strcpy( cur_stream->setero3Str ,  "stereo3d=sbs2l:arcd");         
                            
                }
                else
                {
                   cur_stream->stereo3D = false;
                }
                ui_buttons->delayTimer = 360;
                video_display(cur_stream);
                return true;
            }
        }
    }
        

    if(hcp_clickedOnButton(event, &ui_buttons->forward_button )) {

        float seek_interval = 10;
        double incr = seek_interval ? seek_interval : 10.0;
        mouse_fn( cur_stream,  seek_by_bytes,  incr );

        #ifdef DEBUG
        printf("\n forward_button (%d,%d) \n ", ui_buttons->forward_button.x,  ui_buttons->forward_button.y);
        #endif
        return true;
    }

    if(hcp_clickedOnButton(event, &ui_buttons->backword_button )) {

        float seek_interval = 10;
        double incr = seek_interval ? -seek_interval : -10.0;
        mouse_fn( cur_stream,  seek_by_bytes,  incr );

        #ifdef DEBUG
        printf("\n backword_button (%d,%d) \n ", ui_buttons->backword_button.x,  ui_buttons->backword_button.y);
        #endif

        return true;
    }

    if(hcp_clickedOnButton(event, &ui_buttons->plus_button )) {
        update_volume(cur_stream, 1, SDL_VOLUME_STEP);

        #ifdef DEBUG
        printf("\n vol plus_button (%d,%d) \n ", ui_buttons->plus_button.x,  ui_buttons->plus_button.y);
        #endif
        ui_buttons->delayTimer = 360;
        video_display(cur_stream);
        return true;
    }

    if(hcp_clickedOnButton(event, &ui_buttons->minus_button )) {
        update_volume(cur_stream, -1, SDL_VOLUME_STEP);

        #ifdef DEBUG
        printf("\n vol minus_button(%d,%d) \n ", ui_buttons->minus_button.x,  ui_buttons->minus_button.y);
        #endif
        ui_buttons->delayTimer = 360;
        video_display(cur_stream);
        return true;
    }

   return false;
}





 void setSubTitlesFontSize( st_ui_subtitles *ui_subtitles, int ptsize)
{
   // TTF_SetFontSize(fontPlayer, ptsize);
     TTF_CloseFont( ui_subtitles->fontSubtitles );
    ui_subtitles->fontSubtitles = TTF_OpenFont("skin/Inter-Light.ttf", ptsize);
    if (ui_subtitles->fontSubtitles == NULL) {
       fprintf(stderr, "error: font not found\n");
       exit(EXIT_FAILURE);
    }
}

void setSubTitlesFontColor(st_ui_subtitles *ui_subtitles, SDL_Color subFonCol)
{
    ui_subtitles->subtilesFontCol = subFonCol;
}

void setSubTitlesPosition(st_ui_subtitles *ui_subtitles, SDL_Point subPos)
{
    ui_subtitles->subtilesPos = subPos;
}

void setSubTitlesValue(st_ui_subtitles *ui_subtitles, int tab , int value )
{

    switch(tab)
    {
        case 0:
          setSubTitlesFontSize(ui_subtitles,value);
        break;
        case 1:
          ui_subtitles->subtilesPos.x = value;
        break;
//        case 2:
//          ui_subtitles->subtilesFontCol.r = value;
//        break;
//        case 3:
//          ui_subtitles->subtilesFontCol.g = value;
//        break;
//        case 4:
//          ui_subtitles->subtilesFontCol.b = value;
//        break;

    }
}

void get_text_and_rect(SDL_Renderer *renderer, int x, int y, char *text,  TTF_Font *font, SDL_Texture **texture, SDL_Rect *rect,  SDL_Color textColor)
{
    int text_width;
    int text_height;
    SDL_Surface *surface;

    surface = TTF_RenderUTF8_Blended(font, text, textColor);
    if(*texture)
         SDL_DestroyTexture( *texture );

    *texture = SDL_CreateTextureFromSurface(renderer, surface);
    text_width = surface->w;
    text_height = surface->h;
    SDL_FreeSurface(surface);
    rect->x = x;
    rect->y = y;
    rect->w = text_width;
    rect->h = text_height;
}




void getMovieDuration( const AVFormatContext *fmt_ctx,  int index)
{

    AVStream *st = fmt_ctx->streams[index];

    if (st->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
        int bfps = st->avg_frame_rate.den && st->avg_frame_rate.num;

        if (bfps)
        {
            printf("fps %f \n" ,  av_q2d(st->avg_frame_rate));
            ui_buttons->fps = av_q2d(st->avg_frame_rate);
        }
    
        AVRational *time_base = &fmt_ctx->streams[index]->time_base;

        if (fmt_ctx->duration != AV_NOPTS_VALUE)
        {
            int hours, mins, secs, us;
            int64_t duration = fmt_ctx->duration + (fmt_ctx->duration <= INT64_MAX - 5000 ? 5000 : 0);
            secs  = duration / AV_TIME_BASE;
            us    = duration % AV_TIME_BASE;

           // printf( "%02d.%02d \n ", secs, (100 * us) / AV_TIME_BASE);
            ui_buttons->progressbar.mvduration =  secs;
            mins  = secs / 60;
            secs %= 60;
            hours = mins / 60;
            mins %= 60;
            //printf( "%02d:%02d:%02d.%02d \n", hours, mins, secs, (100 * us) / AV_TIME_BASE);
    //        printf("%s: pts:%s pts_time:%s dts:%s dts_time:%s duration:%s duration_time:%s stream_index:%d\n",           tag,
    //        av_ts2str(pkt->pts), av_ts2timestr(pkt->pts, time_base),
    //        av_ts2str(pkt->dts), av_ts2timestr(pkt->dts, time_base),
    //        av_ts2str(pkt->duration), av_ts2timestr(pkt->duration, time_base),
    //        pkt->stream_index);
          //  ui_subtitles->mvCurPos   = av_q2d(*time_base) * pkt->pts;

            printf( "\n progressbar duration %f secs \n", ui_buttons->progressbar.mvduration);

        }

    }
}


 void mouse_fn( VideoState *cur_stream, int seek_by_bytes, double incr )
 {
    double  pos;

    if (seek_by_bytes)
    {
      pos = -1;
      if (pos < 0 && cur_stream->video_stream >= 0)
          pos = frame_queue_last_pos(&cur_stream->pictq);
      if (pos < 0 && cur_stream->audio_stream >= 0)
          pos = frame_queue_last_pos(&cur_stream->sampq);
      if (pos < 0)
          pos = avio_tell(cur_stream->ic->pb);
      if (cur_stream->ic->bit_rate)
          incr *= cur_stream->ic->bit_rate / 8.0;
      else
          incr *= 180000.0;
      pos += incr;
      stream_seek(cur_stream, pos, incr, 1);

    } else {
      pos = get_master_clock(cur_stream);

    //  printf("\n pos0 %f,  incr  %f \n", pos, incr );

    if (isnan(pos))
        pos = (double)cur_stream->seek_pos / AV_TIME_BASE;
    pos += incr;

    //  printf("\n pos1 %f,  incr  %f \n", pos, incr );

    if (cur_stream->ic->start_time != AV_NOPTS_VALUE && pos < cur_stream->ic->start_time / (double)AV_TIME_BASE)
         pos = cur_stream->ic->start_time / (double)AV_TIME_BASE;

    printf("\n seek to pos %f  \n", pos );

    stream_seek(cur_stream, (int64_t)(pos * AV_TIME_BASE), (int64_t)(incr * AV_TIME_BASE), 0);
    }

 }


static bool file_exists(const char *filename)
{
    FILE *fp = fopen(filename, "r");
    bool is_exist = false;
    if (fp != NULL)
    {
        is_exist = true;
        fclose(fp); // close the file
    }
    return is_exist;
}

bool getFromPlaylist(char *line)
{


   FILE *sync = fopen("skin/playlist.txt", "r");
   if( sync ) {
        if( fgets(line, 1024, sync) !=NULL ) {
           printf("First line %s\n", line); //<this is just a log... you can remove it
           line[strcspn(line, "\n")] = '\0';
        }

      fclose(sync);
      // This will look for Time and it will discard it collecting the number you are looking for in n2 and n3

      return file_exists(line);

   }
   else return false;
}

void cleanPlaylist()
{
    cleanup_settings_ui(ui_settigs);
}


void resetPlaylist(VideoState *cur_stream)
{
    ui_settigs->slider[0].cur = 1; //contrast; // -1000.0, 1000.0); (default “1.0”)
    ui_settigs->slider[1].cur = 0; //brightness; // -1.0, 1.0); (default “0.0”)
    ui_settigs->slider[2].cur = 1; //saturation;  //  0.0, 3.0); (default “1.0”)
    ui_settigs->slider[3].cur = 1; //speed
    cur_stream->vfilterSpeed = 1;
    cur_stream->saturation = 1;  //  0.0, 3.0); (default “1.0”)
    cur_stream->brightness = 0; // -1.0, 1.0); (default “0.0”)
    cur_stream->contrast =1;  // -1000.0, 1000.0); (default “1.0”)
    cur_stream->stereo3D = false;;
}


void hcp_setting(SDL_Renderer *renderer, int width, int height, ui_buttons_s *ui_buttons, settings_ui_t *ui_settigs )
{

    hcp_setting_draw(renderer, width, height, ui_buttons, ui_settigs, ui_settigs->fileplaypage); //arvind
 }

 bool mouseMotion(VideoState *cur_stream, SDL_Event *event, SDL_Window *window) {
    SDL_Point pt = {event->motion.x, event->motion.y};

    if(event->motion.state & SDL_BUTTON_LMASK)
    {
        if (SDL_PointInRect(&pt, &ui_buttons->progressbar.bar_bg_duplicate) ) { // mouse is pressed inside the "volume" "slider"?
            const float fx = (float) (pt.x - ui_buttons->progressbar.bar_bg.x);
            float slider_value = (fx / ((float) ui_buttons->progressbar.bar_bg.w)); // a value between 0.0f and 1.0f

            ui_buttons->progressbar.cur = slider_value*( ui_buttons->progressbar.mvduration );

            #ifdef DEBUG
                // printf("SLIDING! progressbar.cur %f slider_value %f \n",  ui_buttons->progressbar.cur, slider_value);
            #endif

            ui_buttons->delayTimer = 360;
            video_display(cur_stream);
            printf("\n mouseMotion progressbar\n" );
            return true;
        } else
        {

            if( ui_settigs->activeTab == 2)
            for(int i=0; i < SLIDER_COUNT; i++) {

                if (SDL_PointInRect(&pt, &ui_settigs->slider[i].bar)) {


                    const float fx = (float) (pt.x - ui_settigs->slider[i].bar.x);

                    float slider_value = (fx / ((float) ui_settigs->slider[i].bar.w)); // a value between 0.0f and 1.0f

                    #ifdef DEBUG
                    //printf("SLIDING! At %s (%f percent), current %f \n",  ui_settigs->slider[i].label, slider_value, ui_settigs->slider[i].min +  slider_value*( ui_settigs->slider[i].max - ui_settigs->slider[i].min));
                    #endif
                    ui_settigs->slider[i].cur = ui_settigs->slider[i].min +  slider_value*( ui_settigs->slider[i].max - ui_settigs->slider[i].min);
                    
                    //if( cur_stream->vfilterSpeed == 1.0 )
                    {
                        switch(i)
                        {
                            case 0:
                                cur_stream->contrast = ui_settigs->slider[i].cur;
                            break;

                            case 1:
                                cur_stream->brightness = ui_settigs->slider[i].cur;
                            break;

                             case 2:
                                cur_stream->saturation = ui_settigs->slider[i].cur;
                            break;

                             case 3:
                             // if (!cur_stream->vfilterSpeed) cur_stream->vfilterSpeed = 1;
                                //cur_stream->vfilterSpeed = cur_stream->vfilterSpeed / 2;
                            break;

                            default:
                                printf("\nError not a possible state\n");continue;
                                break;

                        };

                        if( i < 3 &&  cur_stream->paused)
                        {
                            step_to_next_frame(cur_stream);
                        }
                        ui_buttons->delayTimer = 360;
                        video_display(cur_stream);
                    }

                    return true;

                }//if

            }//for
        }
    }


    return false;
}

bool mouseUp(VideoState *cur_stream, SDL_Event *event, SDL_Window *window) {
    if(ui_buttons->progressbar.cur   )
    {
        //printf("Final POSSTION %f", ui_buttons->progressbar.cur);
        //fflush(stdout);
        double PrevPos = ui_buttons->progressbar.mvduration * ui_buttons->progressbar.bar.w / ui_buttons->progressbar.bar_bg.w;
       // double CurPos = ui_buttons->progressbar.mvduration * (ui_buttons->progress_knob.x -  ui_buttons->progressbar.bar.x )/ ui_buttons->progressbar.bar_bg.w;

        printf( "\n PrevPos %f  CurPos %f  \n", PrevPos, ui_buttons->progressbar.cur );
        mouse_fn(cur_stream, 0, (ui_buttons->progressbar.cur  - PrevPos));
        // ui_buttons->progressbar.cur = 0;
        ui_buttons->progressbar.cur = 0;
       
       return true;
    }

    if( ui_settigs->activeTab == 2 )
    {
        // if(!cur_stream->vfilterSpeed) cur_stream->vfilterSpeed = 1;
        //if( 1/ui_settigs->slider[3].cur <  cur_stream->vfilterSpeed)
        cur_stream->vfilterSpeed = 1.0 / ui_settigs->slider[3].cur ;

    }
    return false;
}


 void hcp_fill_rounded_box_b(SDL_Surface *dst, SDL_Rect *coords, int r, SDL_Color color)
{
    Uint32 *pixels = NULL;
    Uint32 pixcolor = SDL_MapRGBA(dst->format, color.r, color.g, color.b, 115);

    int i, j;
    int rpsqrt2 = (int)(r / sqrt(2));
    int yd = dst->pitch / dst->format->BytesPerPixel;
    int w = coords->w / 2 - 1;
    int h = coords->h / 2 - 1;
    int xo = coords->x + w;
    int yo = coords->y + h;
    int sy = 0;
    int ey = 0;
    int sx = 0;
    int ex = 0;
    int d = 0;
    int x2m1 = 0;
    int y = 0;

    w -= r;
    h -= r;

    if (w <= 0 || h <= 0)
        return;

    SDL_LockSurface(dst);
    pixels = (Uint32 *)(dst->pixels);

    sy = (yo - h) * yd;
    ey = (yo + h) * yd;
    sx = xo - w;
    ex = xo + w;

    for (i = sy; i <= ey; i += yd)
        for (j = sx - r; j <= ex + r; j++)
            pixels[i + j] = pixcolor;

    d = -r;
    x2m1 = -1;
    y = r;

    for (int x = 0; x <= rpsqrt2; x++)
    {
        x2m1 += 2;
        d += x2m1;
        if (d >= 0)
        {
            y--;
            d -= y * 2;
        }

        for (i = sx - x; i <= ex + x; i++)
        {
            pixels[sy - y * yd + i] = pixcolor;
        }

        for (i = sx - y; i <= ex + y; i++)
        {
            pixels[sy - x * yd + i] = pixcolor;
        }

        for (i = sx - y; i <= ex + y; i++)
        {
            pixels[ey + x * yd + i] = pixcolor;
        }

        for (i = sx - x; i <= ex + x; i++)
        {
            pixels[ey + y * yd + i] = pixcolor;
        }
    }

    SDL_UnlockSurface(dst);
}

void drawVolume( SDL_Renderer *renderer, ui_buttons_s *ui_buttons,  int vol)
{
    //printf("\n drawVolume %d \n", vol );
    int maxVol = ui_buttons->plus_button.x - ui_buttons->minus_button.x + ui_buttons->plus_button.w;
    SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255);
    SDL_Rect maxRect = (SDL_Rect){ui_buttons->minus_button.x, ui_buttons->minus_button.y - 12, maxVol , 8 };
    SDL_RenderDrawRect(renderer, &maxRect);
    float curVol = (maxVol*vol)/128.0;
    maxRect.x += 1;
    maxRect.y += 1;
    maxRect.h -= 2;
    maxRect.w = (int)curVol;
    SDL_SetRenderDrawColor(renderer, 255, 213, 128, 80);
    SDL_RenderFillRect(renderer, &maxRect);
}
 

//static int random_number(int min, int max) {
//    srand(time(NULL));
//    return (rand() % (max - min + 1)) + min;
//}

  
bool getNextPlayList(  char *path )
{
//    //printf("\n getNextPlayList %s \n" );
// 
//    int totalCount = 0;
//    {
//        stPlayList *list= fileplaylist;
//        while(list != NULL)
//        {
//           ++totalCount;
//           list = list->next;
//        }
//    }

    if( ui_settigs->activePlaylistButton == 2)
    {
        if(ui_settigs->fileplayactive && ui_settigs->fileplayactive->next)
        {
            ui_settigs->fileplayactive = ui_settigs->fileplayactive->next;
        }
        else
        ui_settigs->fileplayactive = ui_settigs->fileplaylist;
    }
    else if( ui_settigs->fileplayactive == 1 )
    {
         //ui_settigs->fileplayactive = random_number(0, totalCount -1);
        if(ui_settigs->fileplayactive && ui_settigs->fileplayactive->next)
        {
            ui_settigs->fileplayactive = ui_settigs->fileplayactive->next;
        }
        else
        ui_settigs->fileplayactive = ui_settigs->fileplaylist;
    }
    else
        return false;

  
    if( ui_settigs->fileplayactive)
    {
        printf("\n getNextPlayList %s \n",  ui_settigs->fileplayactive->path);
        strcpy( path, ui_settigs->fileplayactive->path);
        return true;
    }
    return false;
}


void renderGUI(VideoState *is, SDL_Renderer *renderer, int *cursor_hidden) 
{


    if(!*cursor_hidden || !is->nCount || is->paused || ui_buttons->delayTimer > 0)
    {
        if(ui_buttons->delayTimer )
        --ui_buttons->delayTimer;

        ui_buttons->muted = is->muted;
       // ui_buttons->paused = is->paused;
        hcp_render_ui(renderer, is->paused , is->width, is->height, ui_buttons);

      if(ui_buttons->progressbar.bgrect.w !=  ui_buttons->playcontrol_bg.w || ui_buttons->progressbar.bgrect.h !=  ui_buttons->playcontrol_bg.h )
      {

            if(ui_buttons->progressbar.playCtrlSurface)
            {
                SDL_FreeSurface( ui_buttons->progressbar.playCtrlSurface);
                ui_buttons->progressbar.playCtrlSurface=NULL;
            }
            if(   ui_buttons->progressbar.playCtrlTexture) {
                SDL_DestroyTexture( ui_buttons->progressbar.playCtrlTexture);
               ui_buttons->progressbar.playCtrlTexture=NULL;
            }
            //bgrect.x= bgrect.y = 0;
            ui_buttons->progressbar.bgrect.w = ui_buttons->playcontrol_bg.w;
            ui_buttons->progressbar.bgrect.h = ui_buttons->playcontrol_bg.h;

            int radius = ui_buttons->progressbar.bgrect.h * 0.18;
            ui_buttons->progressbar.playCtrlSurface = SDL_CreateRGBSurface(0, ui_buttons->progressbar.bgrect.w, ui_buttons->progressbar.bgrect.h, 32 ,0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);

            hcp_fill_rounded_box_b(   ui_buttons->progressbar.playCtrlSurface, &ui_buttons->progressbar.bgrect, radius, ui_buttons->progressbar.bg_color);
            ui_buttons->progressbar.playCtrlTexture = SDL_CreateTextureFromSurface(renderer, ui_buttons->progressbar.playCtrlSurface);
        }

        SDL_RenderCopy(renderer, ui_buttons->progressbar.playCtrlTexture, &ui_buttons->progressbar.bgrect, &ui_buttons->playcontrol_bg);

        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

        SDL_Color bluFont =  (SDL_Color){255, 255,255, 0};

//        SDL_Rect rectFps;
//        char buffPFS[15]= "FPS:";
//        gcvt(ui_buttons->fps, 6, &buffPFS[4]);
//        get_text_and_rect(renderer, 0, 0, buffPFS , ui_subtitles.fontPlayer, &fps_texture, &rectFps, bluFont );
//        SDL_RenderCopy(renderer, fps_texture, NULL, &rectFps);

        SDL_Rect rectDuration;
        char buffdruation[15];
        sprintf( buffdruation, "%.2f", ui_buttons->progressbar.mvduration/60 );
        get_text_and_rect(renderer, (is->width*TOTAL_DURATION_X)/100, (is->height*TOTAL_DURATION_Y)/100, buffdruation,
            ui_buttons->fontPlayer, &ui_buttons->duration_texture, &rectDuration, bluFont);
        //rectDuration.w = is->width*0.04;
        SDL_RenderCopy(renderer, ui_buttons->duration_texture, NULL, &rectDuration);

        SDL_Rect rectCurPos;
        char buffcurPos[15];
        sprintf( buffcurPos, "%.2f", is->vidclk.pts/(is->vfilterSpeed*60.0 ));
        get_text_and_rect(renderer, (is->width*CURRENT_DURATION_X)/100, (is->height*CURRENT_DURATION_Y)/100, buffcurPos ,
            ui_buttons->fontPlayer, &ui_buttons->curPos_texture, &rectCurPos, bluFont);
        SDL_RenderCopy(renderer, ui_buttons->curPos_texture, NULL, &rectCurPos);
       // hcp_render_ui(renderer, is, is->width, is->height, ui_buttons);

        SDL_Color blackFont =  (SDL_Color){0, 0, 0, 0};

        SDL_Rect rectVollevel;
        char buffVollevel[15];
        //sprintf( buffVollevel, "%.2f",  );
        sprintf( buffVollevel, "%d", is->audio_volume );
        if(ui_buttons->delayTimer && !is->muted )
        {
            drawVolume(renderer, ui_buttons, is->audio_volume);
        }
        get_text_and_rect(renderer, ui_buttons->volume_button.x + ui_buttons->volume_button.w/6 , ui_buttons->volume_button.y + ui_buttons->volume_button.h/3 , buffVollevel ,
            ui_buttons->fontPlayer, &ui_buttons->volumetexture, &rectVollevel, blackFont);
        SDL_RenderCopy(renderer, ui_buttons->volumetexture, NULL, &rectVollevel);


        ui_buttons->progress_knob.y = ui_buttons->progressbar.bar_bg.y-5;
        ui_buttons->progress_knob.h = ui_buttons->progressbar.bar_bg.h+10;
        ui_buttons->progress_knob.w = 15;

        ui_buttons->progressbar.bar.w = (ui_buttons->progressbar.bar_bg.w*is->vidclk.pts)/(ui_buttons->progressbar.mvduration * is->vfilterSpeed);

        if(ui_buttons->progressbar.cur)
        {
            float tmpFlot = ui_buttons->progressbar.bar_bg.x +  (( ui_buttons->progressbar.cur)*((float)ui_buttons->progressbar.bar_bg.w ))/( ui_buttons->progressbar.mvduration );
            ui_buttons->progress_knob.x = tmpFlot  - ui_buttons->progress_knob.w/2;
        }else
        {
            ui_buttons->progress_knob.x = ui_buttons->progressbar.bar.x + ui_buttons->progressbar.bar.w  - ui_buttons->progress_knob.w/2;
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &ui_buttons->progressbar.bar);

	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderFillRect(renderer, &ui_buttons->progress_knob);


	// we can not create and destory surface on live thread
/*
        if(surface)
            SDL_FreeSurface(surface);
        if(texture)
            SDL_DestroyTexture(texture);
*/

        if(ui_settigs->setting)
        {
           // ui_settigs->win_h=is->height;
           // ui_settigs->win_w = is->width;
            //render_video_settings_ui(renderer, ui_settigs); //KP
            hcp_setting(renderer, is->width, is->height, ui_buttons, ui_settigs ); //arvind

        }
    }

    if(ui_settigs->activePlaylistButton < 1 )
    if(is->nCount == 0)
    {
        toggle_pause(is);
    }

   // printf("\n is->nCount %d \n ",is->nCount);

    is->nCount = is->nCount + 1;

    if(ui_subtitles.isfoundSubtiles)
    {
        SDL_Rect rectTitle;

        get_text_and_rect(renderer, ui_subtitles.subtilesPos.x, ui_subtitles.subtilesPos.y , ui_subtitles.foundSubtiles , ui_subtitles.fontSubtitles, &ui_buttons->titletexture, &rectTitle, ui_subtitles.subtilesFontCol);

        SDL_RenderCopy(renderer, ui_buttons->titletexture, NULL, &rectTitle);

    }
}



void renderSubtiles( Frame *sp)
{

    for (unsigned i = 0; i < sp->sub.num_rects; i++)
    {
        switch (sp->sub.rects[i]->type) {
            case SUBTITLE_ASS:

                printf("%s\n", sp->sub.rects[i]->ass);


                int hour1, min1, sec1, hunsec1, hour2, min2, sec2, hunsec2;
                char line[512], *ret;
                // fixme: "\0" maybe not allowed
                if (sscanf(sp->sub.rects[i]->ass, "Dialogue: Marked=%*d,%d:%d:%d.%d,%d:%d:%d.%d%[^\r\n]", //&nothing,
                        &hour1, &min1, &sec1, &hunsec1,
                        &hour2, &min2, &sec2, &hunsec2,
                        line) < 9) {

                    if (sscanf(sp->sub.rects[i]->ass, "Dialogue: %*d,%d:%d:%d.%d,%d:%d:%d.%d%[^\r\n]", //&nothing,
                            &hour1, &min1, &sec1, &hunsec1,
                            &hour2, &min2, &sec2, &hunsec2,
                            line) < 9)
{
                        //int x = 0;
                    } else {
                        printf("%s\n", line);
                    }

                } else {
                    printf("%s\n", line);
                }

                char *fondit;
                ret = strchr(line, ',');
                if (!ret) {
                    fondit = line;
                } else {
                    static const char kDefaultStyle[] = "Default,";
                    for (int comma = 0; comma < 6; comma++) {
                        if (!(ret = strchr(++ret, ','))) {
                            // workaround for ffmpeg decoded srt in ass format: "Dialogue: 0,0:42:29.20,0:42:31.08,Default,Chinese\NEnglish.
                            if (!(ret = strstr(line, kDefaultStyle))) {
                                if (line[0] == ',') //work around for libav-9-
                                {
                                    fondit = line + 1;
                                } else {
                                    fondit = line;
                                }
                            } else {
                                ret += sizeof (kDefaultStyle) - 1 - 1; // tail \0
                            }
                        }
                    }
                }
                fondit = ++ret;


               ui_subtitles.isfoundSubtiles = 1;
               strcpy( ui_subtitles.foundSubtiles, fondit);

                break;
            case SUBTITLE_TEXT:
                //qDebug("txt frame: %s", sub.rects[i]->text);
                //frame.text.append(QString::fromUtf8(sub.rects[i]->text)).append(ushort('\n'));
                break;
            case SUBTITLE_BITMAP:
                //sub.rects[i]->w > 0 && sub.rects[i]->h > 0
                //qDebug("bmp sub");
                // frame = SubtitleFrame(); // not support bmp subtitle now
                break;
            default:
                break;
        }
    }

}


void cleanUI( )
{

    SDL_DestroyTexture( ui_buttons->duration_texture );
    ui_buttons->duration_texture = NULL;

    SDL_DestroyTexture( ui_buttons->curPos_texture );
    ui_buttons->curPos_texture = NULL;


    SDL_DestroyTexture( ui_buttons->volumetexture );
    ui_buttons->volumetexture = NULL;

    SDL_DestroyTexture( ui_buttons->titletexture );
    ui_buttons->titletexture = NULL;

    TTF_CloseFont( ui_buttons->font ); 
    TTF_CloseFont( ui_buttons->fontPlayer );
    TTF_CloseFont( ui_subtitles.fontSubtitles );

    cleanPlaylist();
      
    TTF_Quit();
    
}


void intSubtitiles()
{
    ui_subtitles.fontSubtitles = TTF_OpenFont("skin/Inter-Light.ttf", 14);
    if (ui_subtitles.fontSubtitles == NULL) {
       fprintf(stderr, "error: font not found\n");
       exit(EXIT_FAILURE);
    }
    
    ui_subtitles.isfoundSubtiles = 0;
    
}

void initUI( SDL_Renderer *renderer, int default_width, int default_height )
{
    TTF_Init();
    /*Initialization of UI*/
    ui_buttons=hcp_init_ui_icons(renderer);
    ui_settigs=hcp_init_settings_ui(renderer);
    ui_buttons->muted = 1;
    
    intSubtitiles();


    ui_buttons->fontPlayer = TTF_OpenFont("skin/Inter-Light.ttf", 12);
    if (ui_buttons->fontPlayer == NULL) {
       fprintf(stderr, "error: font not found\n");
       exit(EXIT_FAILURE);
    }    
    
    

    ui_buttons->font = TTF_OpenFont("skin/Ubuntu-B.ttf", 14);
    if (ui_buttons->font == NULL) {
        printf("error: font not found\n");
        exit(EXIT_FAILURE);
    }

    
    hcp_render_ui(renderer,1, default_width, default_height, ui_buttons);
}


bool checkDuration(VideoState *is)
{
    if( (ui_settigs->activePlaylistButton > 0 ) && is->vidclk.pts/is->vfilterSpeed + .05  >=  ui_buttons->progressbar.mvduration )
    {
        return false;
    }
    return true;
}

void showplayIconOnCenter(int show)
{
    ui_buttons->showplayIconOnCenter= show;
}


void set_audioSpeedFilter(VideoState *is,  char **af,  float *last_speed)
{
    char *afilters =  *af;
    
    if(is->vfilterSpeed !=1 )
    {
        if(afilters == NULL  )
            afilters = malloc(256);
        sprintf(  afilters ,  "atempo=%.2f", 1.0/is->vfilterSpeed );
        printf("\n audio filter %s \n", afilters );
    }
    else
    {   printf("\n reset filter\n"  );
        if(afilters)
            free(afilters);
        afilters = NULL;
        *last_speed = is->vfilterSpeed;
    }

    *last_speed = is->vfilterSpeed;
    
    *af = afilters;
}

void set_videoFilter(VideoState *is, char **vf,  float *last_speed, float *last_contrast, float  *last_brightness, float  *last_saturation,  bool *last_stereo3D)
{
    char *vfilter =  *vf;
    
     if(!vfilter)
         vfilter = malloc(256);
    if( is->vfilterSpeed != *last_speed )
    {
        if(is->vfilterSpeed != 1 &&  is->vfilterSpeed  <   *last_speed  )
        sprintf(  vfilter ,  "setpts=%.2f*PTS", is->vfilterSpeed );
        else if( is->vfilterSpeed == 1 || is->vfilterSpeed  >   *last_speed )
        {
            if(is->vfilterSpeed == 1 )
            {
                printf("clear the filters %f \n", is->vfilterSpeed ); 
                if(vfilter)
                    free(vfilter);
                vfilter = NULL;
                resetPlaylist(is);
            }
            else
            {
                sprintf(  vfilter ,  "setpts=%.2f*PTS", is->vfilterSpeed );
            }
            printf("reset filters and reset seek for speed %f \n", is->vfilterSpeed ); 
              
            float incr = 1;
            mouse_fn( is,  0,  incr );
        }
    }
    else if( is->brightness != *last_brightness || is->contrast != *last_contrast || is->saturation != *last_saturation  )
    {
        
        if( is->brightness != *last_brightness)
        {
             sprintf(  vfilter ,  "eq=brightness=%.2f",  is->brightness );
        }
        else if( is->contrast != *last_contrast)
        {
             sprintf(  vfilter ,  "eq=contrast=%.2f",  is->contrast );
        }
        else if(  is->saturation != *last_saturation)
        {
             sprintf(  vfilter ,  "eq=saturation=%.2f",  is->saturation );
        }
        
        if(  is->vfilterSpeed != 1 )
        {   
            ui_settigs->slider[3].cur =1;
            is->vfilterSpeed = 1;

            printf("reset filters and seek to interval %f \n", is->vfilterSpeed ); 

            float incr = 1;
            mouse_fn( is,  0,  incr );
        }
    }
    else if( is->stereo3D != *last_stereo3D)
    {
       if(is->stereo3D)
       strcpy( vfilter, is->setero3Str);  
       else
       {
            printf("reset stereo3D filters %d \n", is->stereo3D ); 
            if(vfilter)
                free(vfilter);
            vfilter = NULL;
            resetPlaylist(is);
       }
    }
    else
    {
        printf("reset filters %f \n", is->vfilterSpeed ); 
        if(vfilter)
            free(vfilter);
        vfilter = NULL;
        resetPlaylist(is);
    }

    *vf = vfilter;
    *last_speed = is->vfilterSpeed;
    *last_contrast = is->contrast;
    *last_brightness = is->brightness;
    *last_saturation = is->saturation;
    *last_stereo3D = is->stereo3D;
}


bool Stereo3d(VideoState *is)
{
   if(is->stereo3D == true)
       return true;
   else
   return false;
        
   
}
