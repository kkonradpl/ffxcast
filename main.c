/*
 *  ffxcast 2015-02-18
 *  Copyright (C) 2015  Konrad Kosmatka

 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 */

#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include "ui.h"
#include "settings.h"

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);
    settings_read();
    ui_init();
    gtk_main();
    return 0;
}
