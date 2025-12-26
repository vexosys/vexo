#include "hc_common.h"
#include "hcp_setting.h"
#include "gui.h"

// Button titlebar_buttons[BUTTON_COUNT] = {
//     {.label = "Video"},
//     {.label = "Audio"},
//     {.label = "Subtitle"},
//     {.label = "Playlist"}};

settings_ui_t *hcp_init_settings_ui(SDL_Renderer *renderer)
{
    printf("hcp_init_settings_ui..\n");
    settings_ui_t *s_ui = av_malloc(sizeof(settings_ui_t));
    if (!s_ui)
    {
        printf("hcp_init_settings_ui: Failed to allocate memory settings context \n");
        return NULL;
    }

    s_ui->font = TTF_OpenFont("skin/Inter-Light.ttf", 24);
    if (s_ui->font == NULL)
    {
        fprintf(stderr, "ERROR: hcp_init_settings_ui font not found\n");
        free(s_ui);
        return NULL;
    }

//    Button buttons[BUTTON_COUNT];
//    Spin subtilesSpin[SPIN_COUNT];
//
//    Slider slider[SLIDER_COUNT];

    for(int i =0; i < BUTTON_COUNT; ++i)
    {
        s_ui->buttons[i].texture = NULL;
    }
    for(int i =0; i < SPIN_COUNT; ++i)
    {
        s_ui->subtilesSpin[i].ltexture = NULL;
        s_ui->subtilesSpin[i].vtexture = NULL;
    }

    for(int i =0; i < SLIDER_COUNT; ++i)
    {
        s_ui->slider[i].texture = NULL;
        s_ui->slider[i].ltexture = NULL;
        s_ui->slider[i].rtexture = NULL;
        s_ui->slider[i].ctexture = NULL;
    }

    for(int i = 0; i <  PageListLen ; ++i)
    {
        s_ui->playlist_rowtexture[i] = NULL;
    }
    
    s_ui->fileplaylist = NULL;
    s_ui->fileplaypage = NULL;

    
    if(!s_ui->fileplaylist )
       readplaylist(&s_ui->fileplaylist);
 
    s_ui->fileplayactive = s_ui->fileplaylist ;
    bool retEof = getPage( s_ui->fileplaylist, true,  &s_ui->fileplaypage);

    for(int i = 0; i <  CHECKBOX_COUNT ; ++i)
    {
       s_ui->checkbox[i].texture = NULL;
       s_ui->checkbox[i].checked = false;
    }


    s_ui->pallete.label = "Color :";
    s_ui->pallete.selected = 0;
    s_ui->pallete.texture = NULL;

    s_ui->fps_texture = NULL;

    s_ui->pallete.color[0] = (SDL_Color) {255,255,255,255  };
    s_ui->pallete.color[1] = (SDL_Color) {255,0,0,255  } ;
    s_ui->pallete.color[2] = (SDL_Color) {0,255,0,255  } ;
    s_ui->pallete.color[3] = (SDL_Color) {0,0,255,255  } ;
    s_ui->pallete.color[4] = (SDL_Color) {0,255,255,255  } ;

    s_ui->subtilesSpin[0].value = 14;
    s_ui->subtilesSpin[1].value = 10;
    // s_ui->subtilesSpin[2].value = 255;
    // s_ui->subtilesSpin[3].value = 255;
    // s_ui->subtilesSpin[4].value = 255;

    s_ui->slider[0].cur = 1; //contrast; // -1000.0, 1000.0); (default “1.0”)
    s_ui->slider[1].cur = 0; //brightness; // -1.0, 1.0); (default “0.0”)
    s_ui->slider[2].cur = 1; //saturation;  //  0.0, 3.0); (default “1.0”)
    s_ui->slider[3].cur = 1; //speed

    s_ui->activeTab = 0;

    //s_ui->playlist_active = 0;
    s_ui->activePlaylistButton = -1;
     //float saturation;
    //float brightness;
   // float contrast;
    s_ui->setting = 0;

     //Delete
    s_ui->delete_icon = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("skin/delete.bmp"));
    if(!s_ui->delete_icon)
    {
        printf("Failed to load delete icon\n");
        exit(EXIT_FAILURE);
    }

    //playlist loop
    s_ui->loop_icon = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("skin/loop.bmp"));
    if(!s_ui->loop_icon)
    {
        printf("Failed to load playlist loop icon\n");
        exit(EXIT_FAILURE);
    }

    //playlist shuffle
    s_ui->shuffle_icon = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("skin/shuffle.bmp"));
    if(!s_ui->shuffle_icon)
    {
        printf("Failed to load playlist shuffle icon\n");
        exit(EXIT_FAILURE);
    }

    //playlist: up
    s_ui->up_icon = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("skin/up.bmp"));
    if(!s_ui->up_icon)
    {
        printf("Failed to load playlist up icon\n");
        exit(EXIT_FAILURE);
    }

    //playlist: down
    s_ui->down_icon = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("skin/down.bmp"));
    if(!s_ui->down_icon)
    {
        printf("Failed to load playlist down icon\n");
        exit(EXIT_FAILURE);
    }

    //playlist: page up
    s_ui->page_up_icon = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("skin/page_up.bmp"));
    if(!s_ui->page_up_icon)
    {
        printf("Failed to load playlist page up icon\n");
        exit(EXIT_FAILURE);
    }

    //playlist: page down
    s_ui->page_down_icon = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("skin/page_down.bmp"));
    if(!s_ui->page_down_icon)
    {
        printf("Failed to load playlist page down icon\n");
        exit(EXIT_FAILURE);
    }

    //playlist: file add
    s_ui->file_add_icon = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("skin/file_add.bmp"));
    if(!s_ui->file_add_icon)
    {
        printf("Failed to load playlist file add icon\n");
        exit(EXIT_FAILURE);
    }
    for(int i = 0; i <  PLAYLIST_BUTTON ; ++i)
    {
       s_ui->playlist_buttons[i].texture = NULL;
       s_ui->playlist_buttons[i].disable = 0; 
    }

    s_ui->playlist_buttons[0].label = "Add File";
    s_ui->playlist_buttons[0].texture = s_ui->file_add_icon;

    s_ui->playlist_buttons[1].label = "shuffle";
    s_ui->playlist_buttons[1].texture = s_ui->shuffle_icon;

    s_ui->playlist_buttons[2].label = "loop";
    s_ui->playlist_buttons[2].texture = s_ui->loop_icon;

    s_ui->playlist_buttons[3].label = "Page Down";
    s_ui->playlist_buttons[3].texture = s_ui->page_down_icon;

    s_ui->playlist_buttons[4].label = "Page UP";
    s_ui->playlist_buttons[4].texture = s_ui->page_up_icon;

    s_ui->playlist_buttons[3].disable = retEof;
    
    return s_ui;
}


void cleanup_settings_ui(settings_ui_t *s_ui)
{

    SDL_DestroyTexture(s_ui->delete_icon);
    SDL_DestroyTexture(s_ui->loop_icon);
    SDL_DestroyTexture(s_ui->shuffle_icon);
    SDL_DestroyTexture(s_ui->up_icon);
    SDL_DestroyTexture(s_ui->down_icon);
    SDL_DestroyTexture(s_ui->page_up_icon);
    SDL_DestroyTexture(s_ui->page_down_icon);
    SDL_DestroyTexture(s_ui->file_add_icon);

    if (s_ui->font )
    {
       // TTF_CloseFont(s_ui->font); // check the crash probably you have to destroy all the surface before closing font. TBD
        s_ui->font = NULL;
    }
    
    deleteList(s_ui->fileplaylist);
    s_ui->fileplaylist = NULL;

}
