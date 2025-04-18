#ifndef GUICOMMON_H_
#define GUICOMMON_H_

#include <SDL.h>
#include <SDL_ttf.h>
#include <stdbool.h>
#include "cmdutils.h"

//#include "hcp_setting.h"
#include "playlinklist.h"

#define MIN_BUTTONS_WIDTH 8
#define MIN_BUTTONS_HEIGHT MIN_BUTTONS_WIDTH

#define GAP_BETWEEN_BUTTONS 2

#define SETTINGS_ICON_X 97
#define SETTINGS_ICON_Y 6
#define SETTINGS_ICON_WIDTH (0.018)
#define SETTINGS_ICON_HIEGHT (0.029)

#define PLUS_ICON_X (SETTINGS_ICON_X - GAP_BETWEEN_BUTTONS)
#define PLUS_ICON_Y (SETTINGS_ICON_Y)
#define PLUS_ICON_WIDTH (0.015)
#define PLUS_ICON_HEIGHT (0.015)

#define VOLUME_ICON_X (PLUS_ICON_X - GAP_BETWEEN_BUTTONS)
#define VOLUME_ICON_Y SETTINGS_ICON_Y
#define VOLUME_ICON_WIDTH (SETTINGS_ICON_WIDTH)
#define VOLUME_ICON_HEIGHT (SETTINGS_ICON_HIEGHT)

#define MINUS_ICON_X (VOLUME_ICON_X - GAP_BETWEEN_BUTTONS)
#define MINUS_ICON_Y (PLUS_ICON_Y)
#define MINUS_ICON_WIDTH PLUS_ICON_WIDTH
#define MINUS_ICON_HEIGHT PLUS_ICON_HEIGHT

#define PLAY_CONTROL_BG_X 32
#define PLAY_CONTROL_BG_Y 83
#define PLAY_CONTROL_BG_WIDTH  (PLAY_CONTROL_BG_X*0.012)
#define PLAY_CONTROL_BG_HEIGHT (PLAY_CONTROL_BG_WIDTH*0.22)


#define ZOOM_ICON_X (PLAY_CONTROL_BG_X+(PLAY_CONTROL_BG_WIDTH*95))
#define ZOOM_ICON_Y (PLAY_CONTROL_BG_Y+(PLAY_CONTROL_BG_HEIGHT*30))


#define PROGRESS_BAR_X (PLAY_CONTROL_BG_X+PLAY_CONTROL_BG_WIDTH*10)
#define PROGRESS_BAR_Y (ZOOM_ICON_Y+1)
#define PROGRESS_BAR_WIDTH (PLAY_CONTROL_BG_WIDTH - (PLAY_CONTROL_BG_WIDTH*0.15))
#define PROGRESS_BAR_HEIGHT (0.01)

#define CENTER_POSITION 50
#define PLAY_ICON_X (PLAY_CONTROL_BG_X+PLAY_CONTROL_BG_WIDTH*3)
#define PLAY_ICON_Y ZOOM_ICON_Y

#define BACKWARD_ICON_X (PLAY_ICON_X + (PROGRESS_BAR_WIDTH*100)/2)
#define BACKWARD_ICON_Y (PROGRESS_BAR_Y+2)

#define FORWARD_ICON_X (BACKWARD_ICON_X+3)
#define FORWARD_ICON_Y BACKWARD_ICON_Y

#define TOTAL_DURATION_X (ZOOM_ICON_X-1.2*GAP_BETWEEN_BUTTONS)
#define TOTAL_DURATION_Y BACKWARD_ICON_Y

#define CURRENT_DURATION_X (PROGRESS_BAR_X)
#define CURRENT_DURATION_Y BACKWARD_ICON_Y

#define LOGO_ICON_X 93
#define LOGO_ICON_Y 90

#define FPS_HEIGHT 5
#define FPS_WIDTH 2

