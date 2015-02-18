#include <gtk/gtk.h>
#include <string.h>
#include <ctype.h>
#include "ui.h"
#include "settings.h"
#include "xwininfo.h"

const gchar* find_strings[] =
{
    "Absolute upper-left X:",
    "Absolute upper-left Y:",
    "Width:",
    "Height:"
};

enum
{
    XWININFO_X,
    XWININFO_Y,
    XWININFO_WIDTH,
    XWININFO_HEIGHT,
    XWININFO__END
};

gboolean xwininfo()
{
    FILE *xwin;
    gchar buff[1024], *ptr;
    gint i;

    if(conf.decorations)
    {
        xwin = popen("xwininfo -frame", "r");
    }
    else
    {
        xwin = popen("xwininfo", "r");
    }

    if(!xwin)
    {
        ui_dialog(GTK_MESSAGE_ERROR, "Failed to spawn the xwininfo process.");
        gtk_status_icon_set_from_icon_name(tray, ICON_IDLE);
        return FALSE;
    }

    while(fgets(buff, sizeof(buff), xwin))
    {
        for(i=0; i<XWININFO__END; i++)
        {
            if((ptr = strstr(buff, find_strings[i])))
            {
                ptr += strlen(find_strings[i]);
                while(isspace(*ptr))
                    ptr++;
                switch(i)
                {
                case XWININFO_X:
                    sscanf(ptr, "%u", &conf.x);
                    break;
                case XWININFO_Y:
                    sscanf(ptr, "%u", &conf.y);
                    break;
                case XWININFO_WIDTH:
                    sscanf(ptr, "%u", &conf.width);
                    break;
                case XWININFO_HEIGHT:
                    sscanf(ptr, "%u", &conf.height);
                    break;
                }
            }
        }
    }
#ifdef DEBUG
    printf("New position: x=%d, y=%d, width=%d, height=%d.\n", conf.x, conf.y, conf.width, conf.height);
#endif
    pclose(xwin);

    selection_frame();
    settings_write();
    gtk_status_icon_set_from_icon_name(tray, ICON_IDLE);
    return FALSE;
}

void selection_frame()
{
    GtkWidget *frame;
    GdkPixmap *pixmap;
    cairo_t *cr;
    GdkColor color;

    frame = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_type_hint(GTK_WINDOW(frame), GDK_WINDOW_TYPE_HINT_DOCK);
    gtk_window_set_title(GTK_WINDOW(frame), "FFmpeg recording area");
    gtk_window_set_resizable(GTK_WINDOW(frame), FALSE);
    gtk_window_stick(GTK_WINDOW(frame));
    gtk_window_set_keep_above(GTK_WINDOW(frame), TRUE);
    gtk_window_move(GTK_WINDOW(frame), conf.x-1, conf.y-1);
    gtk_widget_set_size_request(GTK_WIDGET(frame), conf.width+2, conf.height+2);
    gdk_color_parse("red", &color);
    gtk_widget_modify_bg(frame, GTK_STATE_NORMAL, &color);
    gtk_widget_realize(frame);

    pixmap = gdk_pixmap_new(NULL, conf.width+2, conf.height+2, 1);
    cr = gdk_cairo_create(pixmap);

    cairo_save(cr);
    cairo_set_source_rgba(cr, 1.0, 1.0, 1.0, 0.0);
    cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
    cairo_paint(cr);
    cairo_restore(cr);

    cairo_set_source_rgba(cr, 0.0, 0.0, 0.0, 1.0);
    cairo_set_line_width(cr, 1);
    cairo_rectangle(cr, 0.0, 0.0, conf.width+2.0, conf.height+2.0);
    cairo_stroke(cr);

    gdk_window_shape_combine_mask(gtk_widget_get_window(frame), pixmap, 0, 0);
    gtk_widget_show_all(frame);
    cairo_destroy(cr);
    g_object_unref(pixmap);
    g_timeout_add(1000, selection_frame_destroy, frame);
}

gboolean selection_frame_destroy(gpointer ptr)
{
    gtk_widget_destroy(GTK_WIDGET(ptr));
    return FALSE;
}
