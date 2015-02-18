#include <gtk/gtk.h>
#include "ui.h"
#include "settings.h"
#include "ffmpeg.h"
#include "xwininfo.h"

void ui_init()
{
    tray = gtk_status_icon_new_from_icon_name(ICON_IDLE);
    gtk_status_icon_set_title(tray, APP_NAME);
    gtk_status_icon_set_tooltip(tray, APP_NAME " - click to record");
    g_signal_connect(tray, "activate", G_CALLBACK(ui_record), NULL);
    g_signal_connect(tray, "popup-menu", G_CALLBACK(ui_menu_popup), ui_menu_init());
    state = FALSE;
}

GtkWidget* ui_menu_init()
{
    GtkWidget* menu = gtk_menu_new();
    GtkWidget* menu_picker = gtk_image_menu_item_new_with_label("Select window");
    gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(menu_picker), GTK_WIDGET(gtk_image_new_from_icon_name(ICON_PICK, GTK_ICON_SIZE_MENU)));
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_picker);
    g_signal_connect(menu_picker, "activate", G_CALLBACK(ui_select_window), NULL);

    GtkWidget* menu_decorations = gtk_check_menu_item_new_with_label("Select decorations");
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_decorations);
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menu_decorations), conf.decorations);
    g_signal_connect(menu_decorations, "toggled", G_CALLBACK(settings_update_menuitem), &conf.decorations);

    GtkWidget* menu_mouse = gtk_check_menu_item_new_with_label("Record mouse pointer");
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_mouse);
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menu_mouse), conf.mouse);
    g_signal_connect(menu_mouse, "toggled", G_CALLBACK(settings_update_menuitem), &conf.mouse);

    GtkWidget* menu_frame = gtk_check_menu_item_new_with_label("Show recording frame");
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_frame);
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menu_frame), conf.frame);
    g_signal_connect(menu_frame, "toggled", G_CALLBACK(settings_update_menuitem), &conf.frame);

    GtkWidget* menu_exit = gtk_image_menu_item_new_with_label("Exit");
    gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(menu_exit), GTK_WIDGET(gtk_image_new_from_icon_name(ICON_EXIT, GTK_ICON_SIZE_MENU)));
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_exit);
    g_signal_connect(menu_exit, "activate", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(menu);
    return menu;
}

void ui_menu_popup(GtkStatusIcon *status_icon, guint button, guint activate_time, GtkMenu* menu)
{
    if(!state)
    {
        gtk_menu_popup(menu, NULL, NULL, gtk_status_icon_position_menu, status_icon, button, activate_time);
    }
}

void ui_record(GtkStatusIcon *status_icon, gpointer user_data)
{
    /* Start recording */
    if(!state)
    {
        if(!conf.width || !conf.height)
        {
            ui_dialog(GTK_MESSAGE_ERROR,
                      "Please choose a window to record.\nCurrent selection: x=%d, y=%d, w=%d, h=%d.",
                      conf.x, conf.y, conf.width, conf.height);
            return;
        }

        state = TRUE;
        g_thread_unref(g_thread_new("ffmpeg_thread", ffmpeg_thread, NULL));
        gtk_status_icon_set_from_icon_name(status_icon, ICON_ACTIVE);
    }
    /* Stop recording */
    else
    {
        ffmpeg_stop();
    }
}

void ui_select_window(GtkMenuItem *menu_item, gpointer user_data)
{
    gtk_status_icon_set_from_icon_name(tray, ICON_PICK);
    /* Timeout workaround (click at the tray icon is also caught by xwininfo?!) */
    g_timeout_add(250, xwininfo, NULL);
}

void ui_dialog(GtkMessageType type, gchar* format, ...)
{
    GtkWidget* dialog;
    va_list args;
    gchar *msg;

    va_start(args, format);
    msg = g_strdup_vprintf(format, args);
    va_end(args);
    dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, type, GTK_BUTTONS_CLOSE, msg);
    gtk_window_set_title(GTK_WINDOW(dialog), APP_NAME);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    g_free(msg);
}
