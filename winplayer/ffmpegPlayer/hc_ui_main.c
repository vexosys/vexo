#include "hc_common.h"
#include "gui.h"

ui_buttons_s* hcp_init_ui_icons(SDL_Renderer *renderer)
{
    SDL_Surface *play_surface, *pause_surface;
    ui_buttons_s *ui_buttons = av_malloc(sizeof(ui_buttons_s));
    if(!ui_buttons)
    {
        printf("hcp_init_ui_icons: Failed to allocate memory volume context \n");
        return NULL;
    }


    ui_buttons->delayTimer=0;

    ui_buttons->progressbar.bg_color = (SDL_Color){ .r = 0, .g = 0, .g = 0, .a = 153};
    ui_buttons->progressbar.cur = 0;

    ui_buttons->progressbar.playCtrlSurface = NULL;
    ui_buttons->progressbar.playCtrlTexture = NULL;
    ui_buttons->progressbar.bgrect = (SDL_Rect){0,0,0,0 };

    //Settings
    ui_buttons->settings_icon = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("skin/hcsettings.bmp"));
    if(!ui_buttons->settings_icon)
    {
        printf("Failed to load settings icon\n");
        exit(EXIT_FAILURE);
    }

    //Plus
    ui_buttons->plus_icon = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("skin/hcplus.bmp"));
    if(!ui_buttons->plus_icon)
    {
        printf("Failed to load plus icon\n");
        exit(EXIT_FAILURE);
    }

    //Volumes
    ui_buttons->volume_icon=SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("skin/hcvolume.bmp"));
    ui_buttons->mute_icon=SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("skin/mute.bmp"));
    ui_buttons->muted=0;
    if(!ui_buttons->mute_icon || !ui_buttons->volume_icon)
    {
        printf("Failed to load ui_buttons->mute_icon/volume_icon icon(s)\n");
        exit(EXIT_FAILURE);
    }

    //Minus
    ui_buttons->minus_icon = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("skin/hcminus.bmp"));
    if(!ui_buttons->minus_icon)
    {
        printf("Failed to load minus icon\n");
        exit(EXIT_FAILURE);
    }

    //play/pause
    play_surface = SDL_LoadBMP("skin/hcplay.bmp");
    pause_surface = SDL_LoadBMP("skin/hcpause.bmp");

    if (play_surface && pause_surface) {
        ui_buttons->play_icon = SDL_CreateTextureFromSurface(renderer, play_surface);
        ui_buttons->pause_icon = SDL_CreateTextureFromSurface(renderer, pause_surface);
        ui_buttons->showplayIconOnCenter=1;
    } else {
        printf("Failed to create play/pause textures.\n");
        exit(EXIT_FAILURE);
    }

    //Zoomout
    ui_buttons->zoomout=0;
    ui_buttons->zoomout_icon = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("skin/hczoomout.bmp"));
    ui_buttons->zoomin_icon = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("skin/hczoomin.bmp"));
    if(!ui_buttons->zoomout_icon || !ui_buttons->zoomin_icon)
    {
        printf("Failed to load zoomout/zoomin icon\n");
        exit(EXIT_FAILURE);
    }

    //Forward
    //ui_buttons->forward_icon = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("skin/hcforward.bmp"));
    ui_buttons->forward_icon = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("skin/forward.bmp"));
    if(!ui_buttons->forward_icon)
    {
        printf("Failed to load forward_icon\n");
        exit(EXIT_FAILURE);
    }

    //Backward
    //ui_buttons->backward_icon = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("skin/hcbackward.bmp"));
    ui_buttons->backward_icon = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("skin/backward.bmp"));
    if(!ui_buttons->backward_icon)
    {
        printf("Failed to load backward_icon \n");
        exit(EXIT_FAILURE);
    }


    ui_buttons->logo_icon = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("skin/logo.bmp"));
    if(!ui_buttons->logo_icon)
    {
        printf("Failed to load logo icon \n");
        exit(EXIT_FAILURE);
    }

    ui_buttons->duration_texture = NULL;
    ui_buttons->curPos_texture = NULL;
    ui_buttons->titletexture = NULL;
    ui_buttons->volumetexture = NULL;
    
    return ui_buttons;
}


