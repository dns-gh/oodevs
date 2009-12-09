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

#include <boost/mpl/and.hpp> // $$$$ MCO : see http://svn.boost.org/trac/boost/ticket/1782
#include <boost/serialization/serialization.hpp>

#include <boost/serialization/map.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/set.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/export.hpp>

#include <boost/utility.hpp>

#pragma warning( pop )

#include "MT/MT_Tools/MT_Tools_lib.h"
#include "MT/MT_Thread/MT_Thread_lib.h"
#include "MT/MT_IO/MT_IO_lib.h"
#include "MT/MT_Archive/MT_Archive_lib.h"
#include "MT/MT_Logger/MT_Logger_lib.h"
#include "MT/MT_Time/MT_Time_lib.h"
//#include "MT/MT_XmlTools/MT_XmlTools_lib.h"

#include "MT_Tools/MT_Assert.h"
#include "MT_Tools/MT_Scipio_enum.h"
#include "MT_Tools/MT_Vector2D.h"
#include "MT_Tools/MT_Vector3D.h"
#include "MT_Tools/MT_Triangle.h" 
#include "MT_Tools/MT_Line.h"
#include "MT_Tools/MT_Circle.h"
#include "MT_Tools/MT_ScipioException.h"
#include "MT_Tools/MT_Random.h"
#include "MT_Tools/MT_GaussianRandom.h"

#include "game_asn/Simulation.h"

#include "simulation_terrain/TER.h"
#include "simulation_terrain/TER_Localisation.h"
#include "simulation_terrain/TER_World.h"
#include "simulation_terrain/TER_PathFindRequest_ABC.h"
#include "simulation_terrain/TER_Agent_ABC.h"
#include "simulation_terrain/TER_Object_ABC.h"
#include "simulation_terrain/TER_AgentManager.h"
#include "simulation_terrain/TER_ObjectManager.h"
#include "simulation_terrain/TER_Polygon.h"

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


