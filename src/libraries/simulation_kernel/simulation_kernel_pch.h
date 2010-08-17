//*****************************************************************************
//
// $Created: NLD 2002-07-17 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/MIL_pch.h $
// $Author: Jvt $
// $Modtime: 12/04/05 14:09 $
// $Revision: 11 $
// $Workfile: MIL_pch.h $
//
//*****************************************************************************

#ifndef __MIL_pch_h_
#define __MIL_pch_h_

#pragma warning( disable : 4996 ) // $$$$ SBO 2008-05-13: vc80 deprecation

#pragma warning( push )
#pragma warning( disable : 4702 )
#pragma warning( disable : 4511 4512 4100 4244 )

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/extended_type_info.hpp> // $$$$ MCO : see http://old.nabble.com/-Serialization--extended_type_info.hpp-to27448440.html
#include <boost/serialization/map.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/set.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>

#include <boost/noncopyable.hpp>

#pragma warning( pop )

#include <MT/MT_Logger/MT_Logger_lib.h>

#include "MT_Tools/MT_Assert.h"
#include "MT_Tools/MT_Scipio_enum.h"
#include "MT_Tools/MT_Vector2D.h"
#include "MT_Tools/MT_Vector3D.h"
#include "MT_Tools/MT_Triangle.h"
#include "MT_Tools/MT_Line.h"
#include "MT_Tools/MT_Circle.h"
#include "MT_Tools/MT_Ellipse.h"
#include "MT_Tools/MT_ScipioException.h"
#include "MIL_Random.h"

#include "CheckPoints/MIL_CheckPointSerializationHelpers.h" // $$$$ _RC_ SLI 2010-08-11: waiting for a better solution
#include "simulation_terrain/TER_Localisation.h"
#include "simulation_terrain/TER_World.h"

#pragma warning( push )
#pragma warning( disable : 4005 ) // $$$$ SBO 2008-05-13: "macro redefinition"
//$$$ DEGUEU
#ifdef  NDEBUG
#   define assert( exp ) ( static_cast< void >( exp ) )
#else
#   define assert( exp ) ( (exp) || MT_Assert( #exp, __FILE__, __LINE__ ) )
#endif  // NDEBUG
#pragma warning( pop )

#endif // __MIL_pch_h_
