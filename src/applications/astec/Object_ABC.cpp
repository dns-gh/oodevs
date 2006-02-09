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
#include "Object_ABC.h"

#include "Net_Def.h"
#include "App.h"
#include "World.h"
#include "AgentManager.h"
#include "Tools.h"
#include "Team.h"
#include "FireResult.h"

Object_ABC::T_ObjectIDs Object_ABC::objectIds_;
Object_ABC::T_Managers  Object_ABC::managers_;

// -----------------------------------------------------------------------------
// Name: Object_ABC constructor
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
Object_ABC::Object_ABC( ASN1T_EnumObjectType eType )
    : nID_                           ( 0 )
    , strName_                       ( ENT_Tr::ConvertFromObjectType( ( E_ObjectType )eType ) )
    , nType_                         ( eType )
    , pTeam_                         ( 0 )
    , rConstructionPercentage_       ( 0.0 )
    , rValorizationPercentage_       ( 0.0 )
    , rBypassConstructionPercentage_ ( 0.0 )
    , bPrepared_                     ( false )
    , nTypeLocalisation_             ( (ASN1T_EnumTypeLocalisation)0 )
    , strTypeDotationConstruction_   ()
    , strTypeDotationValorization_   ()
    , nNbrDotationConstruction_      ( 0 )
    , nNbrDotationValorization_      ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Object_ABC::Object_ABC
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
Object_ABC::Object_ABC( const ASN1T_MsgObjectCreation& asnMsg )
    : nID_                           ( asnMsg.oid )
    , strName_                       ( asnMsg.nom )
    , nType_                         ( asnMsg.type )
    , pTeam_                         ( App::GetApp().GetAgentManager().FindTeam( asnMsg.camp ) )
    , rConstructionPercentage_       ( 0.0 )
    , rValorizationPercentage_       ( 0.0 )
    , rBypassConstructionPercentage_ ( 0.0 )
    , bPrepared_                     ( false )
    , nTypeLocalisation_             ( asnMsg.localisation.type )
    , strTypeDotationConstruction_   ()
    , strTypeDotationValorization_   ()
    , nNbrDotationConstruction_      ( 0 )
    , nNbrDotationValorization_      ( 0 )
{
    GetIDManagerForObjectType( nType_ ).LockIdentifier( nID_ );

    for( uint i = 0; i < asnMsg.localisation.vecteur_point.n; ++i )
    {
        MT_Vector2D vTmp;
        App::GetApp().GetWorld().MosToSimMgrsCoord( (const char*)asnMsg.localisation.vecteur_point.elem[i].data, vTmp );
        pointVector_.push_back( vTmp );
        center_ += vTmp;
    }

    if( pointVector_.size() > 1 )
        center_ /= pointVector_.size();

    if( asnMsg.m.type_dotation_constructionPresent )
    {
        strTypeDotationConstruction_ = App::GetApp().GetResourceName( asnMsg.type_dotation_construction );
        nNbrDotationConstruction_    = 0;
    }
    
    if( asnMsg.m.type_dotation_valorisationPresent )
    {
        strTypeDotationValorization_ = App::GetApp().GetResourceName( asnMsg.type_dotation_valorisation );
        nNbrDotationValorization_    = 0;
    }
}



// -----------------------------------------------------------------------------
// Name: Object_ABC::~Object_ABC
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
Object_ABC::~Object_ABC()
{
    pointVector_.clear();
}


// -----------------------------------------------------------------------------
// Name: Object_ABC::Update
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
void Object_ABC::Update( const ASN1T_MsgObjectUpdate& asnMsg )
{
    bPrepared_ = asnMsg.en_preparation;

    if( asnMsg.m.nb_dotation_constructionPresent )
        nNbrDotationConstruction_ = asnMsg.nb_dotation_construction;
    if( asnMsg.m.nb_dotation_valorisationPresent )
        nNbrDotationValorization_ = asnMsg.nb_dotation_valorisation;

    if( asnMsg.m.pourcentage_constructionPresent )
        rConstructionPercentage_ = asnMsg.pourcentage_construction;
    if( asnMsg.m.pourcentage_valorisationPresent )
        rValorizationPercentage_ = asnMsg.pourcentage_valorisation;
    if( asnMsg.m.pourcentage_creation_contournementPresent )
        rBypassConstructionPercentage_ = asnMsg.pourcentage_creation_contournement;

    if( asnMsg.m.localisationPresent )
    {
        center_.Reset();
        pointVector_.clear();
        nTypeLocalisation_ = asnMsg.localisation.type;
        for( uint i = 0; i < asnMsg.localisation.vecteur_point.n; ++i )
        {
            MT_Vector2D vTmp;
            App::GetApp().GetWorld().MosToSimMgrsCoord( (const char*)asnMsg.localisation.vecteur_point.elem[i].data, vTmp );
            pointVector_.push_back( vTmp );
            center_ += vTmp;
        }
        if( pointVector_.size() > 1 )
            center_ /= pointVector_.size();
    }

    std::stringstream strOutputMsg;
    strOutputMsg << "ObjectUpdate - ID: " << (int)nID_ 
                 << " Prepared: " << (int)bPrepared_
                 << " Construction :" << rConstructionPercentage_ << "%"
                 << " Valorisation :" << rValorizationPercentage_ << "%"
                 << " Contournement :" << rBypassConstructionPercentage_ << "%";
    MT_LOG_INFO( strOutputMsg.str().c_str(), eReceived, 0 );
}


