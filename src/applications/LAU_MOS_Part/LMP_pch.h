//*****************************************************************************
// Name   : LMP_pch.h
// Created: AML 02-01-24
//*****************************************************************************

#ifndef __LMP_PCH_
#define __LMP_PCH_

#pragma   warning(disable:4512) // disable warning C4512: 'class' : assignment operator could not be generated

#include "LMP.h"

#include "MT/MT_Archive/MT_Archive_lib.h"
#include "MT/MT_Tools/MT_Tools_lib.h"
#include "MT/MT_IO/MT_IO_lib.h"
#include "MT/MT_Logger/MT_Logger_lib.h"
////#include "MT/MT_Thread/Mt_Thread_lib.h"
#include "MT/MT_Tools/MT_Types.h"

#include "MT_Tools/MT_Tools_Types.h"
////#include "MT_Scipio_enum.h"
////#include "MT_Vector2D.h"
////#include "MT_Vector3D.h"
////#include "MT_Rect.h"
////#include "MT_Line.h"
////#include "MT_Triangle.h"

#include "DIN/MessageService/DIN_MessageServiceUserCbk.h"
#include "DIN/ConnectionService/DIN_ConnectionServiceServerUserCbk.h"
#include "DIN/ConnectionService/DIN_ConnectionServiceClientUserCbk.h"

////#include "DIA_Tool_Script_lib.h"
////#include "DIA_Kernel_lib.h"
////#include "DIA_ToolManager_lib.h"

#include <winsock2.h>
#include <qgl.h>

//#include "GFX/GFX_Color.h"
//#include "GFX/GFX_Tools.h"

#include "MT_Tools/MT_ScipioException.h"

#undef GetCurrentTime

#define LMP_DEFAULT_NUM_PORT            4321  
#define LMP_DEFAULT_MAGIC_CONNECTION    3232

#endif //__LMP_PCH_
