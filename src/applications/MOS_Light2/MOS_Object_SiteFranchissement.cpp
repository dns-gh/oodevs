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
#include "MOS_Object_SiteFranchissement.h"

// -----------------------------------------------------------------------------
// Name: MOS_Object_SiteFranchissement constructor
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
MOS_Object_SiteFranchissement::MOS_Object_SiteFranchissement( ASN1T_EnumObjectType eType /* = EnumObjectType::site_franchissement */ )
    : MOS_Object_ABC ( eType )
    , nWidth_            ( 0 )
    , nDepth_            ( 0 )
    , nSpeed_            ( 0 )
    , bConstruct_        ( false )
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: MOS_Object_SiteFranchissement constructor
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
MOS_Object_SiteFranchissement::MOS_Object_SiteFranchissement( const ASN1T_MsgObjectCreation& asnMsg )
    : MOS_Object_ABC ( asnMsg )
    , nWidth_            ( asnMsg.attributs_specifiques.u.site_franchissement->largeur )
    , nDepth_            ( asnMsg.attributs_specifiques.u.site_franchissement->profondeur )
    , nSpeed_            ( asnMsg.attributs_specifiques.u.site_franchissement->vitesse_courant )
    , bConstruct_        ( asnMsg.attributs_specifiques.u.site_franchissement->berges_a_amenager )
{
    // NOTHING
}
// -----------------------------------------------------------------------------
// Name: MOS_Object_SiteFranchissement destructor
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
MOS_Object_SiteFranchissement::~MOS_Object_SiteFranchissement()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MOS_Object_SiteFranchissement::ReadODB
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
void MOS_Object_SiteFranchissement::ReadODB( MT_XXmlInputArchive& archive )
{
    MOS_Object_ABC::ReadODB( archive );

    archive.ReadField( "Largeur"        , nWidth_     );
    archive.ReadField( "Profondeur"     , nDepth_     );
    archive.ReadField( "VitesseCourant" , nSpeed_     );
    archive.ReadField( "BergesAAmenager", bConstruct_ );
}

// -----------------------------------------------------------------------------
// Name: MOS_Object_SiteFranchissement::WriteODB
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
void MOS_Object_SiteFranchissement::WriteODB( MT_XXmlOutputArchive& archive ) const
{
    archive.Section( "Objet" );
    MOS_Object_ABC::WriteODB( archive );

    archive.WriteField( "Largeur"        , nWidth_     );
    archive.WriteField( "Profondeur"     , nDepth_     );
    archive.WriteField( "VitesseCourant" , nSpeed_     );
    archive.WriteField( "BergesAAmenager", bConstruct_ );

    archive.EndSection(); // Object
}
