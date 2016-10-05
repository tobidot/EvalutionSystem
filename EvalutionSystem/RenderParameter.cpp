#include "RenderParameter.h"

namespace render
{
	size_t RenderParameter::NEXT_RENDER_PARAMETER_ID = 1;
	const size_t RenderParameterID::TYPE_ID = RenderParameter::get_next_render_parameter_id();
	const size_t RenderParameterColor::TYPE_ID = RenderParameter::get_next_render_parameter_id();

}
