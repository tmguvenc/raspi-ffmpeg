#include <audio_encoder.h>
#include <stdexcept>
#include <capture_utils.h>
#include <iostream>
#include <assert.h>

extern "C"{
#include <libavdevice/avdevice.h>
#include <libavcodec/avcodec.h>
#include <libavutil/samplefmt.h>
#include <libavutil/common.h>
}

static int check_sample_fmt(const AVCodec *codec, enum AVSampleFormat sample_fmt)
{
	auto p = codec->sample_fmts;
	while (*p != AV_SAMPLE_FMT_NONE) {
		if (*p == sample_fmt)
			return 1;
		p++;
	}
	return 0;
}

/* select layout with the highest channel count */
static int select_channel_layout(AVCodec *codec)
{
	const uint64_t *p;
	uint64_t best_ch_layout = 0;
	int best_nb_channels = 0;

	if (!codec->channel_layouts)
		return AV_CH_LAYOUT_STEREO;

	p = codec->channel_layouts;
	while (*p) {
		int nb_channels = av_get_channel_layout_nb_channels(*p);

		if (nb_channels > best_nb_channels) {
			best_ch_layout = *p;
			best_nb_channels = nb_channels;
		}
		p++;
	}
	return best_ch_layout;
}

/* just pick the highest supported samplerate */
static int select_sample_rate(AVCodec *codec)
{
	const int *p;
	int best_samplerate = 0;

	if (!codec->supported_samplerates)
		return 44100;

	p = codec->supported_samplerates;
	while (*p) {
		best_samplerate = FFMAX(*p, best_samplerate);
		p++;
	}
	return best_samplerate;
}

AudioEncoder::AudioEncoder()
{
	av_register_all();
	avdevice_register_all();
	avformat_network_init();
	avcodec_register_all();

	m_pCodec = avcodec_find_encoder(CODEC_ID_MP2);
	if (!m_pCodec)
		throw std::invalid_argument("cannot find codec");

	m_pCodecContext = avcodec_alloc_context3(m_pCodec);

	m_pCodecContext->bit_rate = 64000;
	m_pCodecContext->sample_fmt = AV_SAMPLE_FMT_S16;
	
	if (!check_sample_fmt(m_pCodec, m_pCodecContext->sample_fmt))
		throw std::invalid_argument("invalid sample format");

	m_pCodecContext->sample_rate = select_sample_rate(m_pCodec);
	m_pCodecContext->channel_layout = select_channel_layout(m_pCodec);
	m_pCodecContext->channels = av_get_channel_layout_nb_channels(m_pCodecContext->channel_layout);

	if (avcodec_open2(m_pCodecContext, m_pCodec, nullptr) < 0)
		throw std::invalid_argument("cannot open codec");

	m_frame = allocate_frame();

	m_frame->nb_samples = m_pCodecContext->frame_size;
	m_frame->format = m_pCodecContext->sample_fmt;
	m_frame->channel_layout = m_pCodecContext->channel_layout;
	m_buffer_size = av_samples_get_buffer_size(nullptr, m_pCodecContext->channels, m_pCodecContext->frame_size, m_pCodecContext->sample_fmt, 0);
}

AudioEncoder::~AudioEncoder()
{
	av_freep(&m_frame->data[0]);
	free_frame(&m_frame);
	avcodec_close(m_pCodecContext);
	av_free(m_pCodecContext);
}

bool AudioEncoder::encode(void* inputData, int /*inputLen*/, void* data, int& len)
{
	AVPacket packet;
	av_init_packet(&packet);

	m_frame->data[0] = static_cast<unsigned char*>(inputData);

	int gotFrame;
	if (avcodec_encode_audio2(m_pCodecContext, &packet, m_frame, &gotFrame) >= 0) {
		if (gotFrame) {
			memcpy(data, packet.data, packet.size);
			len = packet.size;
			return true;
		}
	}
	av_free_packet(&packet);
	return false;
}