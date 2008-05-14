// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created:  $
// $Archive: /MVW_v10/Build/SDK/TER/src/ter_pch.h $
// $Author: Jvt $
// $Modtime: 12/04/05 14:16 $
// $Revision: 3 $
// $Workfile: ter_pch.h $
//
// *****************************************************************************

#ifndef __TER_pch_h_
#define __TER_pch_h_

#include <string>
#include <vector>
#include <cassert>

#pragma warning( push )
#pragma warning( disable : 4511 4512 4100 4244 4996 )

#include <boost/pool/pool.hpp>
#undef min
#undef max

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/set.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>
#pragma warning( pop )

#include "TER.h"

#endif // __TER_pch_h_
