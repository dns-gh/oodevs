//*****************************************************************************
//
// This file is part of MASA DIN library.
//
// Refer to the included end-user license agreement (License.rtf) for
// restrictions.
//
// Copyright (c) 2002 Mathématiques Appliquées SA (MASA)
// All Rights Reserved. DIN is a trademark of MASA Corporation.
//
//*****************************************************************************
//
// $Created: NLD 2002-07-16 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_ODBTypeAutomate.cpp $
// $Author: Fbd $
// $Modtime: 28/07/03 18:47 $
// $Revision: 1 $
// $Workfile: MOS_ODBTypeAutomate.cpp $
//
//*****************************************************************************

#ifdef __GNUG__
#    pragma implementation
#endif

#include "MOS_Light_Pch.h"
#include "MOS_ODBTypeAutomate.h"

#ifndef MOS_USE_INLINE
#   include "MOS_ODBTypeAutomate.inl"
#endif

//-----------------------------------------------------------------------------
// Name: MOS_ODBTypeAutomate constructor
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
MOS_ODBTypeAutomate::MOS_ODBTypeAutomate( const std::string& sName, const std::string& sSymbol )
: sName_ ( sName )
, sSymbol_( sSymbol )
{
}


//-----------------------------------------------------------------------------
// Name: MOS_ODBTypeAutomate destructor
// Created: FBD 03-07-28
//-----------------------------------------------------------------------------
MOS_ODBTypeAutomate::~MOS_ODBTypeAutomate()
{
   
}

