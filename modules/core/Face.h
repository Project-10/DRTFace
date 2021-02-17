#pragma once

#include "types.h"

namespace drtf {
	class CFace {
	public:
		/**
		 * @brief Constructor
		 */
		DllExport CFace(void) : CFace(Rect()) {}
		/**
		 * @brief Constructor
		 */
		DllExport CFace(const Rect& area, int id = -1, const std::string& text = "unknown")
			: m_area(area)
			, m_id(id)
			, m_text(text)
		{}
		DllExport ~CFace(void) = default;

		// Setters
		/**
		 * @brief Sets face id
		 */
		DllExport void setID(int id) { m_id = id; }
		/**
		 * @brief Sets face name
		 */
		DllExport void setText(const std::string& text) { m_text = text; }
		/**
		 * @brief Sets bounding rectangle for the face
		 */
		DllExport void setArea(const Rect& area) { m_area = area; }

		// Getters
		/**
		 * @brief Gets face id
		 */
		DllExport int			getID(void) const { return m_id; }
		/**
		 * @brief Gets face name
		 */
		DllExport std::string	getText(void) const { return m_text; }
		/**
		 * @brief Gets the face bounding rectangle
		 */
		DllExport Rect			getArea(void) const { return m_area; }


	private:
		Rect			m_area;
		int 			m_id;
		std::string		m_text;
	};

	// Pointer
	using ptr_face_t = std::shared_ptr<CFace>;
}
