#ifndef FFXCAST_FFMPEG_H_
#define FFXCAST_FFMPEG_H_

#define BUFF_LEN 1024

gpointer ffmpeg_thread(gpointer);
gboolean ffmpeg_info(gpointer);
gboolean ffmpeg_exit(gpointer);
void ffmpeg_stop();

#endif // FFXCAST_FFMPEG_H_