// -----------------------------------------------------------------------------
// Name: Object_ABC::ReadODB
// Created: APE 2004-08-31
// -----------------------------------------------------------------------------
void Object_ABC::ReadODB( InputArchive& archive )
{
    std::string strType;
    archive.ReadAttribute( "type", strType );

    nType_ = (ASN1T_EnumObjectType)ENT_Tr::ConvertToObjectType( strType );

    archive.ReadAttribute( "id", nID_ );
    GetIDManagerForObjectType( nType_ ).LockIdentifier( nID_ );

    std::string strTeam;
    archive.ReadField( "Armee", strTeam );
    pTeam_ = App::GetApp().GetAgentManager().FindTeam( strTeam );
    assert( pTeam_ != 0 );

    if( !archive.ReadField( "Nom", strName_, InputArchive::eNothing ) )
    {
        strName_ = ENT_Tr::ConvertFromObjectType( ( E_ObjectType )nType_ );
    }

    archive.Section( "Forme" );
    archive.Section( "Localisation" );

    std::string strLocType;
    archive.ReadAttribute( "type", strLocType );
    Tools::FromString( strLocType.c_str(), nTypeLocalisation_ );

    archive.BeginList( "Points" );
    while( archive.NextListElement() )
    {
        std::string strPos;
        archive.Section( "Point" );
            archive.Read( strPos );
        archive.EndSection(); // Point
        MT_Vector2D vPos;
        App::GetApp().GetWorld().MosToSimMgrsCoord( strPos, vPos );
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
// Name: Object_ABC::WriteODB
// Created: APE 2004-08-31
// -----------------------------------------------------------------------------
void Object_ABC::WriteODB( MT_XXmlOutputArchive& archive ) const
{
    archive.WriteAttribute( "type", ENT_Tr::ConvertFromObjectType( (E_ObjectType)nType_ ) );
    archive.WriteAttribute( "id", nID_ );
    archive.WriteField( "Armee", pTeam_->GetName() );

    if( !strName_.empty() && strName_ != ENT_Tr::ConvertFromObjectType( (E_ObjectType)nType_ ) )
        archive.WriteField( "Nom", strName_ );

    archive.Section( "Forme" );
    archive.Section( "Localisation" );

    archive.WriteAttribute( "type", (Tools::ToString( nTypeLocalisation_ )).ascii() );

    archive.BeginList( "Points", pointVector_.size() );
    for( CIT_PointVector it = pointVector_.begin(); it != pointVector_.end(); ++it )
    {
        archive.Section( "Point" );
        std::string strPos;
        App::GetApp().GetWorld().SimToMosMgrsCoord( (*it), strPos );
        archive << strPos;
        archive.EndSection(); // Point
    }
    archive.EndList(); // Points

    archive.EndSection(); // Forme
    archive.EndSection(); // Localisation
}


// -----------------------------------------------------------------------------
// Name: Object_ABC::GetIDManagerForObjectType
// Created: APE 2004-09-01
// -----------------------------------------------------------------------------
IDManager& Object_ABC::GetIDManagerForObjectType( ASN1T_EnumObjectType nType )
{
    return *managers_[ objectIds_[ nType ] ];
}

// -----------------------------------------------------------------------------
// Name: Object_ABC::GetIDManagerForObjectType
// Created: HME 2005-11-08
// -----------------------------------------------------------------------------
IDManager& Object_ABC::GetIDManagerForObjectType( uint nType )
{
    return *managers_[ nType ];
}

// -----------------------------------------------------------------------------
// Name: Object_ABC::InitializeObjectIds
// Created: AGE 2005-04-05
// -----------------------------------------------------------------------------
void Object_ABC::InitializeObjectIds( InputArchive& archive )
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
            IDManager*& pManager = managers_[ nId ];
            if( ! pManager )
                pManager = new IDManager( nId );
        }
        archive.EndSection(); // Classe
    };
}

// -----------------------------------------------------------------------------
// Name: Object_ABC::OnReceiveMsgExplosion
// Created: SBO 2005-09-07
// -----------------------------------------------------------------------------
void Object_ABC::OnReceiveMsgExplosion( const ASN1T_FireDamagesPion& asnMsg )
{
    explosionResults_.push_back( new FireResult( asnMsg ) );
    if( explosionResults_.size() > 20 )
        explosionResults_.erase( explosionResults_.begin() );
}
    
// -----------------------------------------------------------------------------
// Name: Object_ABC::DeleteAllFireResults
// Created: SBO 2005-08-30
// -----------------------------------------------------------------------------
void Object_ABC::DeleteAllExplosionResults()
{
    for( CIT_FireResults it = explosionResults_.begin(); it != explosionResults_.end(); ++it )
        delete *it;
    explosionResults_.clear();
}
