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
#include "MOS_Object_Generic.h"

// -----------------------------------------------------------------------------
// Name: MOS_Object_Generic constructor
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
MOS_Object_Generic::MOS_Object_Generic( ASN1T_EnumObjectType eType )
    : MOS_Object_ABC ( eType )
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: MOS_Object_Generic constructor
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
MOS_Object_Generic::MOS_Object_Generic( const ASN1T_MsgObjectCreation& asnMsg )
    : MOS_Object_ABC ( asnMsg )
{
    // NOTHING
}
// -----------------------------------------------------------------------------
// Name: MOS_Object_Generic destructor
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
MOS_Object_Generic::~MOS_Object_Generic()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MOS_Object_Generic::ReadODB
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
void MOS_Object_Generic::ReadODB( MOS_InputArchive& archive )
{
    MOS_Object_ABC::ReadODB( archive );
}

// -----------------------------------------------------------------------------
// Name: MOS_Object_Generic::WriteODB
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
void MOS_Object_Generic::WriteODB( MT_XXmlOutputArchive& archive ) const
{
    archive.Section( "Objet" );
    MOS_Object_ABC::WriteODB( archive );
    archive.EndSection(); // Object
}
