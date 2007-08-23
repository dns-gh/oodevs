// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AGR_pch_h_
#define __AGR_pch_h_

#include "MT/MT_Tools/MT_Tools_lib.h"
#include "MT/MT_Thread/MT_Thread_lib.h"
#include "MT/MT_IO/MT_IO_lib.h"
#include "MT/MT_Logger/MT_Logger_lib.h"
#include "MT/MT_Time/MT_Time_lib.h"

#include <xeumeuleu/xml.h>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

typedef std::set< std::string >        T_String_Set;
typedef T_String_Set::iterator        IT_String_Set;
typedef T_String_Set::const_iterator CIT_String_Set;

typedef std::vector< std::string >        T_String_Vector;
typedef T_String_Vector::iterator        IT_String_Vector;
typedef T_String_Vector::const_iterator CIT_String_Vector;

class AGR_Enumeration;

typedef std::vector< AGR_Enumeration* >      T_Enumeration_Vector;
typedef T_Enumeration_Vector::iterator       IT_Enumeration_Vector;
typedef T_Enumeration_Vector::const_iterator CIT_Enumeration_Vector;

#define AGR_SKEL_DIR "../../src/applications/AGR/skel/"

#endif // __AGR_pch_h_
