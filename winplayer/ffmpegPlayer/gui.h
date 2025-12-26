
#ifndef GUI_H_
#define GUI_H_


#include <SDL.h>
#include <SDL_ttf.h>
#include <stdbool.h>
#include "hc_ffplay.h"
#include "playlinklist.h"

typedef struct
{
    SDL_Color subtilesFontCol;
    SDL_Point subtilesPos ;
    TTF_Font* fontSubtitles;
    int isfoundSubtiles;
    char foundSubtiles[512];

} st_ui_subtitles;







VideoState *stream_open(const char *filename, AVInputFormat *iformat);

void playlist(VideoState *cur_stream, char * dropped_file, AVInputFormat *file_iformat , SDL_Window *window );

void stream_close(VideoState *is);

void video_display(VideoState *is);

void step_to_next_frame(VideoState *is);

void minimize_clicked(void);

void close_clicked(void);

bool LeftMouseButtonDown(VideoState *cur_stream, SDL_Event *event, int seek_by_bytes, SDL_Window *window, char *file);

void update_volume(VideoState *is, int sign, double step);

void setSubTitlesFontSize(st_ui_subtitles *ui_subtitles, int ptsize);

void setSubTitlesPosition(st_ui_subtitles *ui_subtitles, SDL_Point subPos);

void setSubTitlesFontColor(st_ui_subtitles *ui_subtitles, SDL_Color subFonCol);

void setSubTitlesValue(st_ui_subtitles *ui_subtitles, int tab , int value );

void get_text_and_rect(SDL_Renderer *renderer, int x, int y, char *text,  TTF_Font *font, SDL_Texture **texture, SDL_Rect *rect,  SDL_Color textColor);

void getMovieDuration( const AVFormatContext *fmt_ctx, int index);

int64_t frame_queue_last_pos(FrameQueue *f);

void mouse_fn(VideoState *cur_stream, int seek_by_bytes, double incr );

void stream_seek(VideoState *is, int64_t pos, int64_t rel, int seek_by_bytes);

double get_master_clock(VideoState *is);

bool getFromPlaylist(char *line);

void toggle_full_screen(VideoState *is);

void toggle_pause(VideoState *is);


void resetPlaylist(VideoState *cur_stream);

void hcp_setting(SDL_Renderer *renderer, int width, int height, ui_buttons_s *ui_buttons, settings_ui_t *ui_settigs);

void hcp_setting_draw(SDL_Renderer *renderer, int width, int height, ui_buttons_s *ui_buttons, settings_ui_t *ui_settigs, stPlayList *list);

bool mouseMotion(VideoState *cur_stream, SDL_Event *event, SDL_Window *window);

bool mouseUp(VideoState *cur_stream, SDL_Event *event, SDL_Window *window);

void hcp_fill_rounded_box_b(SDL_Surface* dst, SDL_Rect *coords, int r, SDL_Color color);

void hcp_render_ui(SDL_Renderer *renderer,int paused, int width, int height, ui_buttons_s *v_ctx);

void drawVolume(SDL_Renderer *renderer,  ui_buttons_s *ui_buttons,  int vol);

bool getNextPlayList( char *path );

void renderSubtiles( Frame *sp);

void intSubtitiles(void);

bool checkDuration(VideoState *is);

void showplayIconOnCenter(int show);

void initUI( SDL_Renderer *renderer, int default_width, int default_height );

void renderGUI(VideoState *is, SDL_Renderer *renderer, int *cursor_hidden);

void set_audioSpeedFilter(VideoState *is,  char **af,  float *last_speed);

void set_videoFilter(VideoState *is, char **vf,  float *last_speed, float *last_contrast, float  *last_brightness, float  *last_saturation, bool *last_stereo3D);

void cleanPlaylist(void);

void cleanUI(void);

bool Stereo3d(VideoState *is);

#endif
