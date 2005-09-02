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
#include "MOS_DynaObject_Camp.h"

// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_Camp constructor
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
MOS_DynaObject_Camp::MOS_DynaObject_Camp()
    : MOS_DynaObject_ABC ()
    , nTC2ID_            ( 0 )
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_Camp constructor
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
MOS_DynaObject_Camp::MOS_DynaObject_Camp( const ASN1T_MsgObjectCreation& asnMsg )
    : MOS_DynaObject_ABC ( asnMsg )
    , nTC2ID_            ( 0 )
{
    if( GetType() == EnumObjectType::camp_prisonniers )
        nTC2ID_ = asnMsg.attributs_specifiques.u.camp_prisonniers->tc2;
    else if( GetType() == EnumObjectType::camp_refugies )
        nTC2ID_ = asnMsg.attributs_specifiques.u.camp_refugies->tc2;
}
// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_Camp destructor
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
MOS_DynaObject_Camp::~MOS_DynaObject_Camp()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_Camp::ReadODB
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
void MOS_DynaObject_Camp::ReadODB( MT_XXmlInputArchive& archive )
{
    archive.Section( "Objet" );
    MOS_DynaObject_ABC::ReadODB( archive );

    archive.ReadField( "TC2", nTC2ID_ );

    archive.EndSection(); // Object
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_Camp::WriteODB
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
void MOS_DynaObject_Camp::WriteODB( MT_XXmlOutputArchive& archive ) const
{
    archive.Section( "Objet" );
    MOS_DynaObject_ABC::WriteODB( archive );

    archive.WriteField( "TC2", nTC2ID_ );

    archive.EndSection(); // Object
}