static void hcp_calculate_button_size(int screen_width, int screen_height, int x_percent, int y_percent, SDL_Rect *button) {
    button->x = (x_percent * screen_width)/100;
    button->y = (y_percent * screen_height)/100;
    button->w = (int)(0.018 * screen_width);
    //button->w = button->w > 36 ? 36: button->w;
    button->h = button->w;
    if(button->w < MIN_BUTTONS_WIDTH ) button->w=MIN_BUTTONS_WIDTH;
    if(button->h < MIN_BUTTONS_HEIGHT) button->h = MIN_BUTTONS_HEIGHT;
}

// Render UI elements
void hcp_render_ui(SDL_Renderer *renderer, int paused, int width, int height, ui_buttons_s *ui_buttons) {
    SDL_Texture *icon=NULL;

    //Settings
    hcp_calculate_button_size(width, height, SETTINGS_ICON_X, SETTINGS_ICON_Y,
        &ui_buttons->settings_button);
    ui_buttons->settings_button.w=width*SETTINGS_ICON_WIDTH;
    ui_buttons->settings_button.h=height*SETTINGS_ICON_HIEGHT;
    ui_buttons->settings_button.h=ui_buttons->settings_button.h < 15 ? 15:ui_buttons->settings_button.h;
    SDL_RenderCopy(renderer, ui_buttons->settings_icon, NULL, &ui_buttons->settings_button);

    //Plus
    hcp_calculate_button_size(width, height, PLUS_ICON_X, PLUS_ICON_Y,
        &ui_buttons->plus_button);
    ui_buttons->plus_button.w=width*PLUS_ICON_WIDTH;
    ui_buttons->plus_button.h=height*PLUS_ICON_HEIGHT+5;
    ui_buttons->plus_button.h=ui_buttons->plus_button.h < 15 ? 15:ui_buttons->plus_button.h;
    SDL_RenderCopy(renderer, ui_buttons->plus_icon, NULL, &ui_buttons->plus_button);

    //Volume
    hcp_calculate_button_size(width, height, VOLUME_ICON_X, VOLUME_ICON_Y,
        &ui_buttons->volume_button);
    ui_buttons->volume_button.w=width*VOLUME_ICON_WIDTH;
    ui_buttons->volume_button.h=height*VOLUME_ICON_HEIGHT;
    icon = ui_buttons->muted ? ui_buttons->mute_icon: ui_buttons->volume_icon;
    ui_buttons->volume_button.h=ui_buttons->volume_button.h < 15 ? 15:ui_buttons->volume_button.h;
    SDL_RenderCopy(renderer, icon, NULL, &ui_buttons->volume_button);

    //Minus
    hcp_calculate_button_size(width, height, MINUS_ICON_X, MINUS_ICON_Y,
        &ui_buttons->minus_button);
    ui_buttons->minus_button.w=width*MINUS_ICON_WIDTH;
    ui_buttons->minus_button.h=height*MINUS_ICON_HEIGHT+5;
    ui_buttons->minus_button.h=ui_buttons->minus_button.h < 15 ? 15:ui_buttons->minus_button.h;
    SDL_RenderCopy(renderer, ui_buttons->minus_icon, NULL, &ui_buttons->minus_button);


    //playcontrol_bg
    hcp_calculate_button_size(width, height, PLAY_CONTROL_BG_X, PLAY_CONTROL_BG_Y,
        &ui_buttons->playcontrol_bg);
    ui_buttons->playcontrol_bg.w=width*PLAY_CONTROL_BG_WIDTH;
    ui_buttons->playcontrol_bg.h=height*PLAY_CONTROL_BG_HEIGHT;
    //sui_buttons->playcontrol_bg.h = ui_buttons->playcontrol_bg.h > 60 ? 60: ui_buttons->playcontrol_bg.h;

    //Progress bar bg
    hcp_calculate_button_size(width, height, PROGRESS_BAR_X, PROGRESS_BAR_Y,
        &ui_buttons->progressbar.bar_bg);
    ui_buttons->progressbar.bar_bg.w=width*PROGRESS_BAR_WIDTH;
    ui_buttons->progressbar.bar_bg.h=height*PROGRESS_BAR_HEIGHT;
    ui_buttons->progressbar.bar_bg_duplicate = ui_buttons->progressbar.bar_bg;
    ui_buttons->progressbar.bar_bg_duplicate.y -= 5;
    ui_buttons->progressbar.bar_bg_duplicate.h += 10;

    SDL_SetRenderDrawColor(renderer, 160, 160, 160, 255);
    SDL_RenderFillRect(renderer, &ui_buttons->progressbar.bar_bg);

    //Progress bar
    ui_buttons->progressbar.bar = ui_buttons->progressbar.bar_bg;
    //ui_buttons->progressbar.bar.h=height*0.01;
    ui_buttons->progressbar.bar.w = 0;

    //Play/pause
    if(ui_buttons->showplayIconOnCenter)
    {
        hcp_calculate_button_size(width, height, CENTER_POSITION, CENTER_POSITION,
            &ui_buttons->play_pause_button);
        ui_buttons->play_pause_button.w=ui_buttons->play_pause_button.h +=10;
        paused = 1;
    } else {
        hcp_calculate_button_size(width, height, PLAY_ICON_X, PLAY_ICON_Y,
            &ui_buttons->play_pause_button);
    }
    icon = paused ? ui_buttons->play_icon : ui_buttons->pause_icon;
    SDL_RenderCopy(renderer, icon, NULL, &ui_buttons->play_pause_button);

    //Zoomout
    hcp_calculate_button_size(width, height, ZOOM_ICON_X, ZOOM_ICON_Y,
        &ui_buttons->zoomout_button);

    icon = ui_buttons->zoomout?ui_buttons->zoomin_icon:ui_buttons->zoomout_icon;
    SDL_RenderCopy(renderer, icon, NULL, &ui_buttons->zoomout_button);

    //Forward
    hcp_calculate_button_size(width, height, FORWARD_ICON_X, FORWARD_ICON_Y,
        &ui_buttons->forward_button);
    ui_buttons->forward_button.w=ui_buttons->forward_button.h = width*0.015;
    SDL_RenderCopy(renderer, ui_buttons->forward_icon, NULL, &ui_buttons->forward_button);

    //Backwords
    hcp_calculate_button_size(width, height, BACKWARD_ICON_X, BACKWARD_ICON_Y,
        &ui_buttons->backword_button);
    ui_buttons->backword_button.w=ui_buttons->backword_button.h = width*0.015;
    SDL_RenderCopy(renderer, ui_buttons->backward_icon, NULL, &ui_buttons->backword_button);

    //Logo
    hcp_calculate_button_size(width, height, LOGO_ICON_X, LOGO_ICON_Y,
        &ui_buttons->logo);
    ui_buttons->logo.w=width*0.05;
    ui_buttons->logo.h = height*0.015;
    //printf("ui_buttons->logo.w=%d, ui_buttons->logo.h=%d\n", ui_buttons->logo.w, ui_buttons->logo.h);
    SDL_RenderCopy(renderer, ui_buttons->logo_icon, NULL, &ui_buttons->logo);

}

