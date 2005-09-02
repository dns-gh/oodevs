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
#include "MOS_DynaObject_SiteFranchissement.h"

// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_SiteFranchissement constructor
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
MOS_DynaObject_SiteFranchissement::MOS_DynaObject_SiteFranchissement()
    : MOS_DynaObject_ABC ()
    , nLargeur_          ( 0 )
    , nProfondeur_       ( 0 )
    , nVitesseCourant_   ( 0 )
    , bBergesAAmenager_  ( false )
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_SiteFranchissement constructor
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
MOS_DynaObject_SiteFranchissement::MOS_DynaObject_SiteFranchissement( const ASN1T_MsgObjectCreation& asnMsg )
    : MOS_DynaObject_ABC ( asnMsg )
    , nLargeur_          ( asnMsg.attributs_specifiques.u.site_franchissement->largeur )
    , nProfondeur_       ( asnMsg.attributs_specifiques.u.site_franchissement->profondeur )
    , nVitesseCourant_   ( asnMsg.attributs_specifiques.u.site_franchissement->vitesse_courant )
    , bBergesAAmenager_  ( asnMsg.attributs_specifiques.u.site_franchissement->berges_a_amenager )
{
    // NOTHING
}
// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_SiteFranchissement destructor
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
MOS_DynaObject_SiteFranchissement::~MOS_DynaObject_SiteFranchissement()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_SiteFranchissement::ReadODB
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
void MOS_DynaObject_SiteFranchissement::ReadODB( MT_XXmlInputArchive& archive )
{
    archive.Section( "Objet" );
    MOS_DynaObject_ABC::ReadODB( archive );

    archive.ReadField( "Largeur"        , nLargeur_         );
    archive.ReadField( "Profondeur"     , nProfondeur_      );
    archive.ReadField( "VitesseCourant" , nVitesseCourant_  );
    archive.ReadField( "BergesAAmenager", bBergesAAmenager_ );

    archive.EndSection(); // Object
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_SiteFranchissement::WriteODB
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
void MOS_DynaObject_SiteFranchissement::WriteODB( MT_XXmlOutputArchive& archive ) const
{
    archive.Section( "Objet" );
    MOS_DynaObject_ABC::WriteODB( archive );

    archive.WriteField( "Largeur"        , nLargeur_         );
    archive.WriteField( "Profondeur"     , nProfondeur_      );
    archive.WriteField( "VitesseCourant" , nVitesseCourant_  );
    archive.WriteField( "BergesAAmenager", bBergesAAmenager_ );

    archive.EndSection(); // Object
}
