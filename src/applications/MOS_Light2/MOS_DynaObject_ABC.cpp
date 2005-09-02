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
#include "MOS_DynaObject_ABC.h"

#include "MOS_Net_Def.h"
#include "MOS_App.h"
#include "MOS_World.h"
#include "MOS_AgentManager.h"
#include "MOS_Tools.h"
#include "MOS_Team.h"

#ifndef MOS_USE_INLINE
#   include "MOS_DynaObject_ABC.inl"
#endif

MOS_DynaObject_ABC::T_ObjectIDs MOS_DynaObject_ABC::objectIds_;
MOS_DynaObject_ABC::T_Managers  MOS_DynaObject_ABC::managers_;


// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_ABC constructor
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
MOS_DynaObject_ABC::MOS_DynaObject_ABC()
    : nID_                           ( 0 )
    , nType_                         ( (ASN1T_EnumObjectType)0 )
    , pTeam_                         ( 0 )
    , rConstructionPercentage_       ( 0.0 )
    , rValorizationPercentage_       ( 0.0 )
    , rBypassConstructionPercentage_ ( 0.0 )
    , bPrepared_                     ( false )
    , nTypeLocalisation_             ( (ASN1T_EnumTypeLocalisation)0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_ABC::MOS_DynaObject_ABC
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
MOS_DynaObject_ABC::MOS_DynaObject_ABC( const ASN1T_MsgObjectCreation& asnMsg )
    : nID_                           ( asnMsg.oid )
    , nType_                         ( asnMsg.type )
    , pTeam_                         ( MOS_App::GetApp().GetAgentManager().FindTeam( asnMsg.camp ) )
    , rConstructionPercentage_       ( 0.0 )
    , rValorizationPercentage_       ( 0.0 )
    , rBypassConstructionPercentage_ ( 0.0 )
    , bPrepared_                     ( false )
    , nTypeLocalisation_             ( asnMsg.localisation.type )
{
    MOS_DynaObject_ABC::GetIDManagerForObjectType( nType_ ).LockIdentifier( nID_ );

    for( uint i = 0; i < asnMsg.localisation.vecteur_point.n; ++i )
    {
        MT_Vector2D vTmp;
        MOS_App::GetApp().GetWorld().MosToSimMgrsCoord( (const char*)asnMsg.localisation.vecteur_point.elem[i].data, vTmp );
        pointVector_.push_back( vTmp );
        center_ += vTmp;
    }

    if( pointVector_.size() > 1 )
        center_ /= pointVector_.size();
}



// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_ABC::~MOS_DynaObject_ABC
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
MOS_DynaObject_ABC::~MOS_DynaObject_ABC()
{
    pointVector_.clear();
}


// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_ABC::Update
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
void MOS_DynaObject_ABC::Update( const ASN1T_MsgObjectUpdate& asnMsg )
{
    bPrepared_ = asnMsg.en_preparation;

    if( asnMsg.m.pourcentage_constructionPresent )
        rConstructionPercentage_ = asnMsg.pourcentage_construction;
    if( asnMsg.m.pourcentage_valorisationPresent )
        rValorizationPercentage_ = asnMsg.pourcentage_valorisation;
    if( asnMsg.m.pourcentage_creation_contournementPresent )
        rBypassConstructionPercentage_ = asnMsg.pourcentage_creation_contournement;

    std::stringstream strOutputMsg;
    strOutputMsg << "ObjectUpdate - ID: " << (int)nID_ 
                 << " Prepared: " << (int)bPrepared_
                 << " Construction :" << rConstructionPercentage_ << "%"
                 << " Valorisation :" << rValorizationPercentage_ << "%"
                 << " Contournement :" << rBypassConstructionPercentage_ << "%";
    MT_LOG_INFO( strOutputMsg.str().c_str(), eReceived, 0 );
}


// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_ABC::ReadODB
// Created: APE 2004-08-31
// -----------------------------------------------------------------------------
void MOS_DynaObject_ABC::ReadODB( MT_XXmlInputArchive& archive )
{
    std::string strType;
    archive.ReadAttribute( "type", strType );

    nType_ = (ASN1T_EnumObjectType)ENT_Tr::ConvertToObjectType( strType );

    archive.ReadAttribute( "id", nID_ );

    std::string strTeam;
    archive.ReadField( "Armee", strTeam );
    pTeam_ = MOS_App::GetApp().GetAgentManager().FindTeam( strTeam );
    assert( pTeam_ != 0 );

    archive.Section( "Forme" );
    archive.Section( "Localisation" );

    std::string strLocType;
    archive.ReadAttribute( "type", strLocType );
    MOS_Tools::FromString( strLocType.c_str(), nTypeLocalisation_ );

    archive.BeginList( "Points" );
    while( archive.NextListElement() )
    {
        std::string strPos;
        archive.Section( "Point" );
        archive >> strPos;
        archive.EndSection(); // Point
        MT_Vector2D vPos;
        MOS_App::GetApp().GetWorld().MosToSimMgrsCoord( strPos, vPos );
        pointVector_.push_back( vPos );
    }
    archive.EndList(); // Points

    // Compute object center.
    center_ = MT_Vector2D( 0.0, 0.0 );
    for( IT_PointVector it = pointVector_.begin(); it != pointVector_.end(); ++it )
        center_ = center_ + (*it) / (double)pointVector_.size();

    archive.EndSection(); // Localisation
    archive.EndSection(); // Forme
}


// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_ABC::WriteODB
// Created: APE 2004-08-31
// -----------------------------------------------------------------------------
void MOS_DynaObject_ABC::WriteODB( MT_XXmlOutputArchive& archive ) const
{
    archive.WriteAttribute( "type", ENT_Tr::ConvertFromObjectType( (E_ObjectType)nType_ ) );
    archive.WriteAttribute( "id", nID_ );
    archive.WriteField( "Armee", pTeam_->GetName() );

    archive.Section( "Forme" );
    archive.Section( "Localisation" );

    archive.WriteAttribute( "type", (MOS_Tools::ToString( nTypeLocalisation_ )).ascii() );

    archive.BeginList( "Points", pointVector_.size() );
    for( CIT_PointVector it = pointVector_.begin(); it != pointVector_.end(); ++it )
    {
        archive.Section( "Point" );
        std::string strPos;
        MOS_App::GetApp().GetWorld().SimToMosMgrsCoord( (*it), strPos );
        archive << strPos;
        archive.EndSection(); // Point
    }
    archive.EndList(); // Points

    archive.EndSection(); // Forme
    archive.EndSection(); // Localisation
}


// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_ABC::GetIDManagerForObjectType
// Created: APE 2004-09-01
// -----------------------------------------------------------------------------
MOS_IDManager& MOS_DynaObject_ABC::GetIDManagerForObjectType( ASN1T_EnumObjectType nType )
{
    return *managers_[ objectIds_[ nType ] ];
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObject_ABC::InitializeObjectIds
// Created: AGE 2005-04-05
// -----------------------------------------------------------------------------
void MOS_DynaObject_ABC::InitializeObjectIds( MT_XXmlInputArchive& archive )
{
    archive.BeginList( "Classes" );
    while( archive.NextListElement() )
    {
        archive.Section( "Classe" );
        std::string strObjectName;
        archive.ReadAttribute( "nom", strObjectName );
        unsigned int nId;
        archive.ReadAttribute( "id", nId );
        ASN1T_EnumObjectType nType = (ASN1T_EnumObjectType)ENT_Tr::ConvertToObjectType( strObjectName );
        if( nType != -1 )
        {
            objectIds_[ nType ] = nId;
            MOS_IDManager*& pManager = managers_[ nId ];
            if( ! pManager )
                pManager = new MOS_IDManager( nId );
        }
        archive.EndSection(); // Classe
    };
}
