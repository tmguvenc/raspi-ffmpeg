#ifndef DECODER_H
#define DECODER_H

#using <System.Windows.Forms.dll>  
#using <System.Drawing.dll>

#include <zmq.h>
#include <memory>
#include <tbb/concurrent_queue.h>
#include <string>
#include <vcclr.h>
#include <decoder.h>

extern "C"
{
#include "./libavformat/avformat.h"
#include "./libavcodec/avcodec.h"
#include "./libavdevice/avdevice.h"
#include "./libswscale/swscale.h"
#include "./libavutil/imgutils.h"
}

namespace Client
{
	class Frame
	{
	public:
		Frame() : m_size(0), m_data(nullptr){}

		Frame(void* data, size_t size, size_t index) : m_size(size), m_index(index)
		{
			m_data = malloc(size);
			memcpy(m_data, data, size);
		}
		~Frame()
		{
			if (m_data)
			{
				free(m_data);
				m_data = nullptr;
				m_size = 0;
			}
		}

		size_t m_size;
		size_t m_index;
		void* m_data;

	private:
		Frame& operator=(const Frame& other);
		Frame(const Frame& other);
	};

	typedef std::shared_ptr<Frame> spFrame;

	public enum class CodecType {
		MJPEG,
		H264,
		RAW
	};

	public ref class RaspiClient {

	public:
		RaspiClient(System::Windows::Forms::Control^ control, System::String^ serverUrl, System::Int32 destWidth, System::Int32 destHeight, CodecType codec);
		~RaspiClient();

		void start();
		void stop();
		static void initialize();

	protected:
		std::string ManagedtoNativeString(System::String^ input) {
			pin_ptr<const wchar_t> wch = PtrToStringChars(input);
			std::wstring wInput = wch;
			return std::string(wInput.begin(), wInput.end());
		}

		void receive_loop();
		void decode_loop();

	private:
		System::Int32 m_destWidth;
		System::Int32 m_destHeight;
		Decoder* m_decoder;
		void* m_context;
		void* m_socket;
		std::string *m_url;
		tbb::concurrent_bounded_queue<spFrame>* m_frame_queue;
		volatile bool m_started;
		volatile bool m_initialized;

		System::Threading::Thread^ m_receiver_thread;
		System::Threading::Thread^ m_decoder_thread;
		System::Windows::Forms::Control^ m_control;
		System::Drawing::Graphics^ m_graphics;
		System::Drawing::Bitmap^ m_bmp;
	};
}

#endif