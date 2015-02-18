#include <gtk/gtk.h>
#include "ffmpeg.h"
#include "settings.h"
#include "ui.h"

GPid ff_pid = 0;

gpointer ffmpeg_thread(gpointer user_data)
{
    gchar c, buff[BUFF_LEN], *command, **argv;
    gint i, ff_stderr;
    GString* str;
    time_t tt;
    FILE* ff;

    tt = time(NULL);
    str = g_string_new("ffmpeg -f x11grab");
    g_string_append_printf(str, " -video_size %dx%d", conf.width, conf.height);
    g_string_append_printf(str, " -framerate %d", conf.fps);
    g_string_append_printf(str, " -draw_mouse %d", conf.mouse);
    g_string_append_printf(str, " -show_region %d", conf.frame);
    g_string_append_printf(str, " -i :0.0+%d,%d", conf.x, conf.y);
    g_string_append_printf(str, " -f pulse -i default -codec:a libvorbis -qscale:a %d", conf.audio_q);
    strftime(buff, sizeof(buff), " %Y%m%d-%H%M%S", localtime(&tt));
    g_string_append_printf(str, "%s.mkv -v error -stats", buff);
    command = g_string_free(str, FALSE);
#ifdef DEBUG
    printf("Command line: %s\n", command);
#endif
    g_shell_parse_argv(command, NULL, &argv, NULL);

    if(!g_spawn_async_with_pipes(g_get_home_dir(), argv, NULL, G_SPAWN_SEARCH_PATH, NULL, NULL, &ff_pid, NULL, NULL, &ff_stderr, NULL))
    {
        g_idle_add(ffmpeg_exit, command);
        g_strfreev(argv);
        return NULL;
    }

    g_free(command);
    g_strfreev(argv);

    ff = fdopen(ff_stderr, "r");
    i = 0;
    while(fread(&c, sizeof(gchar), 1, ff))
    {
        if(i == BUFF_LEN-1 || c < 32)
        {
            buff[i] = 0;
            i = 0;
            g_idle_add(ffmpeg_info, g_strdup(buff));
            continue;
        }
        buff[i++] = c;
    }

    g_spawn_close_pid(ff_pid);
    ff_pid = 0;
    g_idle_add(ffmpeg_exit, NULL);
    return NULL;
}

gboolean ffmpeg_info(gpointer line)
{
    gtk_status_icon_set_tooltip(tray, (gchar*)line);
    g_free(line);
    return FALSE;
}

gboolean ffmpeg_exit(gpointer failed)
{
    if(failed)
    {
        ui_dialog(GTK_MESSAGE_ERROR, "Failed to spawn the ffmpeg using a following command:\n%s", failed);
        g_free(failed);
    }

    gtk_status_icon_set_from_icon_name(tray, ICON_IDLE);
    gtk_status_icon_set_tooltip(tray, APP_NAME " - click to record");
    state = FALSE;
    return FALSE;
}

void ffmpeg_stop()
{
    if(ff_pid)
    {
        kill(ff_pid, SIGINT);
    }
}
