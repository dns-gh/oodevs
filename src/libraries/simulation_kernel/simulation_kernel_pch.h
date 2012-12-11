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

#pragma warning( disable: 4996 ) // 'function': was declared deprecated
#pragma warning( disable: 4786 ) // identifier was truncated to '255' characters in the debug information
#pragma warning( disable: 4355 ) // 'this' : used in base member initializer list

#include <boost/serialization/extended_type_info.hpp> // $$$$ MCO : see http://old.nabble.com/-Serialization--extended_type_info.hpp-to27448440.html
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
#include <xeumeuleu/xml.hpp>

#include <vector>
#include <list>
#include <map>
#include <set>

#include <tools/Exception.h>

//$$$ DEGUEU
#include "MT_Tools/MT_Assert.h"

#endif // __MIL_pch_h_