int hcp_clickedOnButton(SDL_Event *event, SDL_Rect *button)
{
    return (event->button.x >= button->x &&
    event->button.x <= (button->x + button->w) &&
    event->button.y >= button->y &&
    event->button.y <= (button->y + button->h));
}

static void drawSpin(SDL_Renderer *renderer, ui_buttons_s *ui_buttons, Spin *buttons , TTF_Font* font )
{
    SDL_Rect lrect;
    SDL_Rect vrect;
    SDL_Color whiteFont =  {255, 255, 255, 255};
    get_text_and_rect(renderer, buttons->plus.x, buttons->plus.y,  buttons->label , font, &buttons->ltexture, &lrect, whiteFont );
    SDL_RenderCopy(renderer,  buttons->ltexture, NULL, &lrect);
    char tmpbuf[16]={16};
    sprintf(tmpbuf, "%d", buttons->value);
    get_text_and_rect(renderer, buttons->plus.x + 30 + lrect.h  , buttons->plus.y + lrect.h,  tmpbuf , font, &buttons->vtexture, &vrect, whiteFont );

    buttons->plus.h =lrect.h;
    buttons->minus.h =lrect.h;

    buttons->plus.w =lrect.h;
    buttons->minus.w =lrect.h;
    buttons->plus.x = buttons->plus.x;
    buttons->minus.x = vrect.x - vrect.w/2 + 60 ;
    buttons->plus.y = vrect.y ;
    buttons->minus.y = buttons->plus.y;
    SDL_SetRenderDrawColor(renderer, buttons->color.r, buttons->color.g,   buttons->color.b,   buttons->color.a);
    SDL_RenderCopy(renderer, ui_buttons->plus_icon, NULL, &buttons->plus);
    SDL_RenderCopy(renderer, ui_buttons->minus_icon, NULL, &buttons->minus);
    SDL_RenderCopy(renderer,  buttons->vtexture, NULL, &vrect);
}


