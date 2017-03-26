#ifndef CAPTURE_UTILS_H
#define CAPTURE_UTILS_H

extern "C" {
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <libavformat/avformat.h>
}

#include <cctype>
#include <sys/stat.h>

#ifdef _WIN32
	#include <direct.h>
#endif

inline bool dir_exists(const char* pathname)
{
	struct stat info;
	auto ret = stat(pathname, &info);
	return ret == 0 && (info.st_mode & S_IFDIR) == 0;
}

inline void create_if_not_exist(const char* path)
{
	if (!dir_exists(path))
	{
#if defined(_WIN32) && defined(_MSC_VER)
		_mkdir(path);
#else
		mkdir(path, 0777);
#endif
	}
}

static inline AVFrame* allocate_frame()
{
#if defined(_WIN32) && defined(_MSC_VER)
	return avcodec_alloc_frame();
#else
	return av_frame_alloc();
#endif
}

static inline void free_frame(AVFrame** frame)
{
#if defined(_WIN32) && defined(_MSC_VER)
	avcodec_free_frame(frame);
#else
	av_frame_free(frame);
#endif
}

static inline void free_packet(AVPacket* packet)
{
#if defined(_WIN32) && defined(_MSC_VER)
	av_free_packet(packet);
#else
	av_packet_unref(packet);
#endif
}

static inline void free_codec_context(AVCodecContext* context)
{
#if defined(_WIN32) && defined(_MSC_VER)
#else
	avcodec_free_context(&context);
#endif
}

static inline AVInputFormat* find_input_format(bool video = true)
{
#if defined(_WIN32) && defined(_MSC_VER)
	return av_find_input_format("dshow");
#else
	if(video)
		return av_find_input_format("video4linux2");
	else
		return av_find_input_format("alsa");
#endif
}

#endif