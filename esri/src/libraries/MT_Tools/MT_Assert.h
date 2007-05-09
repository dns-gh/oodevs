//*****************************************************************************
//
// $Created: NLD 2003-07-21 $
// $Archive: /MVW_v10/Build/SDK/MT_Tools/Src/MT_Assert.h $
// $Author: Nld $
// $Modtime: 13/01/04 11:19 $
// $Revision: 4 $
// $Workfile: MT_Assert.h $
//
//*****************************************************************************

#ifndef __MT_Assert_h_
#define __MT_Assert_h_

#include <cassert>

#include "MT/MT_Tools/MT_Tools_lib.h"

//-----------------------------------------------------------------------------
// Name: MT_Assert
// Created: NLD 2003-07-21
//-----------------------------------------------------------------------------
inline
bool MT_Assert( const char* strExpression, const char* strFile, int nLine )
{
    std::stringstream strMsg;

    strMsg << "assert '"   << strExpression 
           << "' - File '" << strFile
           << "' - Line "  << nLine; 

    std::cerr << strMsg.str() << std::endl;
    _asm int 3;
    return true;
}


#undef assert

#ifdef  NDEBUG
#   define assert( exp ) ( static_cast< void >( exp ) )
#else
#   define assert( exp ) ( (exp) || MT_Assert( #exp, __FILE__, __LINE__ ) )
#endif  // NDEBUG

#endif // __MT_Assert_h_
