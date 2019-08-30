#include "IRenderable.h"

ecc::IRenderable::~IRenderable()
{
	if (m_image)
		m_image.reset();

	for (auto& image : m_images) {
		image.reset();
	}
}