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
#include "MOS_DynaObject_Factory.h"

#include "MOS_DynaObject_ABC.h"
#include "MOS_DynaObject_Generic.h"
#include "MOS_DynaObject_SiteFranchissement.h"
#include "MOS_DynaObject_NBC.h"
#include "MOS_DynaObject_Camp.h"
#include "MOS_DynaObject_ROTA.h"
#include "MOS_DynaObject_ItineraireLogistique.h"

// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_Factory::Create
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
MOS_DynaObject_ABC* MOS_DynaObject_Factory::Create( ASN1T_EnumObjectType eType )
{
    switch( eType )
    {
    case EnumObjectType::site_franchissement:
        return new MOS_DynaObject_SiteFranchissement();
    case EnumObjectType::nuage_nbc:
    case EnumObjectType::zone_nbc:
        return new MOS_DynaObject_NBC();
    case EnumObjectType::rota:
        return new MOS_DynaObject_ROTA();
    case EnumObjectType::camp_prisonniers:
    case EnumObjectType::camp_refugies:
        return new MOS_DynaObject_Camp();
    case EnumObjectType::itineraire_logistique:
        return new MOS_DynaObject_ItineraireLogistique();
    default:
        return new MOS_DynaObject_Generic();
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_Factory::Create
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
MOS_DynaObject_ABC* MOS_DynaObject_Factory::Create( MT_XXmlInputArchive& archive )
{
    // read type field to determine what kind of object to create
    archive.Section( "Objet" );
    std::string strType;
    archive.ReadAttribute( "type", strType );
    archive.EndSection(); // Object

    MOS_DynaObject_ABC* pDynaObject = Create( (ASN1T_EnumObjectType)ENT_Tr::ConvertToObjectType( strType ) );
    assert( pDynaObject );
    pDynaObject->ReadODB( archive );

    return pDynaObject;
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_Factory::Create
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
MOS_DynaObject_ABC* MOS_DynaObject_Factory::Create( const ASN1T_MsgObjectCreation& asnMsg  )
{
    switch( asnMsg.type )
    {
    case EnumObjectType::site_franchissement:
        return new MOS_DynaObject_SiteFranchissement( asnMsg );
    case EnumObjectType::nuage_nbc:
    case EnumObjectType::zone_nbc:
        return new MOS_DynaObject_NBC( asnMsg );
    case EnumObjectType::rota:
        return new MOS_DynaObject_ROTA( asnMsg );
    case EnumObjectType::camp_prisonniers:
    case EnumObjectType::camp_refugies:
        return new MOS_DynaObject_Camp( asnMsg );
    case EnumObjectType::itineraire_logistique:
        return new MOS_DynaObject_ItineraireLogistique( asnMsg );
    default:
        return new MOS_DynaObject_Generic( asnMsg );
    }
}
