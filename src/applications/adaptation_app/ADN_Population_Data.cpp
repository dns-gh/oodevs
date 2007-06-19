// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-12-02 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Population_Data.cpp $
// $Author: Ape $
// $Modtime: 7/06/05 9:51 $
// $Revision: 15 $
// $Workfile: ADN_Population_Data.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Population_Data.h"

#include "ADN_Workspace.h"
#include "ADN_Project_Data.h"
#include "ADN_OpenFile_Exception.h"
#include "ADN_DataException.h"
#include "ADN_Xml_Exception.h"
#include "ADN_XmlInput_Helper.h"
#include "ADN_SaveFile_Exception.h"

#include "ADN_Tr.h"
#include "ENT/ENT_Tr.h"

// =============================================================================
// ADN_Population_Data::FireEffectProtectionInfos
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::FireEffectProtectionInfos::FireEffectProtectionInfos
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
ADN_Population_Data::FireEffectProtectionInfos::FireEffectProtectionInfos( ADN_Categories_Data::ArmorInfos* ptr )
: ADN_Ref_ABC                ()
, ADN_DataTreeNode_ABC       ()
, strName_                   ( ptr->strName_.GetData() )
, ptrProtection_             ( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetArmorsInfos(), ptr )
, rDestruction_              ( 0. )
, rFixableWithEvacuation_    ( 0. )
, rFixableWithoutEvacuation_ ( 0. )
{
    BindExistenceTo( &ptrProtection_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::FireEffectProtectionInfos::ReadArchive
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
void ADN_Population_Data::FireEffectProtectionInfos::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.ReadField( "Destruction"            , rDestruction_              );
    input.ReadField( "ReparableAvecEvacuation", rFixableWithEvacuation_    );
    input.ReadField( "ReparableSansEvacuation", rFixableWithoutEvacuation_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::FireEffectProtectionInfos::WriteArchive
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
void ADN_Population_Data::FireEffectProtectionInfos::WriteArchive( MT_OutputArchive_ABC& output )
{
    // do not save unspecified protection hit factor
    if( rDestruction_.GetData()                 == 0.
        && rFixableWithEvacuation_.GetData()    == 0.
        && rFixableWithoutEvacuation_.GetData() == 0. )
        return;

    output.Section( "ProtectionPion" );

    output.WriteAttribute( "nom", ptrProtection_.GetData()->strName_.GetData() );

    output.WriteField( "Destruction"            , rDestruction_.GetData()              );
    output.WriteField( "ReparableAvecEvacuation", rFixableWithEvacuation_.GetData()    );
    output.WriteField( "ReparableSansEvacuation", rFixableWithoutEvacuation_.GetData() );

    output.EndSection(); // ProtectionPion
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::FireEffectProtectionInfos::~FireEffectProtectionInfos
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
ADN_Population_Data::FireEffectProtectionInfos::~FireEffectProtectionInfos()
{
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::FireEffectProtectionInfos::GetNodeName
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
std::string ADN_Population_Data::FireEffectProtectionInfos::GetNodeName()
{
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::FireEffectProtectionInfos::GetItemName
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
std::string ADN_Population_Data::FireEffectProtectionInfos::GetItemName()
{
    return std::string();
}

// =============================================================================
// ADN_Population_Data::FireEffectInfos
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::FireEffectInfos::FireEffectInfos
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
ADN_Population_Data::FireEffectInfos::FireEffectInfos( E_PopulationAttitude nAttitude )
: ADN_Ref_ABC         ()
, ADN_DataTreeNode_ABC()
, nAttitude_          ( nAttitude )
, strName_            ( ENT_Tr::ConvertFromPopulationAttitude( nAttitude ) )
, vProtectionInfos_   ( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetArmorsInfos() )
, rIntensityDensity_  ( 0. )
, rIntensityFactor_   ( 0. )
{
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::FireEffectInfos::ReadArchive
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
void ADN_Population_Data::FireEffectInfos::ReadArchive( ADN_XmlInput_Helper& input )
{
    if( !input.Section( "Intensite", ADN_XmlInput_Helper::eNothing ) )
        return;
    input.ReadAttribute( "densitePopulation", rIntensityDensity_ );
    input.ReadAttribute( "intensite"        , rIntensityFactor_  );
    input.EndSection(); // Intensite

    input.BeginList( "ProtectionsPions" );
    while( input.NextListElement() )
    {
        input.Section( "ProtectionPion" );
        std::string strProtection;
        input.ReadAttribute( "nom", strProtection );

        IT_FireEffectProtectionInfosVector itProtection = std::find_if( vProtectionInfos_.begin(), vProtectionInfos_.end(),
                                                                        ADN_Tools::NameCmp<FireEffectProtectionInfos>(strProtection) );
        assert( itProtection != vProtectionInfos_.end() );
        (*itProtection)->ReadArchive( input );
        input.EndSection(); // ProtectionPion
    }
    input.EndList(); // ProtectionsPions
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::FireEffectInfos::WriteArchive
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
void ADN_Population_Data::FireEffectInfos::WriteArchive( MT_OutputArchive_ABC& output )
{
    if( rIntensityDensity_.GetData() == 0. && rIntensityFactor_.GetData() == 0. )
        return;
    output.Section( "Attitude" );
    output.Section( "Intensite" );
    output.WriteAttribute( "densitePopulation", rIntensityDensity_.GetData() );
    output.WriteAttribute( "intensite"        , rIntensityFactor_.GetData()  );
    output.EndSection(); // Intensite

    output.WriteAttribute( "nom", ENT_Tr::ConvertFromPopulationAttitude( nAttitude_ ) );

    output.BeginList( "ProtectionsPions", vProtectionInfos_.size() );
    for( IT_FireEffectProtectionInfosVector it = vProtectionInfos_.begin(); it != vProtectionInfos_.end(); ++it )
        (*it)->WriteArchive( output );
    output.EndList(); // ProtectionsPions
    output.EndSection(); // Attitude
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::FireEffectInfos::~FireEffectInfos
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
ADN_Population_Data::FireEffectInfos::~FireEffectInfos()
{
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::FireEffectInfos::GetNodeName
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
std::string ADN_Population_Data::FireEffectInfos::GetNodeName()
{
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::FireEffectInfos::GetItemName
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
std::string ADN_Population_Data::FireEffectInfos::GetItemName()
{
    return strName_.GetData();
}

// =============================================================================
// ADN_Population_Data::FireEffectRoeInfos
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::FireEffectRoeInfos::FireEffectRoeInfos
// Created: SBO 2005-11-21
// -----------------------------------------------------------------------------
ADN_Population_Data::FireEffectRoeInfos::FireEffectRoeInfos( E_PopulationRoe nRoe )
: ADN_Ref_ABC         ()
, ADN_DataTreeNode_ABC()
, nRoe_               ( nRoe )
, strName_            ( ENT_Tr::ConvertFromPopulationRoe( nRoe ) )
, rAttritionSurface_  ( 0. )
, rPH_                ( 0. )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::FireEffectRoeInfos::~FireEffectRoeInfos
// Created: SBO 2005-11-21
// -----------------------------------------------------------------------------
ADN_Population_Data::FireEffectRoeInfos::~FireEffectRoeInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::FireEffectRoeInfos::GetNodeName
// Created: SBO 2005-11-21
// -----------------------------------------------------------------------------
std::string ADN_Population_Data::FireEffectRoeInfos::GetNodeName()
{
    return std::string();
}
    
// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::FireEffectRoeInfos::GetItemName
// Created: SBO 2005-11-21
// -----------------------------------------------------------------------------
std::string ADN_Population_Data::FireEffectRoeInfos::GetItemName()
{
    return strName_.GetData();
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::FireEffectRoeInfos::ReadArchive
// Created: SBO 2005-11-21
// -----------------------------------------------------------------------------
void ADN_Population_Data::FireEffectRoeInfos::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.ReadField( "SurfaceAttrition", rAttritionSurface_ );
    input.ReadField( "PH", rPH_ );
}
    
// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::FireEffectRoeInfos::WriteArchive
// Created: SBO 2005-11-21
// -----------------------------------------------------------------------------
void ADN_Population_Data::FireEffectRoeInfos::WriteArchive( MT_OutputArchive_ABC& output )
{
    // do not write ROE with attrition surface == 0 && PH == 0
    if( rAttritionSurface_.GetData() == 0. && rPH_.GetData() == 0. )
        return;
    output.Section( "RegleEngagementTireur" );
    output.WriteAttribute( "nom", ENT_Tr::ConvertFromPopulationRoe( nRoe_ ) );
    output.WriteField( "SurfaceAttrition", rAttritionSurface_.GetData() );
    output.WriteField( "PH", rPH_.GetData() );
    output.EndSection(); // RegleEngagementTireur
}

// =============================================================================
// ADN_Population_Data::SpeedEffectVolumeInfos
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::SpeedEffectVolumeInfos::SpeedEffectVolumeInfos
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
ADN_Population_Data::SpeedEffectVolumeInfos::SpeedEffectVolumeInfos( ADN_Categories_Data::SizeInfos* ptr )
: ADN_Ref_ABC         ()
, ADN_DataTreeNode_ABC()
, strName_            ( ptr->GetData() )
, ptrVolume_          ( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetSizesInfos(), ptr )
, rDensity_           ( 0. )
, rMaxSpeed_          ( 0. )
{
    this->BindExistenceTo( &ptrVolume_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::SpeedEffectVolumeInfos::ReadArchive
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
void ADN_Population_Data::SpeedEffectVolumeInfos::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.ReadAttribute( "densitePopulation", rDensity_  );
    input.ReadAttribute( "vitesseMaximale"  , rMaxSpeed_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::SpeedEffectVolumeInfos::WriteArchive
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
void ADN_Population_Data::SpeedEffectVolumeInfos::WriteArchive( MT_OutputArchive_ABC& output )
{
    // do not save Volume with density and speed == 0
    if( rDensity_ == 0. && rMaxSpeed_ == 0. )
        return;

    output.Section( "VolumePion" );

    output.WriteAttribute( "nom"              , ptrVolume_.GetData()->GetData() );
    output.WriteAttribute( "densitePopulation", rDensity_ .GetData() );
    output.WriteAttribute( "vitesseMaximale"  , rMaxSpeed_.GetData() );

    output.EndSection(); // VolumePion
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::SpeedEffectVolumeInfos::~SpeedEffectVolumeInfos
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
ADN_Population_Data::SpeedEffectVolumeInfos::~SpeedEffectVolumeInfos()
{
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::SpeedEffectVolumeInfos::GetNodeName
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
std::string ADN_Population_Data::SpeedEffectVolumeInfos::GetNodeName()
{
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::SpeedEffectVolumeInfos::GetItemName
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
std::string ADN_Population_Data::SpeedEffectVolumeInfos::GetItemName()
{
    return std::string();
}

// =============================================================================
// ADN_Population_Data::SpeedEffectInfos
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::SpeedEffectInfos::SpeedEffectInfos
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
ADN_Population_Data::SpeedEffectInfos::SpeedEffectInfos( E_PopulationAttitude nAttitude )
: ADN_Ref_ABC         ()
, ADN_DataTreeNode_ABC()
, nAttitude_          ( nAttitude )
, strName_            ( ENT_Tr::ConvertFromPopulationAttitude( nAttitude ) )
, vVolumeInfos_       ( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetSizesInfos() )
{
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::SpeedEffectInfos::ReadArchive
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
void ADN_Population_Data::SpeedEffectInfos::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.BeginList( "VolumesPions" );
    while( input.NextListElement() )
    {
        input.Section( "VolumePion" );
        std::string strVolume;
        input.ReadAttribute( "nom", strVolume );
        IT_SpeedEffectVolumeInfosVector itVolume = std::find_if( vVolumeInfos_.begin(), vVolumeInfos_.end(),
                                                                 ADN_Tools::NameCmp<SpeedEffectVolumeInfos>(strVolume) );
        assert( itVolume != vVolumeInfos_.end() );
        (*itVolume)->ReadArchive( input );
        input.EndSection(); // VolumePion
    }
    input.EndList(); // VolumesPions
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::SpeedEffectInfos::WriteArchive
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
void ADN_Population_Data::SpeedEffectInfos::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( "Attitude" );

    output.WriteAttribute( "nom", ENT_Tr::ConvertFromPopulationAttitude( nAttitude_ ) );

    output.BeginList( "VolumesPions", vVolumeInfos_.size() );
    for( IT_SpeedEffectVolumeInfosVector it = vVolumeInfos_.begin(); it != vVolumeInfos_.end(); ++it )
        (*it)->WriteArchive( output );
    output.EndList(); // VolumesPions

    output.EndSection(); // Attitude
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::SpeedEffectInfos::~SpeedEffectInfos
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
ADN_Population_Data::SpeedEffectInfos::~SpeedEffectInfos()
{
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::SpeedEffectInfos::GetNodeName
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
std::string ADN_Population_Data::SpeedEffectInfos::GetNodeName()
{
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::SpeedEffectInfos::GetItemName
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
std::string ADN_Population_Data::SpeedEffectInfos::GetItemName()
{
    return strName_.GetData();
}

// =============================================================================
// ADN_Population_Data::PopulationInfos
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PopulationInfos::PopulationInfos
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
ADN_Population_Data::PopulationInfos::PopulationInfos()
: ADN_Ref_ABC            ()
, ADN_DataTreeNode_ABC   ()
, ptrModel_              ( ADN_Workspace::GetWorkspace().GetModels().GetData().GetPopulationModelsInfos(), 0 )
, rConcentrationDensity_ ( 0. )
, rMoveDensity_          ( 0. )
, rMoveSpeed_            ( 0. )
, vSpeedEffectInfos_     ()
, vFireEffectInfos_      ()
, vFireEffectRoeInfos_   ()
{
    for( int i = 0; i < eNbrPopulationAttitude; ++i )
    {
        SpeedEffectInfos* pSpeedNew = new SpeedEffectInfos( ( E_PopulationAttitude )i );
        std::auto_ptr< SpeedEffectInfos > spSpeedNew( pSpeedNew );
        vSpeedEffectInfos_.AddItem( spSpeedNew.release() );

        FireEffectInfos* pFireNew = new FireEffectInfos( ( E_PopulationAttitude )i );
        std::auto_ptr< FireEffectInfos > spFireNew( pFireNew );
        vFireEffectInfos_.AddItem( spFireNew.release() );
    }
    for( int i = 0; i < eNbrPopulationRoe; ++i )
    {
        FireEffectRoeInfos* pFireRoeNew = new FireEffectRoeInfos( ( E_PopulationRoe )i );
        std::auto_ptr< FireEffectRoeInfos > spFireRoeNew( pFireRoeNew );
        vFireEffectRoeInfos_.AddItem( spFireRoeNew.release() );
    }
    this->BindExistenceTo( &ptrModel_ );
}


// -----------------------------------------------------------------------------
// Name: PopulationInfos::~PopulationInfos
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
ADN_Population_Data::PopulationInfos::~PopulationInfos()
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: PopulationInfos::GetNodeName
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
std::string ADN_Population_Data::PopulationInfos::GetNodeName()
{
    return std::string();
}


// -----------------------------------------------------------------------------
// Name: PopulationInfos::GetItemName
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
std::string ADN_Population_Data::PopulationInfos::GetItemName()
{
    return std::string();
}


// -----------------------------------------------------------------------------
// Name: PopulationInfos::CreateCopy
// Created: APE 2005-02-14
// -----------------------------------------------------------------------------
ADN_Population_Data::PopulationInfos* ADN_Population_Data::PopulationInfos::CreateCopy()
{
    PopulationInfos* pCopy = new PopulationInfos();

    pCopy->ptrModel_              = ptrModel_.GetData();
    pCopy->rConcentrationDensity_ = rConcentrationDensity_.GetData();
    pCopy->rMoveDensity_          = rMoveDensity_.GetData();
    pCopy->rMoveSpeed_            = rMoveSpeed_.GetData();

    for( int i = 0; i < eNbrPopulationAttitude; ++i )
    {
        pCopy->vSpeedEffectInfos_[ i ] = vSpeedEffectInfos_[ i ];
        pCopy->vFireEffectInfos_ [ i ] = vFireEffectInfos_ [ i ];
    }
    for( int i = 0; i < eNbrPopulationRoe; ++i )
        pCopy->vFireEffectRoeInfos_[ i ] = vFireEffectRoeInfos_[ i ];

    return pCopy;
}

// -----------------------------------------------------------------------------
// Name: PopulationInfos::ReadArchive
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
void ADN_Population_Data::PopulationInfos::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section( "Population" );
    input.ReadAttribute( "nom", strName_ );

    std::string strModel;
    input.ReadField( "ModeleDecisionnel", strModel );
    ADN_Models_Data::ModelInfos* pModel = ADN_Workspace::GetWorkspace().GetModels().GetData().FindPopulationModel( strModel );
    if( !pModel )
        throw ADN_DataException( "Donnée invalide", MT_FormatString( "Modele '%s' inconnu - réferencé par la population '%s'", strModel.c_str(), strName_.GetData().c_str() ), "" );

    ptrModel_ = pModel;

    input.ReadField( "DensiteConcentration"      , rConcentrationDensity_ );
    input.ReadField( "DensiteNominaleDeplacement", rMoveDensity_          );
    input.ReadField( "VitesseDeplacement"        , rMoveSpeed_            );

    input.Section( "Effets" );

    input.Section( "Ralentissement" );
    if( input.BeginList( "Attitudes", ADN_XmlInput_Helper::eNothing ) )
    {
        while( input.NextListElement() )
            if( input.Section( "Attitude", ADN_XmlInput_Helper::eNothing ) )
            {
                std::string strAttitude;
                input.ReadAttribute( "nom", strAttitude );
                uint nAttitude = ENT_Tr::ConvertToPopulationAttitude( strAttitude );
                if( nAttitude == (E_PopulationAttitude)-1 )
                    throw ADN_DataException( tr( "Data error" ).ascii(), tr( "Invalid population attitude." ).ascii() );
                vSpeedEffectInfos_[ nAttitude ]->ReadArchive( input );
                input.EndSection(); // Attitude
            }
        input.EndList(); // Attitudes
    }
    input.EndSection(); // Ralentissement

    input.Section( "Tir" );
    input.Section( "Tireur" );
    if( input.BeginList( "Attitudes", ADN_XmlInput_Helper::eNothing ) )
    {
        while( input.NextListElement() )
            if( input.Section( "Attitude", ADN_XmlInput_Helper::eNothing ) )
            {
                std::string strAttitude;
                input.ReadAttribute( "nom", strAttitude );
                uint nAttitude = ENT_Tr::ConvertToPopulationAttitude( strAttitude );
                if( nAttitude == (E_PopulationAttitude)-1 )
                    throw ADN_DataException( tr( "Data error" ).ascii(), tr( "Invalid population attitude." ).ascii() );
                vFireEffectInfos_[ nAttitude ]->ReadArchive( input );
                input.EndSection(); // Attitude
            }
        input.EndList(); // Attitudes
    }
    input.EndSection(); // Tireur

    input.Section( "Cible" );
    if( input.BeginList( "ReglesEngagementTireur", ADN_XmlInput_Helper::eNothing ) )
    {
        while( input.NextListElement() )
            if( input.Section( "RegleEngagementTireur", ADN_XmlInput_Helper::eNothing ) )
            {
                std::string strROE;
                input.ReadAttribute( "nom", strROE );
                uint nROE = ENT_Tr::ConvertToPopulationRoe( strROE );
                if( nROE == (E_PopulationRoe)-1 )
                    throw ADN_DataException( tr( "Data error" ).ascii(), tr( "Invalid population ROE." ).ascii() );
                vFireEffectRoeInfos_[ nROE ]->ReadArchive( input );
                input.EndSection(); // RegleEngagementTireur
            }
        input.EndList(); // ReglesEngagementTireur
    }
    input.EndSection(); // Cible

    input.EndSection(); // Tir
    input.EndSection(); // Effets
    input.EndSection(); // Population
}


// -----------------------------------------------------------------------------
// Name: PopulationInfos::WriteArchive
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
void ADN_Population_Data::PopulationInfos::WriteArchive( MT_OutputArchive_ABC& output, int nMosId )
{
    output.Section( "Population" );
    output.WriteAttribute( "nom", strName_.GetData() );

    output.WriteField( "MosID", nMosId );
    output.WriteField( "ModeleDecisionnel", ptrModel_.GetData()->strName_.GetData() );

    output.WriteField( "DensiteConcentration"      , rConcentrationDensity_.GetData() );
    output.WriteField( "DensiteNominaleDeplacement", rMoveDensity_.GetData()          );
    output.WriteField( "VitesseDeplacement"        , rMoveSpeed_.GetData()            );

    output.Section( "Effets" );

    output.Section( "Ralentissement" );
    output.BeginList( "Attitudes", vSpeedEffectInfos_.size() );
    for( IT_SpeedEffectInfosVector it = vSpeedEffectInfos_.begin(); it != vSpeedEffectInfos_.end(); ++it )
        ( *it )->WriteArchive( output );
    output.EndList(); // Attitudes
    output.EndSection(); // Ralentissement

    output.Section( "Tir" );
    output.Section( "Tireur" );
    output.BeginList( "Attitudes", vFireEffectInfos_.size() );
    for( IT_FireEffectInfosVector it = vFireEffectInfos_.begin(); it != vFireEffectInfos_.end(); ++it )
        ( *it )->WriteArchive( output );
    output.EndList(); // Attitudes
    output.EndSection(); // Tireur

    output.Section( "Cible" );
    output.BeginList( "ReglesEngagementTireur", vFireEffectRoeInfos_.size() );
    for( IT_FireEffectRoeInfosVector it = vFireEffectRoeInfos_.begin(); it != vFireEffectRoeInfos_.end(); ++it )
        ( *it )->WriteArchive( output );
    output.EndList(); // ReglesEngagementTireur
    output.EndSection(); // Cible

    output.EndSection(); // Tir
    output.EndSection(); // Effets
    output.EndSection(); // Population
}

// =============================================================================
// ADN_Population_Data::ReloadingSpeedEffectInfos
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::ReloadingSpeedEffectInfos::ReloadingSpeedEffectInfos
// Created: SBO 2005-11-14
// -----------------------------------------------------------------------------
ADN_Population_Data::ReloadingSpeedEffectInfos::ReloadingSpeedEffectInfos()
: ADN_Ref_ABC ()
, rDensity_   ( 0. )
, rModifier_  ( 0. )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::ReloadingSpeedEffectInfos::~ReloadingSpeedEffectInfos
// Created: SBO 2005-11-14
// -----------------------------------------------------------------------------
ADN_Population_Data::ReloadingSpeedEffectInfos::~ReloadingSpeedEffectInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::ReloadingSpeedEffectInfos::ReadArchive
// Created: SBO 2005-11-14
// -----------------------------------------------------------------------------
void ADN_Population_Data::ReloadingSpeedEffectInfos::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section( "TempsRechargement" );
    input.ReadAttribute( "densitePopulation", rDensity_  );
    input.ReadAttribute( "modificateur"     , rModifier_ );
    input.EndSection(); // TempsRechargement
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::ReloadingSpeedEffectInfos::WriteArchive
// Created: SBO 2005-11-14
// -----------------------------------------------------------------------------
void ADN_Population_Data::ReloadingSpeedEffectInfos::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( "TempsRechargement" );
    output.WriteAttribute( "densitePopulation", rDensity_.GetData()  );
    output.WriteAttribute( "modificateur"     , rModifier_.GetData() );
    output.EndSection(); // TempsRechargement
}


// =============================================================================
// ADN_Population_Data
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data constructor
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
ADN_Population_Data::ADN_Population_Data()
: ADN_Data_ABC              ()
, vPopulation_              ()
, reloadingSpeedEffectInfos_()
{
}


// -----------------------------------------------------------------------------
// Name: ADN_Population_Data destructor
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
ADN_Population_Data::~ADN_Population_Data()
{
    this->Reset();
}


// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::FilesNeeded
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
void ADN_Population_Data::FilesNeeded( T_StringList& vFiles ) const
{
    vFiles.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szPopulation_.GetData() );
}


// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::Reset
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
void ADN_Population_Data::Reset()
{
    vPopulation_.Reset();
}


// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::ReadArchive
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
void ADN_Population_Data::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.BeginList( "Populations" );

    input.Section( "Effets" );
    reloadingSpeedEffectInfos_.ReadArchive( input );
    input.EndSection(); // Effets

    while( input.NextListElement() )
    {
        std::auto_ptr<PopulationInfos> spNew( new PopulationInfos() );
        spNew->ReadArchive( input );
        vPopulation_.AddItem( spNew.release() );
    }
    input.EndList(); // Populations
}


// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::WriteArchive
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
void ADN_Population_Data::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.BeginList( "Populations", vPopulation_.size() );

    output.Section( "Effets" );
    reloadingSpeedEffectInfos_.WriteArchive( output );
    output.EndSection(); // Effets

    int n = 0;
    for( IT_PopulationInfosVector it = vPopulation_.begin(); it != vPopulation_.end(); ++it, ++n )
    {
        (*it)->WriteArchive( output, n );
    }
    output.EndList(); // Automates
}


// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::GetPopulationsThatUse
// Created: SBO 2006-09-11
// -----------------------------------------------------------------------------
std::string ADN_Population_Data::GetPopulationsThatUse( ADN_Models_Data::ModelInfos& model )
{
    std::string result;
    for( IT_PopulationInfosVector it = vPopulation_.begin(); it != vPopulation_.end(); ++it )
    {
        if( (*it)->ptrModel_.GetData() == &model )
        {
            if( result != "" )
                result += "<br>";
            result += "<nobr>" + (*it)->strName_.GetData() + "</nobr>";
        }
    }
    return result;
}
