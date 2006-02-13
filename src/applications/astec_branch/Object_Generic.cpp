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

#include "astec_pch.h"
#include "Object_Generic.h"

// -----------------------------------------------------------------------------
// Name: Object_Generic constructor
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
Object_Generic::Object_Generic( ASN1T_EnumObjectType eType )
    : Object_ABC ( eType )
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: Object_Generic constructor
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
Object_Generic::Object_Generic( const ASN1T_MsgObjectCreation& asnMsg )
    : Object_ABC ( asnMsg )
{
    // NOTHING
}
// -----------------------------------------------------------------------------
// Name: Object_Generic destructor
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
Object_Generic::~Object_Generic()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Object_Generic::ReadODB
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
void Object_Generic::ReadODB( InputArchive& archive )
{
    Object_ABC::ReadODB( archive );
}

// -----------------------------------------------------------------------------
// Name: Object_Generic::WriteODB
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
void Object_Generic::WriteODB( MT_XXmlOutputArchive& archive ) const
{
    archive.Section( "Objet" );
    Object_ABC::WriteODB( archive );
    archive.EndSection(); // Object
}
