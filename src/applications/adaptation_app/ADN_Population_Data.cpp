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
#include "ADN_SaveFile_Exception.h"
#include "ADN_Tr.h"
#include "ENT/ENT_Tr.h"

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::FireEffectProtectionInfos::FireEffectProtectionInfos
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
ADN_Population_Data::FireEffectProtectionInfos::FireEffectProtectionInfos( helpers::ArmorInfos* ptr )
    : ADN_Ref_ABC()
    , ADN_DataTreeNode_ABC()
    , strName_                  ( ptr->strName_.GetData() )
    , ptrProtection_            ( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetArmorsInfos(), ptr )
    , rUnarmedDestruction_              ( 0. )
    , rUnarmedFixableWithEvacuation_    ( 0. )
    , rUnarmedFixableWithoutEvacuation_ ( 0. )
    , rArmedDestruction_                ( 0. )
    , rArmedFixableWithEvacuation_      ( 0. )
    , rArmedFixableWithoutEvacuation_   ( 0. )
{
    BindExistenceTo( &ptrProtection_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::FireEffectProtectionInfos::ReadArchive
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
void ADN_Population_Data::FireEffectProtectionInfos::ReadArchive( xml::xistream& input )
{
    double rArmedDestruction = 0.0, rArmedFixableWithEvacuation = 0.0, rArmedFixableWithoutEvacuation = 0.0;
    double rUnarmedDestruction = 0.0, rUnarmedFixableWithEvacuation = 0.0, rUnarmedFixableWithoutEvacuation = 0.0;

    input >> xml::start( "armed" )
              >> xml::attribute( "destruction", rArmedDestruction )
              >> xml::attribute( "repairable-with-evacuation", rArmedFixableWithEvacuation )
              >> xml::attribute( "repairable-without-evacuation", rArmedFixableWithoutEvacuation )
          >> xml::end;

    input >> xml::start( "unarmed" )
              >> xml::attribute( "destruction", rUnarmedDestruction )
              >> xml::attribute( "repairable-with-evacuation", rUnarmedFixableWithEvacuation )
              >> xml::attribute( "repairable-without-evacuation", rUnarmedFixableWithoutEvacuation )
          >> xml::end;

    rArmedDestruction_                  = 100.0 * rArmedDestruction;
    rArmedFixableWithEvacuation_        = 100.0 * rArmedFixableWithEvacuation;
    rArmedFixableWithoutEvacuation_     = 100.0 * rArmedFixableWithoutEvacuation;
    rUnarmedDestruction_                = 100.0 * rUnarmedDestruction;
    rUnarmedFixableWithEvacuation_      = 100.0 * rUnarmedFixableWithEvacuation;
    rUnarmedFixableWithoutEvacuation_   = 100.0 * rUnarmedFixableWithoutEvacuation;
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::FireEffectProtectionInfos::WriteArchive
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
void ADN_Population_Data::FireEffectProtectionInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "protection" )
                << xml::attribute( "name", ptrProtection_.GetData()->strName_ )
                << xml::start( "unarmed" )
                    << xml::attribute( "destruction",                   rUnarmedDestruction_.GetData() /100.0 )
                    << xml::attribute( "repairable-with-evacuation",    rUnarmedFixableWithEvacuation_.GetData() /100.0 )
                    << xml::attribute( "repairable-without-evacuation", rUnarmedFixableWithoutEvacuation_.GetData() /100.0 )
                << xml::end
                << xml::start( "armed" )
                    << xml::attribute( "destruction",                   rArmedDestruction_.GetData() /100.0 )
                    << xml::attribute( "repairable-with-evacuation",    rArmedFixableWithEvacuation_.GetData() /100.0 )
                    << xml::attribute( "repairable-without-evacuation", rArmedFixableWithoutEvacuation_.GetData() /100.0 )
                << xml::end
            << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::FireEffectProtectionInfos::~FireEffectProtectionInfos
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
ADN_Population_Data::FireEffectProtectionInfos::~FireEffectProtectionInfos()
{
    // NOTHING
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

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::FireEffectInfos::FireEffectInfos
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
ADN_Population_Data::FireEffectInfos::FireEffectInfos( E_PopulationAttitude nAttitude )
: ADN_Ref_ABC()
, ADN_DataTreeNode_ABC()
, nAttitude_        ( nAttitude )
, strName_          ( ENT_Tr::ConvertFromPopulationAttitude( nAttitude, ENT_Tr::eToTr ) )
, vProtectionInfos_ ( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetArmorsInfos() )
, rIntensityDensity_( 0. )
, rIntensityFactor_ ( 0. )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::FireEffectInfos::ReadArchive
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
void ADN_Population_Data::FireEffectInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "population-density", rIntensityDensity_ )
          >> xml::attribute( "intensity", rIntensityFactor_ )
          >> xml::list( "protection", *this, &ADN_Population_Data::FireEffectInfos::ReadProtection );
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::FireEffectInfos::ReadProtection
// Created: AGE 2007-08-16
// -----------------------------------------------------------------------------
void ADN_Population_Data::FireEffectInfos::ReadProtection( xml::xistream& input )
{
    std::string strProtection;
    input >> xml::attribute( "name", strProtection );
    IT_FireEffectProtectionInfosVector itProtection = std::find_if( vProtectionInfos_.begin(), vProtectionInfos_.end(),
                                                                    ADN_Tools::NameCmp<FireEffectProtectionInfos>(strProtection) );
    assert( itProtection != vProtectionInfos_.end() );
    (*itProtection)->ReadArchive( input );
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::FireEffectInfos::WriteArchive
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
void ADN_Population_Data::FireEffectInfos::WriteArchive( xml::xostream& output ) const
{
    if( rIntensityDensity_.GetData() == 0. && rIntensityFactor_.GetData() == 0. )
        return;
    output << xml::start( "attrition-effect" )
             << xml::attribute( "population-density", rIntensityDensity_ )
             << xml::attribute( "intensity", rIntensityFactor_ )
             << xml::attribute( "population-attitude", ENT_Tr::ConvertFromPopulationAttitude( nAttitude_ ) );
    for( CIT_FireEffectProtectionInfosVector it = vProtectionInfos_.begin(); it != vProtectionInfos_.end(); ++it )
        (*it)->WriteArchive( output );
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::FireEffectInfos::~FireEffectInfos
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
ADN_Population_Data::FireEffectInfos::~FireEffectInfos()
{
    // NOTHING
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

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::FireEffectRoeInfos::FireEffectRoeInfos
// Created: SBO 2005-11-21
// -----------------------------------------------------------------------------
ADN_Population_Data::FireEffectRoeInfos::FireEffectRoeInfos( E_PopulationRoe nRoe )
    : ADN_Ref_ABC()
    , ADN_DataTreeNode_ABC()
    , nRoe_             ( nRoe )
    , strName_          ( ENT_Tr::ConvertFromPopulationRoe( nRoe ) )
    , rAttritionSurface_( 0. )
    , rPH_              ( 0. )
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
// Name: ADN_Population_Data::GetRoe
// Created: LGY 2011-02-15
// -----------------------------------------------------------------------------
unsigned int ADN_Population_Data::FireEffectRoeInfos::GetRoe()
{
    return nRoe_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::FireEffectRoeInfos::ReadArchive
// Created: SBO 2005-11-21
// -----------------------------------------------------------------------------
void ADN_Population_Data::FireEffectRoeInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "attrition-surface", rAttritionSurface_ )
          >> xml::attribute( "ph", rPH_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::FireEffectRoeInfos::WriteArchive
// Created: SBO 2005-11-21
// -----------------------------------------------------------------------------
void ADN_Population_Data::FireEffectRoeInfos::WriteArchive( xml::xostream& output ) const
{
    if( rAttritionSurface_.GetData() == 0. && rPH_.GetData() == 0. )
        return;
    output << xml::start( "unit" )
             << xml::attribute( "rule-of-engagment", ENT_Tr::ConvertFromPopulationRoe( nRoe_ ) )
             << xml::attribute( "attrition-surface", rAttritionSurface_ )
             << xml::attribute( "ph", rPH_ )
           << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::SpeedEffectVolumeInfos::SpeedEffectVolumeInfos
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
ADN_Population_Data::SpeedEffectVolumeInfos::SpeedEffectVolumeInfos( ADN_Categories_Data::SizeInfos* ptr )
    : ADN_Ref_ABC()
    , ADN_DataTreeNode_ABC()
    , strName_  ( ptr->GetData() )
    , ptrVolume_( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetSizesInfos(), ptr )
    , rDensity_ ( 0. )
    , rMaxSpeed_( 0. )
{
    this->BindExistenceTo( &ptrVolume_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::SpeedEffectVolumeInfos::ReadArchive
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
void ADN_Population_Data::SpeedEffectVolumeInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "population-density", rDensity_  )
          >> xml::attribute( "max-speed", rMaxSpeed_  );
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::SpeedEffectVolumeInfos::WriteArchive
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
void ADN_Population_Data::SpeedEffectVolumeInfos::WriteArchive( xml::xostream& output ) const
{
    if( rDensity_ == 0. && rMaxSpeed_ == 0. )
        return;
    output << xml::start( "unit" )
             << xml::attribute( "unit-size", *ptrVolume_.GetData() )
             << xml::attribute( "population-density", rDensity_  )
             << xml::attribute( "max-speed", rMaxSpeed_  )
           << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::SpeedEffectVolumeInfos::~SpeedEffectVolumeInfos
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
ADN_Population_Data::SpeedEffectVolumeInfos::~SpeedEffectVolumeInfos()
{
    // NOTHING
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
    : ADN_Ref_ABC()
    , ADN_DataTreeNode_ABC()
    , nAttitude_   ( nAttitude )
    , strName_     ( ENT_Tr::ConvertFromPopulationAttitude( nAttitude, ENT_Tr::eToTr ) )
    , vVolumeInfos_( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetSizesInfos() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::SpeedEffectInfos::ReadArchive
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
void ADN_Population_Data::SpeedEffectInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::list( "unit", *this, &ADN_Population_Data::SpeedEffectInfos::ReadSpeedEffect );
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::ReadSpeedEffect
// Created: AGE 2007-08-16
// -----------------------------------------------------------------------------
void ADN_Population_Data::SpeedEffectInfos::ReadSpeedEffect( xml::xistream& input )
{
    std::string strVolume;
    input >> xml::attribute( "unit-size", strVolume );
    IT_SpeedEffectVolumeInfosVector itVolume = std::find_if( vVolumeInfos_.begin(), vVolumeInfos_.end(),
                                                                ADN_Tools::NameCmp<SpeedEffectVolumeInfos>(strVolume) );
    assert( itVolume != vVolumeInfos_.end() );
    (*itVolume)->ReadArchive( input );
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::SpeedEffectInfos::WriteArchive
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
void ADN_Population_Data::SpeedEffectInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "slowing-effect" )
             << xml::attribute( "population-attitude" ,ENT_Tr::ConvertFromPopulationAttitude( nAttitude_ ) );
    for( CIT_SpeedEffectVolumeInfosVector it = vVolumeInfos_.begin(); it != vVolumeInfos_.end(); ++it )
        (*it)->WriteArchive( output );
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::SpeedEffectInfos::~SpeedEffectInfos
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
ADN_Population_Data::SpeedEffectInfos::~SpeedEffectInfos()
{
    // NOTHING
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
// ADN_Population_Data::UrbanEffectInfos
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::UrbanEffectInfos::UrbanEffectInfos
// Created: MMC 2011-03-30
// -----------------------------------------------------------------------------
ADN_Population_Data::UrbanEffectInfos::UrbanEffectInfos( E_PopulationAttitude nAttitude )
: ADN_Ref_ABC()
, ADN_DataTreeNode_ABC()
, nAttitude_   ( nAttitude )
, strName_     ( ENT_Tr::ConvertFromPopulationAttitude( nAttitude, ENT_Tr::eToTr ) )
, rDensity_    ( 0.0 )
, rTime_       ( "0s" )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::UrbanEffectInfos::~UrbanEffectInfos
// Created: MMC 2011-03-30
// -----------------------------------------------------------------------------
ADN_Population_Data::UrbanEffectInfos::~UrbanEffectInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::UrbanEffectInfos::GetNodeName
// Created: MMC 2011-03-30
// -----------------------------------------------------------------------------
std::string ADN_Population_Data::UrbanEffectInfos::GetNodeName()
{
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::UrbanEffectInfos::GetItemName
// Created: MMC 2011-03-30
// -----------------------------------------------------------------------------
std::string ADN_Population_Data::UrbanEffectInfos::GetItemName()
{
    return strName_.GetData();
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::UrbanEffectInfos::ReadArchive
// Created: MMC 2011-03-30
// -----------------------------------------------------------------------------
void ADN_Population_Data::UrbanEffectInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "density", rDensity_ )
        >> xml::attribute( "time", rTime_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::UrbanEffectInfos::WriteArchive
// Created: MMC 2011-03-30
// -----------------------------------------------------------------------------
void ADN_Population_Data::UrbanEffectInfos::WriteArchive( xml::xostream& output ) const
{
    output  << xml::start( "urban-destruction-effect" )
                << xml::attribute( "attitude", ENT_Tr::ConvertFromPopulationAttitude( nAttitude_ ) )
                << xml::attribute( "density", rDensity_ )
                << xml::attribute( "time", rTime_ )
            << xml::end;
}

// =============================================================================
// ADN_Population_Data::PopulationInfos
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PopulationInfos::PopulationInfos
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
ADN_Population_Data::PopulationInfos::PopulationInfos()
    : ADN_Ref_ABC()
    , ADN_DataTreeNode_ABC()
    , ptrModel_             ( ADN_Workspace::GetWorkspace().GetModels().GetData().GetPopulationModelsInfos(), 0 )
    , rConcentrationDensity_( 0. )
    , rMoveDensity_         ( 0. )
    , rMoveSpeed_           ( 0. )
    , repartition_          ( tools::translate( "Population_Data", "Crowd" ) )
    , armedIndividuals_     ( 0 )
    , decontaminationDelay_ ( "300s" )
    , vSpeedEffectInfos_    ()
    , vFireEffectInfos_     ()
    , vFireEffectRoeInfos_  ()
{
    for( int i = 0; i < eNbrPopulationAttitude; ++i )
    {
        SpeedEffectInfos* pSpeedNew = new SpeedEffectInfos( ( E_PopulationAttitude )i );
        std::auto_ptr< SpeedEffectInfos > spSpeedNew( pSpeedNew );
        vSpeedEffectInfos_.AddItem( spSpeedNew.release() );

        FireEffectInfos* pFireNew = new FireEffectInfos( ( E_PopulationAttitude )i );
        std::auto_ptr< FireEffectInfos > spFireNew( pFireNew );
        vFireEffectInfos_.AddItem( spFireNew.release() );

        UrbanEffectInfos* pUrbanNew = new UrbanEffectInfos( ( E_PopulationAttitude )i );
        std::auto_ptr< UrbanEffectInfos > spUrbanNew( pUrbanNew );
        vUrbanEffectInfos_.AddItem( spUrbanNew.release() );
    }
    for( int i = 2; i < eNbrPopulationRoe; ++i )
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
    return strName_.GetData();
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
    pCopy->repartition_           = repartition_;
    pCopy->armedIndividuals_      = armedIndividuals_.GetData();
    pCopy->decontaminationDelay_  = decontaminationDelay_.GetData();

    for( int i = 0; i < eNbrPopulationAttitude; ++i )
    {
        pCopy->vSpeedEffectInfos_[ i ] = vSpeedEffectInfos_[ i ];
        pCopy->vFireEffectInfos_ [ i ] = vFireEffectInfos_ [ i ];
    }
    for( unsigned int i = 0; i < vFireEffectRoeInfos_.size(); ++i )
        pCopy->vFireEffectRoeInfos_[ i ] = vFireEffectRoeInfos_[ i ];

    return pCopy;
}

// -----------------------------------------------------------------------------
// Name: PopulationInfos::ReadArchive
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
void ADN_Population_Data::PopulationInfos::ReadArchive( xml::xistream& input )
{
    std::string strModel;
    double rArmedIndividuals = 0.;
    input >> xml::attribute( "name", strName_ )
          >> xml::attribute( "concentration-density", rConcentrationDensity_ )
          >> xml::attribute( "moving-base-density", rMoveDensity_ )
          >> xml::attribute( "moving-speed", rMoveSpeed_ )
          >> xml::attribute( "decisional-model", strModel )
          >> xml::optional
          >> xml::attribute( "armed-individuals", rArmedIndividuals );
    ADN_Models_Data::ModelInfos* pModel = ADN_Workspace::GetWorkspace().GetModels().GetData().FindPopulationModel( strModel );
    if( !pModel )
        throw ADN_DataException( "Invalid data", tools::translate( "Population_Data", "Crowd types - Invalid behavior model '%1'" ).arg( strModel.c_str() ).ascii() );
    ptrModel_ = pModel;

    input >> xml::start( "slowing-effects" )
            >> xml::list( "slowing-effect", *this, &ADN_Population_Data::PopulationInfos::ReadSlowingEffect )
          >> xml::end
          >> xml::start( "attrition-effects" )
            >> xml::list( "attrition-effect", *this, &ADN_Population_Data::PopulationInfos::ReadAttritionEffect )
          >> xml::end
          >> xml::start( "unit-fire-effects" )
            >> xml::list( "unit", *this, &ADN_Population_Data::PopulationInfos::ReadFireEffect )
          >> xml::end
          >> xml::start( "urban-destruction-effects" )
            >> xml::list( "urban-destruction-effect", *this, &ADN_Population_Data::PopulationInfos::ReadUrbanEffect )
          >> xml::end
          >> xml::start( "nbc" )
            >> xml::attribute( "decontaminationDelay", decontaminationDelay_ ) 
          >> xml::end;
    if( input.has_child("repartition") )
    {
        input >> xml::start( "repartition" );
        repartition_.ReadArchive( input );
        input >> xml::end;
    }

    armedIndividuals_ = static_cast< int >( rArmedIndividuals * 100 );
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::PopulationInfos::ReadSlowingEffect
// Created: AGE 2007-08-16
// -----------------------------------------------------------------------------
void ADN_Population_Data::PopulationInfos::ReadSlowingEffect( xml::xistream& input )
{
    std::string strAttitude;
    input >> xml::attribute( "population-attitude", strAttitude );
    uint nAttitude = ENT_Tr::ConvertToPopulationAttitude( strAttitude );
    if( nAttitude == (E_PopulationAttitude)-1 )
        throw ADN_DataException( "Invalid data",tools::translate( "Population_Data",  "Crowd types - Invalid crowd attitude '%1'" ).arg( strAttitude.c_str() ).ascii() );
    vSpeedEffectInfos_[ nAttitude ]->ReadArchive( input );
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::PopulationInfos::ReadAttritionEffect
// Created: AGE 2007-08-16
// -----------------------------------------------------------------------------
void ADN_Population_Data::PopulationInfos::ReadAttritionEffect( xml::xistream& input )
{
    std::string strAttitude = input.attribute< std::string >( "population-attitude" );
    uint nAttitude = ENT_Tr::ConvertToPopulationAttitude( strAttitude );
    if( nAttitude == (E_PopulationAttitude)-1 )
        throw ADN_DataException( "Invalid data", tools::translate( "Population_Data",  "Crowd types - Invalid crowd attitude '%1'" ).arg( strAttitude.c_str() ).ascii() );
    vFireEffectInfos_[ nAttitude ]->ReadArchive( input );
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::PopulationInfos::ReadFireEffect
// Created: AGE 2007-08-16
// -----------------------------------------------------------------------------
void ADN_Population_Data::PopulationInfos::ReadFireEffect( xml::xistream& input )
{
    std::string strROE;
    input >> xml::attribute( "rule-of-engagment", strROE );
    uint nROE = ENT_Tr::ConvertToPopulationRoe( strROE );
    if( nROE == (E_PopulationRoe)-1 )
        throw ADN_DataException( "Invalid data", tools::translate( "Population_Data", "Crowd types - Invalid crowd 'rules of engagment' '%1'" ).arg( strROE.c_str() ).ascii() );
    for( IT_FireEffectRoeInfosVector it = vFireEffectRoeInfos_.begin(); it != vFireEffectRoeInfos_.end(); ++it )
        if( ( *it )->GetRoe() == nROE )
            ( *it )->ReadArchive( input );
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::PopulationInfos::ReadUrbanEffect
// Created: MMC 2011-03-30
// -----------------------------------------------------------------------------
void ADN_Population_Data::PopulationInfos::ReadUrbanEffect( xml::xistream& input )
{
    std::string strAttitude = input.attribute< std::string >( "attitude" );
    uint nAttitude = ENT_Tr::ConvertToPopulationAttitude( strAttitude );
    if( nAttitude == (E_PopulationAttitude)-1 )
        throw ADN_DataException( "Invalid data", tools::translate( "Population_Data",  "Crowd types - Invalid crowd attitude '%1'" ).arg( strAttitude.c_str() ).ascii() );

    vUrbanEffectInfos_[ nAttitude ]->ReadArchive( input );
}

// -----------------------------------------------------------------------------
// Name: PopulationInfos::WriteArchive
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
void ADN_Population_Data::PopulationInfos::WriteArchive( xml::xostream& output, int nMosId ) const
{
    repartition_.CheckNoError( "ADN_Population_Data" );

    output << xml::start( "population" )
            << xml::attribute( "name", strName_ )
            << xml::attribute( "id", nMosId )
            << xml::attribute( "decisional-model", ptrModel_.GetData()->strName_ )
            << xml::attribute( "concentration-density", rConcentrationDensity_ )
            << xml::attribute( "moving-base-density", rMoveDensity_ )
            << xml::attribute( "moving-speed", rMoveSpeed_ )
            << xml::attribute( "armed-individuals", armedIndividuals_.GetData() / 100. );

    output << xml::start( "slowing-effects" );
    for( CIT_SpeedEffectInfosVector it = vSpeedEffectInfos_.begin(); it != vSpeedEffectInfos_.end(); ++it )
        ( *it )->WriteArchive( output );
    output << xml::end;

    output << xml::start( "attrition-effects" );
    for( CIT_FireEffectInfosVector it = vFireEffectInfos_.begin(); it != vFireEffectInfos_.end(); ++it )
        ( *it )->WriteArchive( output );
    output << xml::end;

    output << xml::start( "unit-fire-effects" );
    for( CIT_FireEffectRoeInfosVector it = vFireEffectRoeInfos_.begin(); it != vFireEffectRoeInfos_.end(); ++it )
        ( *it )->WriteArchive( output );
    output << xml::end;

    output << xml::start( "urban-destruction-effects" );
    for( CIT_UrbanEffectInfosVector it = vUrbanEffectInfos_.begin(); it != vUrbanEffectInfos_.end(); ++it )
        ( *it )->WriteArchive( output );
    output << xml::end;

    output << xml::start( "repartition" );
    repartition_.WriteArchive( output );
    output  << xml::end;

    output << xml::start( "nbc" )
               << xml::attribute( "decontaminationDelay", decontaminationDelay_.GetData() )
           << xml::end;

    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::ReloadingSpeedEffectInfos::ReloadingSpeedEffectInfos
// Created: SBO 2005-11-14
// -----------------------------------------------------------------------------
ADN_Population_Data::ReloadingSpeedEffectInfos::ReloadingSpeedEffectInfos()
    : ADN_Ref_ABC()
    , rDensity_ ( 0. )
    , rModifier_( 0. )
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
void ADN_Population_Data::ReloadingSpeedEffectInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::start( "reloading-time-effect" )
            >> xml::attribute( "population-density", rDensity_ )
            >> xml::attribute( "modifier", rModifier_ )
          >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::ReloadingSpeedEffectInfos::WriteArchive
// Created: SBO 2005-11-14
// -----------------------------------------------------------------------------
void ADN_Population_Data::ReloadingSpeedEffectInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "reloading-time-effect" )
            << xml::attribute( "population-density", rDensity_ )
            << xml::attribute( "modifier", rModifier_ )
          << xml::end;
}
// -----------------------------------------------------------------------------
// Name: ADN_Population_Data constructor
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
ADN_Population_Data::ADN_Population_Data()
    : ADN_Data_ABC()
    , vPopulation_              ()
    , reloadingSpeedEffectInfos_()
    , timeBetweenNbcApplication_ ( "1h" )
{
    // NOTHING
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
void ADN_Population_Data::ReadArchive( xml::xistream& input )
{
    std::string time = "1h";
    input >> xml::start( "populations" )
          >> xml::optional >> xml::start( "time-between-nbc-applications")
          >> xml::attribute( "delay", time ) >> xml::end;
    reloadingSpeedEffectInfos_.ReadArchive( input );
    timeBetweenNbcApplication_ = time;
    input >> xml::list( "population", *this, &ADN_Population_Data::ReadPopulation )
          >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::ReadPopulation
// Created: AGE 2007-08-16
// -----------------------------------------------------------------------------
void ADN_Population_Data::ReadPopulation( xml::xistream& input )
{
    std::auto_ptr<PopulationInfos> spNew( new PopulationInfos() );
    spNew->ReadArchive( input );
    vPopulation_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::WriteArchive
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
void ADN_Population_Data::WriteArchive( xml::xostream& output )
{
    output << xml::start( "populations" );
    ADN_Tools::AddSchema( output, "Crowds" );
    reloadingSpeedEffectInfos_.WriteArchive( output );
    output << xml::start( "time-between-nbc-applications" )
            << xml::attribute( "delay", timeBetweenNbcApplication_.GetData() )
           << xml::end;
    int n = 0;
    for( CIT_PopulationInfosVector it = vPopulation_.begin(); it != vPopulation_.end(); ++it, ++n )
        (*it)->WriteArchive( output, n );
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::GetPopulationsThatUse
// Created: SBO 2006-09-11
// -----------------------------------------------------------------------------
QStringList ADN_Population_Data::GetPopulationsThatUse( ADN_Models_Data::ModelInfos& model )
{
    QStringList result;
    for( IT_PopulationInfosVector it = vPopulation_.begin(); it != vPopulation_.end(); ++it )
        if( (*it)->ptrModel_.GetData() == &model )
            result << (*it)->strName_.GetData().c_str();
    return result;
}
