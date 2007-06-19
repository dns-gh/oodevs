// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2004-04-22 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_pch.h $
// $Author: Ape $
// $Modtime: 22/02/05 10:18 $
// $Revision: 4 $
// $Workfile: AGR_pch.h $
//
// *****************************************************************************

#ifndef __AGR_pch_h_
#define __AGR_pch_h_

#include "MT/MT_Tools/MT_Tools_lib.h"
#include "MT/MT_Thread/MT_Thread_lib.h"
#include "MT/MT_IO/MT_IO_lib.h"
#include "MT/MT_Archive/MT_Archive_lib.h"
#include "MT/MT_Logger/MT_Logger_lib.h"
#include "MT/MT_Time/MT_Time_lib.h"
#include "MT/MT_XmlTools/MT_XmlTools_lib.h"

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
