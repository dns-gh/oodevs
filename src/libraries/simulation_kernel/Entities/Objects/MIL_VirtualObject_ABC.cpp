//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_VirtualObject_ABC.cpp $
// $Author: Jvt $
// $Modtime: 23/03/05 17:54 $
// $Revision: 2 $
// $Workfile: MIL_VirtualObject_ABC.cpp $
//
//*****************************************************************************

#include "MIL_pch.h"

#include "MIL_VirtualObject_ABC.h"
#include "MIL_VirtualObjectType.h"

// -----------------------------------------------------------------------------
// Name: MIL_VirtualObject_ABC constructor
// Created: NLD 2004-10-26
// -----------------------------------------------------------------------------
MIL_VirtualObject_ABC::MIL_VirtualObject_ABC( const MIL_VirtualObjectType& type )
    : MIL_Object_ABC()
    , type_         ( type )
{

}

// -----------------------------------------------------------------------------
// Name: MIL_VirtualObject_ABC destructor
// Created: NLD 2004-10-26
// -----------------------------------------------------------------------------
MIL_VirtualObject_ABC::~MIL_VirtualObject_ABC()
{

}
