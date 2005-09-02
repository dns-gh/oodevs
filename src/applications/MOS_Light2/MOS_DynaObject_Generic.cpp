// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-09-02 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MOS_DynaObject_Generic.h"

// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_Generic constructor
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
MOS_DynaObject_Generic::MOS_DynaObject_Generic()
    : MOS_DynaObject_ABC ()
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_Generic constructor
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
MOS_DynaObject_Generic::MOS_DynaObject_Generic( const ASN1T_MsgObjectCreation& asnMsg )
    : MOS_DynaObject_ABC ( asnMsg )
{
    // NOTHING
}
// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_Generic destructor
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
MOS_DynaObject_Generic::~MOS_DynaObject_Generic()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_Generic::ReadODB
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
void MOS_DynaObject_Generic::ReadODB( MT_XXmlInputArchive& archive )
{
    archive.Section( "Objet" );
    MOS_DynaObject_ABC::ReadODB( archive );
    archive.EndSection(); // Object
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_Generic::WriteODB
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
void MOS_DynaObject_Generic::WriteODB( MT_XXmlOutputArchive& archive ) const
{
    archive.Section( "Objet" );
    MOS_DynaObject_ABC::WriteODB( archive );
    archive.EndSection(); // Object
}
