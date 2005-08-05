//*****************************************************************************
//
// $Created: NLD 2003-01-29 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_DynaObject.cpp $
// $Author: Age $
// $Modtime: 4/07/05 10:57 $
// $Revision: 12 $
// $Workfile: MOS_DynaObject.cpp $
//
//*****************************************************************************

#include "MOS_Light2_Pch.h"
#include "MOS_DynaObject.h"

#include "MOS_Net_Def.h"
#include "MOS_App.h"
#include "MOS_World.h"
#include "MOS_AgentManager.h"
#include "MOS_Tools.h"
#include "MOS_Team.h"

#ifndef MOS_USE_INLINE
#   include "MOS_DynaObject.inl"
#endif

MOS_DynaObject::T_ObjectIDs MOS_DynaObject::objectIds_;
MOS_DynaObject::T_Managers MOS_DynaObject::managers_;

//-----------------------------------------------------------------------------
// Name: MOS_DynaObject constructor
// Created: NLD 2003-01-29
//-----------------------------------------------------------------------------
MOS_DynaObject::MOS_DynaObject()
    : pTeam_                             ( 0 )
    , rConstructionPercentage_           ( 0.0 )
    , rValorizationPercentage_           ( 0.0 )
    , rBypassConstructionPercentage_     ( 0.0 )
    , bPrepared_                         ( false )
    , nSiteFranchissementLargeur_        ( 0 )
    , nSiteFranchissementProfondeur_     ( 0 )
    , nSiteFranchissementVitesseCourant_ ( 0 )
    , nSiteFranchissementBergesAAmenager_( false )
    , bAttrSiteFranchissementPresent_    ( false )
    , bAttrNuageNBCPresent_              ( false )
    , nNuageNBCAgentNbcID_               ( 0 )
{
}


//-----------------------------------------------------------------------------
// Name: MOS_DynaObject destructor
// Created: NLD 2003-01-29
//-----------------------------------------------------------------------------
MOS_DynaObject::~MOS_DynaObject()
{
}


//-----------------------------------------------------------------------------
// Name: MOS_DynaObject::Initialize
// Created: NLD 2003-01-29
//-----------------------------------------------------------------------------
void MOS_DynaObject::Initialize( const ASN1T_MsgObjectCreation& asnMsg )
{
    rConstructionPercentage_       = 0.;
    rValorizationPercentage_       = 0.;
    rBypassConstructionPercentage_ = 0.;
    bPrepared_                     = false;

    nID_   = asnMsg.oid;

    nType_ = asnMsg.type;

    MOS_DynaObject::GetIDManagerForObjectType( nType_ ).LockIdentifier( nID_ );

    nTypeLocalisation_ = asnMsg.localisation.type;
    for( uint i = 0; i < asnMsg.localisation.vecteur_point.n; ++i )
    {
        MT_Vector2D vTmp;
        MOS_App::GetApp().GetWorld().MosToSimMgrsCoord( (const char*)asnMsg.localisation.vecteur_point.elem[i].data, vTmp );
        pointVector_.push_back( vTmp );
        center_ += vTmp;
    }

    if( asnMsg.m.attributs_specifiquesPresent )
    {
        if( asnMsg.attributs_specifiques.t == T_AttrObjectSpecific_site_franchissement )
        {
            bAttrSiteFranchissementPresent_     = true;
            nSiteFranchissementLargeur_         = asnMsg.attributs_specifiques.u.site_franchissement->largeur;
            nSiteFranchissementProfondeur_      = asnMsg.attributs_specifiques.u.site_franchissement->profondeur;
            nSiteFranchissementVitesseCourant_  = asnMsg.attributs_specifiques.u.site_franchissement->vitesse_courant;
            nSiteFranchissementBergesAAmenager_ = asnMsg.attributs_specifiques.u.site_franchissement->berges_a_amenager;    
        }
        else if( asnMsg.attributs_specifiques.t == T_AttrObjectSpecific_nuage_nbc )
        {
            bAttrNuageNBCPresent_ = true;
            nNuageNBCAgentNbcID_ = asnMsg.attributs_specifiques.u.nuage_nbc->agent_nbc;
        }
    }

    pTeam_ = MOS_App::GetApp().GetAgentManager().FindTeam( asnMsg.camp );
    assert( pTeam_ );

    if( pointVector_.size() > 1 )
        center_ /= pointVector_.size();
}


