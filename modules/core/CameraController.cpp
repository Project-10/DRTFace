#include "CameraController.h"

namespace drtf {
	void CCameraCantroller::init(void)
	{
		if (!m_camera.open(0)) {
			printf("Can't find a camera\n");
			exit(1);
		};
	}

	void CCameraCantroller::start(void)
	{
		m_terminate_producer = false;
		m_thr_producer = std::thread([&]() {
			Mat img;
			for (;;) {
				m_mtx_FrameBuffer.lock();
				m_camera >> m_vFrameBuffer[pointer_in % m_vFrameBuffer.size()];
				pointer_in++;
				m_mtx_FrameBuffer.unlock();
				m_inFrameCounter++;

				std::this_thread::sleep_for(std::chrono::milliseconds(1));

				if (m_terminate_producer) break;
			}
		});

		const double tickFrequency = getTickFrequency();
		m_terminate_counter = false;
		m_thr_counter = std::thread([&, tickFrequency]() {
			for (;;) {
				static int64 ticks_old	= getTickCount();
				int64 ticks_new 		= getTickCount();
				double sec = (ticks_new - ticks_old) / tickFrequency;
				ticks_old = ticks_new;
				
				static int64 in_fps_old = m_inFrameCounter;
				int64 in_fps_new 		= m_inFrameCounter;
				int64 in_fps = in_fps_new - in_fps_old;
				in_fps_old = in_fps_new;
				
				static int64 out_fps_old = m_outFrameCounter;
				int64 out_fps_new 		 = m_outFrameCounter;
				int64 out_fps = out_fps_new - out_fps_old;
				out_fps_old = out_fps_new;
				
				printf("FPS: in: %.2f | out: %.2f\n", in_fps / sec, out_fps / sec);

				std::this_thread::sleep_for(std::chrono::seconds(1));
				if (m_terminate_counter) break;
			}
		});
	}

	Mat CCameraCantroller::getFrame(void)
	{
		Mat res;
		if (pointer_out != pointer_in) {
		
			m_mtx_FrameBuffer.lock();
			if (!m_vFrameBuffer.empty()) {
				res = m_vFrameBuffer[pointer_out % m_vFrameBuffer.size()];
				pointer_out++;
			}
			m_mtx_FrameBuffer.unlock();
		}
		m_outFrameCounter++;
		return res;
	}

	void CCameraCantroller::stop(void)
	{
		m_terminate_producer = true;
		m_thr_producer.join();

		m_terminate_counter = true;
		m_thr_counter.join();
	}

}
