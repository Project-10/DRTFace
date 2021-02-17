#pragma once

#include "types.h"

class CCameraCantroller {
public:
	DllExport CCameraCantroller(size_t buf_len) : m_vFrameBuffer(buf_len) {}
	DllExport ~CCameraCantroller(void) = default;

	DllExport void	init(void);
	DllExport void	start(void);
	DllExport Mat	getFrame(void);
	DllExport void	stop(void);


private:
	VideoCapture		m_camera;
	std::deque<Mat> 	m_vFrameBuffer;
	std::mutex			m_mtx_FrameBuffer;

	size_t				m_inFrameCounter = 0;
	size_t				m_outFrameCounter = 0;
	size_t				pointer_in = 0;
	size_t				pointer_out = 0;
	
	// Producer
	std::thread m_thr_producer;
	bool		m_terminate_producer;
	
	// Counter
	std::thread m_thr_counter;
	bool		m_terminate_counter;


};
