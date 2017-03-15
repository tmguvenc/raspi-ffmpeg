#ifndef DECODER_H
#define DECODER_H

#using <System.Windows.Forms.dll>  
#using <System.Drawing.dll>

#include <memory>
#include <tbb/concurrent_queue.h>
#include <vcclr.h>

class Connector;
class Data;
class Decoder;
class IReceiveStrategy;

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
		void readSensor();

		void moveRight();
		void moveUp();
		void moveDown();
		void moveLeft();
		void motorStop();

		event System::Action<float, float>^ OnSensorDataReceived;

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
		tbb::concurrent_bounded_queue<Data*>* m_frame_queue;
		volatile bool m_started;
		volatile bool m_initialized;
		Connector* m_connector;
		IReceiveStrategy* m_receiveStrategy;
		System::Threading::Thread^ m_receiver_thread;
		System::Threading::Thread^ m_decoder_thread;
		System::Windows::Forms::Control^ m_control;
		System::Drawing::Graphics^ m_graphics;
		System::Drawing::Bitmap^ m_bmp;
	};
}

#endif