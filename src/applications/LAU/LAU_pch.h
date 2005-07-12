//*****************************************************************************
// Name   : LAU_pch.h
// Created: AML 02-01-24
//*****************************************************************************

#ifndef __LAU_PCH_
#define __LAU_PCH_

#pragma   warning(disable:4512) // disable warning C4512: 'class' : assignment operator could not be generated


#include "MT/MT_Archive/MT_Archive_lib.h"
#include "MT/MT_Tools/MT_Tools_lib.h"
#include "MT/MT_IO/MT_IO_lib.h"
#include "MT/MT_Logger/MT_Logger_lib.h"
////#include "MT/MT_Thread/Mt_Thread_lib.h"
#include "MT/MT_Tools/MT_Types.h"
#include "MT/MT_XmlTools/MT_XmlTools_lib.h"


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
////#include <qgl.h>

////#include "GFX_Color.h"
////#include "GFX_Tools.h"

#include "MT_Tools/MT_ScipioException.h"

#undef GetCurrentTime

// on redefini un system de log a nous car sinon ca marche pas bien appellé depuis un service pquoi ? ben je sais pas!!!!
const char* MT_GetApplicationPath( const char* pFilenameToConcatene = NULL );
void OutputLogDebug( const char* szMsg, const char* szSourceFilename = NULL, const int nNumLine = -1 );

////#ifdef LAU_IS_A_SERVICE
////
////    #undef MT_LOG_INFO_MSG
////    #define MT_LOG_INFO_MSG(x) OutputLogDebug( x, __FILE__, __LINE__ );
////
////    #undef MT_LOG_ERROR_MSG
////    #define MT_LOG_ERROR_MSG(x) OutputLogDebug( x, __FILE__, __LINE__ );
////
////#endif // LAU_IS_A_SERVICE


const char* SystemGetErrorMessage( const int nNumSystemError );

#endif //__LAU_PCH_
