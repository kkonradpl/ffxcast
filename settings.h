#ifndef FFXCAST_SETTINGS_H_
#define FFXCAST_SETTINGS_H_

#define CONF_FILE APP_NAME ".conf"

#define SETTINGS_DEFAULT_DECORATIONS TRUE
#define SETTINGS_DEFAULT_FPS           15
#define SETTINGS_DEFAULT_FRAME       TRUE
#define SETTINGS_DEFAULT_MOUSE       TRUE
#define SETTINGS_DEFAULT_AUDIOQ         5

typedef struct
{
    guint x, y;
    guint width, height;
    guint fps;
    gboolean decorations;
    gboolean frame;
    gboolean mouse;
    gint audio_q;
} conf_t;

conf_t conf;

void settings_read();
void settings_write();
void settings_update_menuitem(GtkCheckMenuItem*, gpointer);
void settings_update_fps(guint);

#endif // FFXCAST_SETTINGS_H_
