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
#include "MOS_Object_Camp.h"

// -----------------------------------------------------------------------------
// Name: MOS_Object_Camp constructor
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
MOS_Object_Camp::MOS_Object_Camp( ASN1T_EnumObjectType eType )
    : MOS_Object_ABC ( eType )
    , nTC2ID_            ( 0 )
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: MOS_Object_Camp constructor
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
MOS_Object_Camp::MOS_Object_Camp( const ASN1T_MsgObjectCreation& asnMsg )
    : MOS_Object_ABC ( asnMsg )
    , nTC2ID_            ( 0 )
{
    if( GetType() == EnumObjectType::camp_prisonniers )
        nTC2ID_ = asnMsg.attributs_specifiques.u.camp_prisonniers->tc2;
    else if( GetType() == EnumObjectType::camp_refugies )
        nTC2ID_ = asnMsg.attributs_specifiques.u.camp_refugies->tc2;
}
// -----------------------------------------------------------------------------
// Name: MOS_Object_Camp destructor
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
MOS_Object_Camp::~MOS_Object_Camp()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MOS_Object_Camp::ReadODB
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
void MOS_Object_Camp::ReadODB( MOS_InputArchive& archive )
{
    MOS_Object_ABC::ReadODB( archive );

    archive.ReadField( "TC2", nTC2ID_ );
}

// -----------------------------------------------------------------------------
// Name: MOS_Object_Camp::WriteODB
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
void MOS_Object_Camp::WriteODB( MT_XXmlOutputArchive& archive ) const
{
    archive.Section( "Objet" );
    MOS_Object_ABC::WriteODB( archive );

    archive.WriteField( "TC2", nTC2ID_ );

    archive.EndSection(); // Object
}
