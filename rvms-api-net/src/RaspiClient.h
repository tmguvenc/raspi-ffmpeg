#ifndef DECODER_H
#define DECODER_H

#using <System.Windows.Forms.dll>  
#using <System.Drawing.dll>

#include <zmq.h>
#include <memory>
#include <tbb/concurrent_queue.h>
#include <vcclr.h>
#include <decoder.h>
#include "video_frame.h"

extern "C"
{
#include "./libavformat/avformat.h"
#include "./libavcodec/avcodec.h"
#include "./libavdevice/avdevice.h"
#include "./libswscale/swscale.h"
#include "./libavutil/imgutils.h"
}

class Connector;

namespace Client
{
	public ref class RaspiClient {

	public:
		RaspiClient(System::Windows::Forms::Control^ control, System::String^ ip, System::UInt16 port);
		~RaspiClient();
		!RaspiClient();

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
		tbb::concurrent_bounded_queue<spVideoFrame>* m_frame_queue;
		volatile bool m_started;
		volatile bool m_initialized;
		Connector* m_connector;
		System::Threading::Thread^ m_receiver_thread;
		System::Threading::Thread^ m_decoder_thread;
		System::Windows::Forms::Control^ m_control;
		System::Drawing::Graphics^ m_graphics;
		System::Drawing::Bitmap^ m_bmp;
	};
}

#endif