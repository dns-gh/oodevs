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

#pragma warning( disable: 4786 ) // identifier was truncated to '255' characters in the debug information
#pragma warning( disable: 4355 ) // 'this' : used in base member initializer list
#pragma warning( disable: 4503 ) // decorated name length exceeded, name was truncated

#include <tools/Exception.h>
#include <tools/Map.h>
#include <xeumeuleu/xml.hpp>

// find a way to remove them
#include <protocol/Protocol.h>
#include <tools/Extendable.h>
#include <tools/InterfaceContainer.h>
#include <tools/Resolver.h>
#include "MIL.h"

#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>
#include <boost/preprocessor.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/set.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/type_traits.hpp>

#pragma warning( push, 0 )
#include <google/protobuf/stubs/common.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/generated_message_reflection.h>
#pragma warning( pop )

#include <cassert>
#include <functional>
#include <list>
#include <map>
#include <set>
#include <vector>

#endif // __MIL_pch_h_