//-----------------------------------------------------------------------------
// Name: MOS_DynaObject::Update
// Created: NLD 2003-04-29
//-----------------------------------------------------------------------------
void MOS_DynaObject::Update( const ASN1T_MsgObjectUpdate& asnMsg )
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
// Name: MOS_DynaObject::ReadODB
/** @param  archive 
*/
// Created: APE 2004-08-31
// -----------------------------------------------------------------------------
void MOS_DynaObject::ReadODB( MT_XXmlInputArchive& archive )
{
    archive.Section( "Objet" );

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
        archive.EndSection();
        MT_Vector2D vPos;
        MOS_App::GetApp().GetWorld().MosToSimMgrsCoord( strPos, vPos );
        pointVector_.push_back( vPos );
    }
    archive.EndList();

    // Compute object center.
    center_ = MT_Vector2D( 0.0, 0.0 );
    for( IT_PointVector it = pointVector_.begin(); it != pointVector_.end(); ++it )
        center_ = center_ + (*it) / (double)pointVector_.size();

    archive.EndSection();
    archive.EndSection();

    if( nType_ == EnumObjectType::site_franchissement )
    {
        bAttrSiteFranchissementPresent_ = true;
        archive.ReadField( "Largeur", nSiteFranchissementLargeur_ );
        archive.ReadField( "Profondeur", nSiteFranchissementProfondeur_ );
        archive.ReadField( "VitesseCourant", nSiteFranchissementVitesseCourant_ );
        archive.ReadField( "BergesAAmenager", nSiteFranchissementBergesAAmenager_ );
    }

    if( nType_ == EnumObjectType::nuage_nbc )
    {
        bAttrNuageNBCPresent_ = true;
        archive.Section( "AgentNBC" );
        std::string strNBCAgent;
        archive.ReadAttribute( "type", strNBCAgent );
        nNuageNBCAgentNbcID_ = (uint)-1;
        const T_MosId_String_Map& nbcMap = MOS_App::GetApp().GetNBCNames();
        for( CIT_MosId_String_Map it = nbcMap.begin(); it != nbcMap.end(); ++it )
        {
            if( it->second == strNBCAgent )
            {
                nNuageNBCAgentNbcID_ = it->first;
            }
        }
        assert( nNuageNBCAgentNbcID_ != (uint)-1 );
        archive.EndSection();
    }

    archive.EndSection();
}


// -----------------------------------------------------------------------------
// Name: MOS_DynaObject::WriteODB
/** @param  archive 
*/
// Created: APE 2004-08-31
// -----------------------------------------------------------------------------
void MOS_DynaObject::WriteODB( MT_XXmlOutputArchive& archive )
{
    archive.Section( "Objet" );

    archive.WriteAttribute( "type", ENT_Tr::ConvertFromObjectType( (E_ObjectType)nType_ ) );
    archive.WriteAttribute( "id", nID_ );
    archive.WriteField( "Armee", pTeam_->GetName() );

    archive.Section( "Forme" );
    archive.Section( "Localisation" );

    archive.WriteAttribute( "type", (MOS_Tools::ToString( nTypeLocalisation_ )).ascii() );

    archive.BeginList( "Points", pointVector_.size() );
    for( IT_PointVector it = pointVector_.begin(); it != pointVector_.end(); ++it )
    {
        archive.Section( "Point" );
        std::string strPos;
        MOS_App::GetApp().GetWorld().SimToMosMgrsCoord( (*it), strPos );
        archive << strPos;
        archive.EndSection();
    }
    archive.EndList();

    archive.EndSection();
    archive.EndSection();

    if( nType_ == EnumObjectType::site_franchissement )
    {
        archive.WriteField( "Largeur", nSiteFranchissementLargeur_ );
        archive.WriteField( "Profondeur", nSiteFranchissementProfondeur_ );
        archive.WriteField( "VitesseCourant", nSiteFranchissementVitesseCourant_ );
        archive.WriteField( "BergesAAmenager", nSiteFranchissementBergesAAmenager_ );
    }

    if( nType_ == EnumObjectType::rota )
    {
        archive.Section( "ZoneDeSecurite" );
        archive.EndSection(); // ZoneDeSecurite
        archive.WriteField( "Danger", unsigned( 3 ) );
        archive.Section( "AgentNBC" );
        archive.WriteAttribute( "type", MOS_App::GetApp().GetNBCName( nNuageNBCAgentNbcID_ ) );
        archive.EndSection(); // AgentNBC
    }

    if( nType_ == EnumObjectType::nuage_nbc )
    {
        archive.Section( "AgentNBC" );
        archive.WriteAttribute( "type", MOS_App::GetApp().GetNBCName( nNuageNBCAgentNbcID_ ) );
        archive.EndSection();
    }

    archive.EndSection();
}


// -----------------------------------------------------------------------------
// Name: MOS_DynaObject::GetIDManagerForObjectType
// Created: APE 2004-09-01
// -----------------------------------------------------------------------------
MOS_IDManager& MOS_DynaObject::GetIDManagerForObjectType( ASN1T_EnumObjectType nType )
{
    return *managers_[ objectIds_[ nType ] ];
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObject::InitializeObjectIds
// Created: AGE 2005-04-05
// -----------------------------------------------------------------------------
void MOS_DynaObject::InitializeObjectIds( MT_XXmlInputArchive& archive )
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