static void drawSlider(SDL_Renderer *renderer, ui_buttons_s *ui_buttons, Slider *slider , TTF_Font* font )
{

    slider->bar.w -=20;
    slider->bar.h  =20;

   // SDL_Rect lrect;
    SDL_Rect labelrect;
    SDL_Color whiteFont =  {255, 255, 255, 255};
    get_text_and_rect(renderer, slider->bar.x, slider->bar.y,  slider->label , font, &slider->texture, &labelrect, whiteFont );
    SDL_RenderCopy(renderer,  slider->texture, NULL, &labelrect);

    SDL_Rect labelleft;

    char tmpbuf[16]={16};
    sprintf(tmpbuf, "%.1f", slider->min);
    get_text_and_rect(renderer, slider->bar.x, slider->bar.y + labelrect.h  , tmpbuf , font, &slider->ltexture, &labelleft, whiteFont );

    SDL_RenderCopy(renderer,  slider->ltexture, NULL, &labelleft);


    SDL_Rect labelright;

    sprintf(tmpbuf, "%.1f", slider->max);
    get_text_and_rect(renderer, slider->bar.x + slider->bar.w - strlen(tmpbuf)*labelrect.h/2 , labelleft.y  , tmpbuf , font, &slider->rtexture, &labelright, whiteFont );

    SDL_RenderCopy(renderer,  slider->rtexture, NULL, &labelright);

    slider->bar.x +=10;
    slider->bar.y +=  2*labelrect.h;

    SDL_SetRenderDrawColor(renderer,115,115,115,255);
    SDL_RenderFillRect(renderer, &slider->bar);

    slider->knob.y =  slider->bar.y ;
    float tmpFlot =  slider->bar.x +  ((slider->cur - slider->min)*((float)slider->bar.w ))/( slider->max - slider->min);

    float hidePerc = (slider->max - slider->min)/10;
    if( slider->min <  slider->cur- hidePerc  &&   slider->cur + hidePerc  < slider->max )
    {
        sprintf(tmpbuf, "%.1f", slider->cur);
        get_text_and_rect(renderer, (int)tmpFlot, labelright.y  , tmpbuf , font, &slider->ctexture, &labelright, whiteFont );
        SDL_RenderCopy(renderer,  slider->ctexture, NULL, &labelright);
    }
//    float tmpFlot;
//    if(slider->min < 0)
//    tmpFlot = slider->bar.w/2 + ((float)slider->bar.w* slider->cur) / (slider->max - slider->min);
//    else
//    tmpFlot =  ((float)slider->bar.w* slider->cur) / (slider->max - slider->min);

   // printf("\ndrawSlider %s,  cur %.1f, knob Pos x %f\n", slider->label, slider->cur, tmpFlot);


    slider->knob.x =  (int)tmpFlot ;
    slider->knob.w=20;
    slider->knob.h=slider->bar.h;

    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_RenderFillRect(renderer, &slider->knob);
}


