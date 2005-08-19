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
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_ODBTypePion.cpp $
// $Author: Fbd $
// $Modtime: 29/07/03 9:39 $
// $Revision: 1 $
// $Workfile: MOS_ODBTypePion.cpp $
//
//*****************************************************************************

#ifdef __GNUG__
#    pragma implementation
#endif

#include "MOS_Light_pch.h"
#include "MOS_ODBTypePion.h"

#ifndef MOS_USE_INLINE
#   include "MOS_ODBTypePion.inl"
#endif

//-----------------------------------------------------------------------------
// Name: MOS_ODBTypePion constructor
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
MOS_ODBTypePion::MOS_ODBTypePion( const std::string& sName, const std::string& sSymbol )
: sName_ ( sName )
, sSymbol_( sSymbol )
{
}


//-----------------------------------------------------------------------------
// Name: MOS_ODBTypePion destructor
// Created: FBD 03-07-28
//-----------------------------------------------------------------------------
MOS_ODBTypePion::~MOS_ODBTypePion()
{
   
}
