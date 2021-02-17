#pragma once

#include "types.h"

class CFace {
public:
	DllExport CFace(void) : CFace(Rect()) {}
	DllExport CFace(const Rect& area, int id = -1, const std::string& text = "unknown")
		: m_area(area)
		, m_id(id)
		, m_text(text)
	{}
	DllExport ~CFace(void) = default;

	// Setters
	DllExport void setID(int id) { m_id = id; }
	DllExport void setText(const std::string& text) { m_text = text; }
	DllExport void setArea(const Rect& area) { m_area = area; }

	// Getters
	DllExport int			getID(void) const { return m_id; }
	DllExport std::string	getText(void) const { return m_text; }
	DllExport Rect			getArea(void) const { return m_area; }


private:
	Rect			m_area;
	int 			m_id;
	std::string		m_text;
};

// Pointer
using ptr_face_t = std::shared_ptr<CFace>;
