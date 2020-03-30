#pragma once

#include "HalconCpp.h"

using namespace HalconCpp;

void disp_message(HTuple hv_WindowHandle, HTuple hv_String, HTuple hv_CoordSystem,
	HTuple hv_Row, HTuple hv_Column, HTuple hv_Color, HTuple hv_Box);


// Procedure declarations 
// External procedures 
// Chapter: Graphics / Text
// Short Description: Set font independent of OS 
void set_display_font(HTuple hv_WindowHandle, HTuple hv_Size, HTuple hv_Font, HTuple hv_Bold, HTuple hv_Slant);