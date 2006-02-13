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
#include "Object_ItineraireLogistique.h"

// -----------------------------------------------------------------------------
// Name: Object_ItineraireLogistique constructor
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
Object_ItineraireLogistique::Object_ItineraireLogistique( ASN1T_EnumObjectType eType /* = EnumObjectType::itineraire_logistique */ )
    : Object_ABC ( eType )
    , bEquipped_         ( false )
    , nFlow_             ( 0 )
    , nWidth_            ( 0 )
    , nLength_           ( 0 )
    , nMaxWeight_        ( 0 )
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: Object_ItineraireLogistique constructor
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
Object_ItineraireLogistique::Object_ItineraireLogistique( const ASN1T_MsgObjectCreation& asnMsg )
    : Object_ABC ( asnMsg )
    , bEquipped_         ( asnMsg.attributs_specifiques.u.itineraire_logistique->itineraire_equipe )
    , nFlow_             ( asnMsg.attributs_specifiques.u.itineraire_logistique->debit )
    , nWidth_            ( asnMsg.attributs_specifiques.u.itineraire_logistique->largeur )
    , nLength_           ( asnMsg.attributs_specifiques.u.itineraire_logistique->longueur )
    , nMaxWeight_        ( asnMsg.attributs_specifiques.u.itineraire_logistique->poids_max_supporte )
{
    // NOTHING
}
// -----------------------------------------------------------------------------
// Name: Object_ItineraireLogistique destructor
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
Object_ItineraireLogistique::~Object_ItineraireLogistique()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Object_ItineraireLogistique::ReadODB
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
void Object_ItineraireLogistique::ReadODB( InputArchive& archive )
{
    Object_ABC::ReadODB( archive );

    archive.ReadField( "PoidsSupporte", nMaxWeight_ );
    archive.ReadField( "Largeur"      , nWidth_     );
    archive.ReadField( "Longueur"     , nLength_    );
    archive.ReadField( "Debit"        , nFlow_      );
}

// -----------------------------------------------------------------------------
// Name: Object_ItineraireLogistique::WriteODB
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
void Object_ItineraireLogistique::WriteODB( MT_XXmlOutputArchive& archive ) const
{
    archive.Section( "Objet" );
    Object_ABC::WriteODB( archive );

    archive.WriteField( "PoidsSupporte", nMaxWeight_ );
    archive.WriteField( "Largeur"      , nWidth_     );
    archive.WriteField( "Longueur"     , nLength_    );
    archive.WriteField( "Debit"        , nFlow_      );

    archive.EndSection(); // Object
}