static void drawCheckBox (SDL_Renderer *renderer, Checkbox *checkbox , TTF_Font* font )
{
    SDL_Rect rect;
    SDL_Color whiteFont =  {255, 255, 255, 255};

    get_text_and_rect(renderer, checkbox->rect.x, checkbox->rect.y,  checkbox->label , font, &checkbox->texture, &rect, whiteFont );

    SDL_RenderCopy(renderer,  checkbox->texture, NULL, &rect);
    checkbox->rect.x += rect.w;
    // SDL_SetRenderDrawColor(renderer, 0,0,0,255);
    //SDL_RenderDrawRect(renderer, &checkbox->rect);
    SDL_SetRenderDrawColor(renderer, 255,255,255,255);
    SDL_RenderFillRect(renderer, &checkbox->rect);


    if(checkbox->checked)
    {
        SDL_Rect tmp = {checkbox->rect.x+2, checkbox->rect.y+2, checkbox->rect.w -4, checkbox->rect.h -4  };
        SDL_SetRenderDrawColor(renderer, 0,0,255,255);
        SDL_RenderFillRect(renderer, &tmp);
    }
}

static void drawPalleteBox (SDL_Renderer *renderer, Pallete *palletebox , TTF_Font* font )
{
    SDL_Rect rect;
    SDL_Color whiteFont =  {255, 255, 255, 255};

    get_text_and_rect(renderer, palletebox->rect[0].x, palletebox->rect[0].y,  palletebox->label , font, &palletebox->texture, &rect, whiteFont );

    SDL_RenderCopy(renderer,  palletebox->texture, NULL, &rect);

    palletebox->rect[0].y += rect.h;
    palletebox->rect[0].w = 30;
    palletebox->rect[0].h = 20;

    for(int i = 0; i <  PALLETE_COUNT; ++i)
    {
       palletebox->rect[i] = palletebox->rect[0];
       palletebox->rect[i].x = palletebox->rect[i].x + i*30;


       if( palletebox->selected == i)
       {
            SDL_SetRenderDrawColor(renderer, 185, 185, 185, 255);
            SDL_RenderFillRect(renderer, &palletebox->rect[i]);

            SDL_Rect tmp = palletebox->rect[i];
            tmp.x  +=3; tmp.y +=3; tmp.w  -=6; tmp.h  -=6;

            SDL_SetRenderDrawColor(renderer, palletebox->color[i].r, palletebox->color[i].g, palletebox->color[i].b, 255);
            SDL_RenderFillRect(renderer, &tmp);
       }
       else
       {
           SDL_SetRenderDrawColor(renderer, palletebox->color[i].r, palletebox->color[i].g, palletebox->color[i].b, 255);
           SDL_RenderFillRect(renderer, &palletebox->rect[i]);
       }
    }
}

