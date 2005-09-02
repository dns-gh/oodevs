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
#include "MOS_DynaObject_ItineraireLogistique.h"

// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_ItineraireLogistique constructor
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
MOS_DynaObject_ItineraireLogistique::MOS_DynaObject_ItineraireLogistique()
    : MOS_DynaObject_ABC ()
    , bEquipped_         ( false )
    , nFlow_             ( 0 )
    , nWidth_            ( 0 )
    , nLength_           ( 0 )
    , nMaxWeight_        ( 0 )
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_ItineraireLogistique constructor
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
MOS_DynaObject_ItineraireLogistique::MOS_DynaObject_ItineraireLogistique( const ASN1T_MsgObjectCreation& asnMsg )
    : MOS_DynaObject_ABC ( asnMsg )
    , bEquipped_         ( asnMsg.attributs_specifiques.u.itineraire_logistique->itineraire_equipe )
    , nFlow_             ( asnMsg.attributs_specifiques.u.itineraire_logistique->debit )
    , nWidth_            ( asnMsg.attributs_specifiques.u.itineraire_logistique->largeur )
    , nLength_           ( asnMsg.attributs_specifiques.u.itineraire_logistique->longueur )
    , nMaxWeight_        ( asnMsg.attributs_specifiques.u.itineraire_logistique->poids_max_supporte )
{
    // NOTHING
}
// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_ItineraireLogistique destructor
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
MOS_DynaObject_ItineraireLogistique::~MOS_DynaObject_ItineraireLogistique()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_ItineraireLogistique::ReadODB
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
void MOS_DynaObject_ItineraireLogistique::ReadODB( MT_XXmlInputArchive& archive )
{
    archive.Section( "Objet" );
    MOS_DynaObject_ABC::ReadODB( archive );

    archive.ReadField( "PoidsSupporte", nMaxWeight_ );
    archive.ReadField( "Largeur"      , nWidth_     );
    archive.ReadField( "Longueur"     , nLength_    );
    archive.ReadField( "Debit"        , nFlow_      );

    archive.EndSection(); // Object
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_ItineraireLogistique::WriteODB
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
void MOS_DynaObject_ItineraireLogistique::WriteODB( MT_XXmlOutputArchive& archive ) const
{
    archive.Section( "Objet" );
    MOS_DynaObject_ABC::WriteODB( archive );

    archive.WriteField( "PoidsSupporte", nMaxWeight_ );
    archive.WriteField( "Largeur"      , nWidth_     );
    archive.WriteField( "Longueur"     , nLength_    );
    archive.WriteField( "Debit"        , nFlow_      );

    archive.EndSection(); // Object
}
