#ifndef GUISETTING_H_
#define GUISETTING_H_

#include "hc_common.h"

#define MENU_Y_RATIO 0.05  // Title bar height (5% of window height)
#define MENU_X_RATIO 0.70  // Start at 76% of window width
#define MENU_WIDTH_RATIO 0.29  // Width is 23% of window width
#define CONTENT_HEIGHT_RATIO 0.60  // Content area height (60% of window height)
#define BUTTON_COUNT 4
#define SPIN_COUNT 2

#define SLIDER_COUNT 4
#define CHECKBOX_COUNT 1

#define PLAYLIST_BUTTON 5

//#define TITLE_HEIGHT_MIN 25
//#define TITLE_HEIGHT_MAX 35
//#define SCROLLBAR_HEIGHT 12
//#define MAX_PLAYLIST_ITEMS 10
//#define ITEM_HEIGHT 30

#define MINUS_BUTTON_SIZE 20

typedef struct {

    Button buttons[BUTTON_COUNT];
    Spin subtilesSpin[SPIN_COUNT];
    Slider slider[SLIDER_COUNT];
    Checkbox checkbox[CHECKBOX_COUNT];
    Pallete pallete;

    int activeTab;

    bool setting;
    int activePlaylistButton;


    //int playlist_active;
    SDL_Rect playlist_delete[PageListLen];
    SDL_Rect playlist_up[PageListLen];
    SDL_Rect playlist_down[PageListLen];
    SDL_Rect playlist_gridRow[PageListLen];
    SDL_Texture *playlist_rowtexture[PageListLen];
    PlayListButtons playlist_buttons[PLAYLIST_BUTTON];
    
    stPlayList *fileplaylist;
    stPlayList *fileplaypage;
    stPlayList *fileplayactive;


//    SDL_Rect contentRect;
      TTF_Font *font;


    SDL_Texture *shuffle_icon;
    SDL_Texture *loop_icon;

    //up and down
    SDL_Texture *up_icon;
    SDL_Texture *down_icon;

    SDL_Texture *page_up_icon;
    SDL_Texture *page_down_icon;

    //File add and delete
    SDL_Texture *file_add_icon;
    SDL_Texture *delete_icon;

    SDL_Texture* fps_texture;

}settings_ui_t;

settings_ui_t* hcp_init_settings_ui(SDL_Renderer *renderer);


void cleanup_settings_ui(settings_ui_t *s_ui);

#endif
