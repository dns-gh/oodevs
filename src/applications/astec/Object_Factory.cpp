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
#include "Object_Factory.h"

#include "Object_ABC.h"
#include "Object_Generic.h"
#include "Object_SiteFranchissement.h"
#include "Object_NBC.h"
#include "Object_Camp.h"
#include "Object_ROTA.h"
#include "Object_ItineraireLogistique.h"

// -----------------------------------------------------------------------------
// Name: Object_Factory::Create
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
Object_ABC* Object_Factory::Create( ASN1T_EnumObjectType eType )
{
    switch( eType )
    {
    case EnumObjectType::site_franchissement:
        return new Object_SiteFranchissement();
    case EnumObjectType::nuage_nbc:
    case EnumObjectType::zone_nbc:
        return new Object_NBC( eType );
    case EnumObjectType::rota:
        return new Object_ROTA();
    case EnumObjectType::camp_prisonniers:
    case EnumObjectType::camp_refugies:
        return new Object_Camp( eType );
    case EnumObjectType::itineraire_logistique:
        return new Object_ItineraireLogistique();
    default:
        return new Object_Generic( eType );
    }
}

// -----------------------------------------------------------------------------
// Name: Object_Factory::Create
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
Object_ABC* Object_Factory::Create( InputArchive& archive )
{
    // read type field to determine what kind of object to create
    archive.Section( "Objet" );
    std::string strType;
    archive.ReadAttribute( "type", strType );

    Object_ABC* pObject = Create( (ASN1T_EnumObjectType)ENT_Tr::ConvertToObjectType( strType ) );
    assert( pObject );
    pObject->ReadODB( archive );
    archive.EndSection(); // Object

    return pObject;
}

// -----------------------------------------------------------------------------
// Name: Object_Factory::Create
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
Object_ABC* Object_Factory::Create( const ASN1T_MsgObjectCreation& asnMsg  )
{
    switch( asnMsg.type )
    {
    case EnumObjectType::site_franchissement:
        return new Object_SiteFranchissement( asnMsg );
    case EnumObjectType::nuage_nbc:
    case EnumObjectType::zone_nbc:
        return new Object_NBC( asnMsg );
    case EnumObjectType::rota:
        return new Object_ROTA( asnMsg );
    case EnumObjectType::camp_prisonniers:
    case EnumObjectType::camp_refugies:
        return new Object_Camp( asnMsg );
    case EnumObjectType::itineraire_logistique:
        return new Object_ItineraireLogistique( asnMsg );
    default:
        return new Object_Generic( asnMsg );
    }
}
