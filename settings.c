#include <gtk/gtk.h>
#include "settings.h"
#include "ui.h"

void settings_read()
{
    GKeyFile *keyfile = g_key_file_new();
    GError *error = NULL;
    gchar* path = g_strdup_printf("%s/%s", g_get_user_config_dir(), CONF_FILE);
    if(!g_key_file_load_from_file(keyfile, path, G_KEY_FILE_KEEP_COMMENTS, &error))
    {
        g_key_file_free(keyfile);
        conf.x = conf.y = 0;
        conf.width = conf.height = 0;
        conf.decorations = SETTINGS_DEFAULT_DECORATIONS;

        conf.fps = SETTINGS_DEFAULT_FPS;
        conf.frame = SETTINGS_DEFAULT_FRAME;
        conf.mouse = SETTINGS_DEFAULT_MOUSE;
        conf.audio_q = SETTINGS_DEFAULT_AUDIOQ;

        settings_write();
        g_free(path);
        g_error_free(error);
        return;
    }
    g_free(path);
    conf.x = g_key_file_get_integer(keyfile, "position", "x", NULL);
    conf.y = g_key_file_get_integer(keyfile, "position", "y", NULL);
    conf.width = g_key_file_get_integer(keyfile, "position", "width", NULL);
    conf.height = g_key_file_get_integer(keyfile, "position", "height", NULL);
    conf.decorations = g_key_file_get_boolean(keyfile, "position", "decorations", NULL);

    conf.fps = g_key_file_get_integer(keyfile, "ffmpeg", "fps", NULL);
    conf.frame = g_key_file_get_boolean(keyfile, "ffmpeg", "frame", NULL);
    conf.mouse = g_key_file_get_boolean(keyfile, "ffmpeg", "mouse", NULL);
    conf.audio_q = g_key_file_get_integer(keyfile, "ffmpeg", "audio_q", NULL);

    if(!conf.fps)
    {
        conf.fps = SETTINGS_DEFAULT_FPS;
        settings_write();
    }

    g_key_file_free(keyfile);
}

void settings_write()
{
    GKeyFile *keyfile = g_key_file_new();
    g_key_file_set_integer(keyfile, "position", "x", conf.x);
    g_key_file_set_integer(keyfile, "position", "y", conf.y);
    g_key_file_set_integer(keyfile, "position", "width", conf.width);
    g_key_file_set_integer(keyfile, "position", "height", conf.height);
    g_key_file_set_boolean(keyfile, "position", "decorations", conf.decorations);

    g_key_file_set_integer(keyfile, "ffmpeg", "fps", conf.fps);
    g_key_file_set_boolean(keyfile, "ffmpeg", "frame", conf.frame);
    g_key_file_set_boolean(keyfile, "ffmpeg", "mouse", conf.mouse);
    g_key_file_set_integer(keyfile, "ffmpeg", "audio_q", conf.audio_q);

    gchar* path = g_strdup_printf("%s/%s", g_get_user_config_dir(), CONF_FILE);
    if(!g_key_file_save_to_file(keyfile, path, NULL))
        ui_dialog(GTK_MESSAGE_WARNING, "Unable to save the configuration file:\n%s", path);
    g_free(path);
    g_key_file_free(keyfile);
}

void settings_update_menuitem(GtkCheckMenuItem *menuitem, gpointer ptr)
{
    gboolean* conf = (gboolean*)ptr;
    *conf = gtk_check_menu_item_get_active(menuitem);
    settings_write();
}
