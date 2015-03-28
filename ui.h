#ifndef FFXCAST_UI_H_
#define FFXCAST_UI_H_

#define APP_NAME "ffxcast"
#define DEBUG 1

#define ICON_IDLE   "media-record-symbolic"
#define ICON_ACTIVE "media-record"
#define ICON_PICK   "gtk-color-picker"
#define ICON_EDIT   "gtk-edit"
#define ICON_EXIT   "gtk-quit"

void ui_init();
GtkWidget* ui_menu_init();
void ui_menu_popup(GtkStatusIcon*, guint, guint, GtkMenu*);
void ui_record(GtkStatusIcon*, gpointer);
void ui_select_window(GtkMenuItem*, gpointer);
void ui_dialog(GtkMessageType, gchar*, ...);
void ui_update_fps(GtkMenuItem*, guint);
void ui_change_fps(GtkMenuItem*, gpointer);

GtkStatusIcon* tray;
gboolean state;

#endif // FFXCAST_UI_H_
