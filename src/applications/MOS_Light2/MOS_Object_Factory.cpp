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
#include "MOS_Object_Factory.h"

#include "MOS_Object_ABC.h"
#include "MOS_Object_Generic.h"
#include "MOS_Object_SiteFranchissement.h"
#include "MOS_Object_NBC.h"
#include "MOS_Object_Camp.h"
#include "MOS_Object_ROTA.h"
#include "MOS_Object_ItineraireLogistique.h"

// -----------------------------------------------------------------------------
// Name: MOS_Object_Factory::Create
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
MOS_Object_ABC* MOS_Object_Factory::Create( ASN1T_EnumObjectType eType )
{
    switch( eType )
    {
    case EnumObjectType::site_franchissement:
        return new MOS_Object_SiteFranchissement();
    case EnumObjectType::nuage_nbc:
    case EnumObjectType::zone_nbc:
        return new MOS_Object_NBC( eType );
    case EnumObjectType::rota:
        return new MOS_Object_ROTA();
    case EnumObjectType::camp_prisonniers:
    case EnumObjectType::camp_refugies:
        return new MOS_Object_Camp( eType );
    case EnumObjectType::itineraire_logistique:
        return new MOS_Object_ItineraireLogistique();
    default:
        return new MOS_Object_Generic( eType );
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_Object_Factory::Create
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
MOS_Object_ABC* MOS_Object_Factory::Create( MOS_InputArchive& archive )
{
    // read type field to determine what kind of object to create
    archive.Section( "Objet" );
    std::string strType;
    archive.ReadAttribute( "type", strType );

    MOS_Object_ABC* pObject = Create( (ASN1T_EnumObjectType)ENT_Tr::ConvertToObjectType( strType ) );
    assert( pObject );
    pObject->ReadODB( archive );
    archive.EndSection(); // Object

    return pObject;
}

// -----------------------------------------------------------------------------
// Name: MOS_Object_Factory::Create
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
MOS_Object_ABC* MOS_Object_Factory::Create( const ASN1T_MsgObjectCreation& asnMsg  )
{
    switch( asnMsg.type )
    {
    case EnumObjectType::site_franchissement:
        return new MOS_Object_SiteFranchissement( asnMsg );
    case EnumObjectType::nuage_nbc:
    case EnumObjectType::zone_nbc:
        return new MOS_Object_NBC( asnMsg );
    case EnumObjectType::rota:
        return new MOS_Object_ROTA( asnMsg );
    case EnumObjectType::camp_prisonniers:
    case EnumObjectType::camp_refugies:
        return new MOS_Object_Camp( asnMsg );
    case EnumObjectType::itineraire_logistique:
        return new MOS_Object_ItineraireLogistique( asnMsg );
    default:
        return new MOS_Object_Generic( asnMsg );
    }
}