#define EQ_X 70
#define EQ_Y 10
#define EQ_WIDTH 80
#define EQ_HEIGHT 60


typedef struct {
    SDL_Rect rect;
    SDL_Color color;
    const char *label;
    SDL_Texture *texture;

} Button;

typedef struct {
    SDL_Rect rect;
    const char *label;
    SDL_Texture *texture;
    bool checked;
} Checkbox;
#define PALLETE_COUNT 5
typedef struct {
    SDL_Rect rect[PALLETE_COUNT];
    SDL_Color color[PALLETE_COUNT];
    const char *label;
    SDL_Texture *texture;
    int selected;
} Pallete;
typedef struct {
    SDL_Rect rect;
    const char *label;
    SDL_Texture *texture;
    bool disable;
} PlayListButtons;


typedef struct {
    SDL_Rect plus;
    SDL_Rect minus;
    SDL_Color color;
    const char *label;
    SDL_Texture *ltexture;
    int value;
    SDL_Texture *vtexture;
} Spin;


typedef struct {
    SDL_Rect bar;
    SDL_Rect knob;

    const char *label;
    SDL_Texture *texture;
    const char *rlabel;
    SDL_Texture *rtexture;
    const char *llabel;
    SDL_Texture *ltexture;
    SDL_Texture *ctexture;
    float min;
    float max;
    float cur;
} Slider;

typedef struct _progressbar
{
    SDL_Color bg_color;
    SDL_Rect bar_bg;
    SDL_Rect bar_bg_duplicate;
    SDL_Rect bar;
    SDL_Color bar_color;
    //bool enable_knob;
  //  SDL_Rect knob;
    int radius;
    SDL_Surface *surface;
    SDL_Texture *texture;
    double mvduration;
    float cur;

    SDL_Surface *playCtrlSurface;
    SDL_Texture *playCtrlTexture;
    SDL_Rect bgrect;
}ProgressBar;

typedef struct _ui_buttons
{
    // Volume
    SDL_Rect volume_button;
    SDL_Texture *volume_icon;
    SDL_Texture *mute_icon;
    int muted;

    //play/pause
    SDL_Rect play_pause_button;
    SDL_Texture *play_icon;
    SDL_Texture *pause_icon;
    int showplayIconOnCenter;
    unsigned int delayTimer;
    //int paused;

    //zoom out
    SDL_Rect zoomout_button;
    SDL_Texture *zoomout_icon;
    SDL_Texture *zoomin_icon;
    int zoomout;

    //Settings
    SDL_Rect settings_button;
    SDL_Texture *settings_icon;

    //Plus
    SDL_Rect plus_button;
    SDL_Texture *plus_icon;

    //Minus
    SDL_Rect minus_button;
    SDL_Texture *minus_icon;
    //Forward
    SDL_Rect forward_button;
    SDL_Texture *forward_icon;

    //Backword
    SDL_Rect backword_button;
    SDL_Texture *backward_icon;

    ProgressBar progressbar;
    SDL_Rect playcontrol_bg;

    //SDL_Rect fps;
    SDL_Texture *fps_texture;
    double fps;

    TTF_Font* fontPlayer;
    TTF_Font* font;
    SDL_Rect total_duration;
    SDL_Texture *total_duration_texture;
    SDL_Rect currect_duration;
    SDL_Texture *current_duration_texture;

    //Eq
    SDL_Rect eq_bg;

    //SDL_Rect volume_rect ;

    SDL_Rect progress_knob;

    //logo
    SDL_Rect logo;
    SDL_Texture *logo_icon;

    SDL_Texture* duration_texture;
    SDL_Texture* curPos_texture;
    SDL_Texture* titletexture;
    SDL_Texture* volumetexture;


}ui_buttons_s;

ui_buttons_s * hcp_init_ui_icons(SDL_Renderer *renderer);


int hcp_clickedOnButton(SDL_Event *event, SDL_Rect *button);

#endif