static void drawButton (SDL_Renderer *renderer, Button *buttons , TTF_Font* font)
{
    SDL_Rect rect;
    SDL_Color blackFont =  {0, 0, 0, 255};
    get_text_and_rect(renderer, buttons->rect.x, buttons->rect.y,  buttons->label , font, &buttons->texture, &rect, blackFont );

    buttons->rect.h =rect.h;
    SDL_SetRenderDrawColor(renderer, buttons->color.r, buttons->color.g,   buttons->color.b,   buttons->color.a);
    SDL_RenderFillRect(renderer, &buttons->rect);
    SDL_RenderCopy(renderer,  buttons->texture, NULL, &rect);
}

 void hcp_setting_draw(SDL_Renderer *renderer, int width, int height, ui_buttons_s *ui_buttons, settings_ui_t *ui_settigs,  stPlayList *list)
{
     //EQ
    hcp_calculate_button_size(width, height, EQ_X, EQ_Y,
        &ui_buttons->eq_bg);
    ui_buttons->eq_bg.w=width*0.25;
    ui_buttons->eq_bg.h=height*0.40;
    ui_buttons->eq_bg.x=  width - ui_buttons->eq_bg.w;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 153);
    SDL_RenderFillRect(renderer, &ui_buttons->eq_bg);

    ui_settigs->buttons[0].rect= ui_buttons->eq_bg;
    ui_settigs->buttons[0].rect.w= ui_buttons->eq_bg.w/BUTTON_COUNT;;
    ui_settigs->buttons[0].label = "Playlist";
    ui_settigs->buttons[0].color = (SDL_Color){255, 255, 255, 255};


    ui_settigs->buttons[1].rect= ui_buttons->eq_bg;
    ui_settigs->buttons[1].rect.x= ui_buttons->eq_bg.x + ui_settigs->buttons[0].rect.w;
    ui_settigs->buttons[1].rect.w=  ui_buttons->eq_bg.w/BUTTON_COUNT;;
    ui_settigs->buttons[1].label = "Subtitles";
    ui_settigs->buttons[1].color = (SDL_Color){255, 255, 255, 255};


    ui_settigs->buttons[2].rect= ui_buttons->eq_bg;
    ui_settigs->buttons[2].rect.x= ui_settigs->buttons[1].rect.x + ui_settigs->buttons[1].rect.w;
    ui_settigs->buttons[2].rect.w= ui_buttons->eq_bg.w/BUTTON_COUNT;
    ui_settigs->buttons[2].label = "Video";
    ui_settigs->buttons[2].color = (SDL_Color){255, 255, 255, 255};
    
    ui_settigs->buttons[3].rect= ui_buttons->eq_bg;
    ui_settigs->buttons[3].rect.x= ui_settigs->buttons[2].rect.x + ui_settigs->buttons[2].rect.w;
    ui_settigs->buttons[3].rect.w= ui_buttons->eq_bg.w/BUTTON_COUNT;
    ui_settigs->buttons[3].label = "3D";
    ui_settigs->buttons[3].color = (SDL_Color){255, 255, 255, 255};
    

    ui_settigs->buttons[ui_settigs->activeTab].color = (SDL_Color){185, 185, 185, 255};

    drawButton(renderer, &ui_settigs->buttons[0], ui_settigs->font );
    drawButton(renderer, &ui_settigs->buttons[1], ui_settigs->font );
    drawButton(renderer, &ui_settigs->buttons[2], ui_settigs->font );
    drawButton(renderer, &ui_settigs->buttons[3], ui_settigs->font );


    switch( ui_settigs->activeTab)
    {

        case 0:
        {
            int Deff = 2;
            int inc = 0;

             printf("\nPlaylist\n");

            while(list != NULL)
            {
                //printf("\n%s\n", list->file);
                int ActionButtonCount =1;

                SDL_Color bluFont =  {0, 0, 0, 255};

                SDL_Rect rect;

                ui_settigs->playlist_gridRow[inc] = ui_buttons->eq_bg;
                ui_settigs->playlist_delete[inc]=  ui_settigs->playlist_up[inc] = ui_settigs->playlist_down[inc]  = ui_buttons->eq_bg;

                ui_settigs->playlist_gridRow[inc].x  += 4;


                ui_settigs->playlist_delete[inc].x += -20*ActionButtonCount++ + ui_buttons->eq_bg.w;

                ui_settigs->playlist_up[inc].x += -20*ActionButtonCount++ + ui_buttons->eq_bg.w;

                ui_settigs->playlist_down[inc].x += -20*ActionButtonCount++ + ui_buttons->eq_bg.w;


                ui_settigs->playlist_gridRow[inc].y += EQ_Y+ 20 + Deff;
                ui_settigs->playlist_delete[inc].y = ui_settigs->playlist_up[inc].y = ui_settigs->playlist_down[inc].y = ui_settigs->playlist_gridRow[inc].y ;

                if(list == ui_settigs->fileplayactive )
                {
                    SDL_SetRenderDrawColor(renderer, 0, 250, 0, 255);
                }
                else
                {
                    if(inc % 2 )
                    SDL_SetRenderDrawColor(renderer, 185, 185, 185, 255);
                    else
                    SDL_SetRenderDrawColor(renderer, 115, 115, 115, 255);
                }

                rect = ui_settigs->playlist_gridRow[inc];
                ui_settigs->playlist_gridRow[inc].w = ui_buttons->eq_bg.w/2;
                ui_settigs->playlist_delete[inc].w = ui_settigs->playlist_up[inc].w = ui_settigs->playlist_down[inc].w  = 20;

                get_text_and_rect(renderer, ui_settigs->playlist_gridRow[inc].x, ui_settigs->playlist_gridRow[inc].y,  list->file , ui_buttons->font, &ui_settigs->playlist_rowtexture[inc], &rect, bluFont );

                ui_settigs->playlist_gridRow[inc].h =rect.h;
                ui_settigs->playlist_delete[inc].h =ui_settigs->playlist_up[inc].h = ui_settigs->playlist_down[inc].h = rect.h;

                //SDL_RenderFillRect(renderer, &ui_settigs->playlist_gridRow[inc]);
                SDL_Rect tmp = {ui_settigs->playlist_gridRow[inc].x, ui_settigs->playlist_gridRow[inc].y, 2*ui_settigs->playlist_gridRow[inc].w, ui_settigs->playlist_gridRow[inc].h};
                SDL_RenderFillRect(renderer, &tmp);

                SDL_RenderCopy(renderer, ui_settigs->playlist_rowtexture[inc], NULL, &rect);

                SDL_RenderCopy(renderer, ui_settigs->delete_icon, NULL, &ui_settigs->playlist_delete[inc]);

                SDL_RenderCopy(renderer, ui_settigs->down_icon, NULL, &ui_settigs->playlist_down[inc]);

                SDL_RenderCopy(renderer, ui_settigs->up_icon, NULL, &ui_settigs->playlist_up[inc]);


                Deff = Deff + rect.h;
                list = list->next;
                if( PageListLen == ++inc)
                {
                    break;
                }

            }

            SDL_Rect tmp=ui_buttons->eq_bg;
            tmp.h=ui_settigs->buttons[0].rect.h;
            tmp.y=ui_buttons->eq_bg.y + ui_buttons->eq_bg.h -  tmp.h;
            SDL_SetRenderDrawColor(renderer, 135, 135, 145, 153);
            SDL_RenderFillRect(renderer, &tmp);

            for(int i = 0; i <  PLAYLIST_BUTTON ; ++i)
            {
                ui_settigs->playlist_buttons[i].rect = tmp;
                ui_settigs->playlist_buttons[i].rect.x += 4 + i*50;
                ui_settigs->playlist_buttons[i].rect.w = 30;
                ui_settigs->playlist_buttons[i].rect.h = 30;
                if( ui_settigs->playlist_buttons[i].disable)
                   SDL_SetTextureAlphaMod( ui_settigs->playlist_buttons[i].texture, 65);
                else
                   SDL_SetTextureAlphaMod( ui_settigs->playlist_buttons[i].texture, 255);
                 
                SDL_RenderCopy(renderer, ui_settigs->playlist_buttons[i].texture, NULL, &ui_settigs->playlist_buttons[i].rect);

                if(ui_settigs->activePlaylistButton > -1 && ui_settigs->activePlaylistButton == i && !ui_settigs->playlist_buttons[i].disable )
                {
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 120);
                    SDL_Rect maxRect = (SDL_Rect){ui_settigs->playlist_buttons[i].rect.x, ui_settigs->playlist_buttons[i].rect.y+3,  ui_settigs->playlist_buttons[i].rect.w,ui_settigs->playlist_buttons[i].rect.h -6};
                    SDL_RenderDrawRect(renderer, &maxRect);
                }
            }


        }
        break;
        case 1: // Subtiles
        {
            printf("\nSubtiles\n");
            ui_settigs->subtilesSpin[0].plus = ui_buttons->eq_bg;
            ui_settigs->subtilesSpin[0].plus.x += 20;
            ui_settigs->subtilesSpin[0].plus.y += 40;
            ui_settigs->subtilesSpin[0].label = "Font Size :";
            //ui_settigs->subtilesSpin[0].value = 14;
            ui_settigs->subtilesSpin[0].color = (SDL_Color){255, 255, 255, 255};
            drawSpin(renderer, ui_buttons,  &ui_settigs->subtilesSpin[0], ui_buttons->font );

            ui_settigs->subtilesSpin[1].plus = ui_buttons->eq_bg;
            ui_settigs->subtilesSpin[1].plus.x += 20;
            ui_settigs->subtilesSpin[1].plus.y += 90;
            ui_settigs->subtilesSpin[1].label = "Pos X :";
           // ui_settigs->subtilesSpin[1].value = 125;
            ui_settigs->subtilesSpin[1].color = (SDL_Color){255, 255, 255, 255};
            drawSpin(renderer, ui_buttons,  &ui_settigs->subtilesSpin[1], ui_buttons->font );

            ui_settigs->pallete.rect[0]= ui_settigs->subtilesSpin[1].plus;
            ui_settigs->pallete.rect[0].y += 22;
            drawPalleteBox(renderer, &ui_settigs->pallete, ui_buttons->font );


            ui_settigs->checkbox[0].rect = (SDL_Rect){ui_settigs->subtilesSpin[1].plus.x , ui_settigs->subtilesSpin[1].plus.y+68, 16, 16 };
            ui_settigs->checkbox[0].label = "Bottom Aline :    ";
            drawCheckBox (renderer,  &ui_settigs->checkbox[0] , ui_buttons->font );
        }
        break;
        case 2: // Videos
        {

    //float saturation;  //  0.0, 3.0); (default “1.0”)
    //float brightness; // -1.0, 1.0); (default “0.0”)
   // float contrast;  // -1000.0, 1000.0); (default “1.0”)


            ui_settigs->slider[0].bar = ui_buttons->eq_bg;
            ui_settigs->slider[0].bar.y +=  ui_settigs->buttons[0].rect.h + 10;
            ui_settigs->slider[0].label = "Contrast :";
            ui_settigs->slider[0].min = -10.0;
            ui_settigs->slider[0].max = 10.0;
            drawSlider(renderer, ui_buttons,  &ui_settigs->slider[0], ui_buttons->font );

            ui_settigs->slider[1].bar = ui_buttons->eq_bg;
            ui_settigs->slider[1].bar.y = ui_settigs->slider[0].bar.y + 30;
            ui_settigs->slider[1].label = "brightness :";
            ui_settigs->slider[1].min = -1.0;
            ui_settigs->slider[1].max = 1.0;
            drawSlider(renderer, ui_buttons,  &ui_settigs->slider[1], ui_buttons->font );

            ui_settigs->slider[2].bar = ui_buttons->eq_bg;
            ui_settigs->slider[2].bar.y = ui_settigs->slider[1].bar.y + 30;
            ui_settigs->slider[2].label = "Saturation :";
            ui_settigs->slider[2].min = 0.0;
            ui_settigs->slider[2].max = 3.0;
            drawSlider(renderer, ui_buttons,  &ui_settigs->slider[2], ui_buttons->font );


            ui_settigs->slider[3].bar = ui_buttons->eq_bg;
            ui_settigs->slider[3].bar.y =  ui_settigs->slider[2].bar.y + 30;
            ui_settigs->slider[3].label = "Speed :";
            ui_settigs->slider[3].min = 1;
            ui_settigs->slider[3].max = 5;
            drawSlider(renderer, ui_buttons,  &ui_settigs->slider[3], ui_buttons->font );

            SDL_Color whiteFont =  (SDL_Color){255, 255,255, 0};
            SDL_Rect rectFps;
            char buffPFS[15]= "FPS: ";
            gcvt(ui_buttons->fps, 6, &buffPFS[5]);
            get_text_and_rect(renderer, ui_buttons->eq_bg.x, ui_settigs->slider[3].bar.y+30, buffPFS , ui_buttons->font, &ui_settigs->fps_texture, &rectFps, whiteFont );
            SDL_RenderCopy(renderer, ui_settigs->fps_texture, NULL, &rectFps);

        }
        break;
        case 3: //3dAnaglyph
        {
            ui_settigs->checkbox[1].rect = (SDL_Rect){ui_buttons->eq_bg.x , ui_buttons->eq_bg.y  + ui_settigs->buttons[0].rect.h +10, 16, 16 };
            ui_settigs->checkbox[1].label = "Red cyan dubois:    ";
            drawCheckBox (renderer,  &ui_settigs->checkbox[1] , ui_buttons->font );
            
            //ui_settigs->checkbox[1].checked = 1;

            ui_settigs->checkbox[2].rect = (SDL_Rect){ ui_buttons->eq_bg.x ,   ui_settigs->checkbox[1].rect.y+16+4, 16, 16 };
            ui_settigs->checkbox[2].label = "Green magenta dubois:    ";
            drawCheckBox (renderer,  &ui_settigs->checkbox[2] , ui_buttons->font );
        }
        break;
    };

/*
    if(font !=NULL)
    {
        TTF_CloseFont(font);
        font = NULL;
    }
*/
}
