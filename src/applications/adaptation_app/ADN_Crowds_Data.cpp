// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Crowds_Data.h"
#include "ADN_Categories_Data.h"
#include "ADN_Workspace.h"
#include "ADN_Project_Data.h"
#include "ADN_Tr.h"
#include "ENT/ENT_Tr.h"
#include "ADN_WorkspaceElement.h"

tools::IdManager ADN_Crowds_Data::idManager_;

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_Data::FireEffectProtectionInfos::FireEffectProtectionInfos
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
ADN_Crowds_Data::FireEffectProtectionInfos::FireEffectProtectionInfos( ADN_Armors_Data::ArmorInfos* ptr )
    : ADN_CrossedRef( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetElement< ADN_Armors_Data >( eArmors ).GetArmorsInfos(), ptr, true )
    , rUnarmedDestruction_( 0. )
    , rUnarmedFixableWithEvacuation_( 0. )
    , rUnarmedFixableWithoutEvacuation_ ( 0. )
    , rArmedDestruction_( 0. )
    , rArmedFixableWithEvacuation_( 0. )
    , rArmedFixableWithoutEvacuation_( 0. )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_Data::FireEffectProtectionInfos::ReadArchive
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
void ADN_Crowds_Data::FireEffectProtectionInfos::ReadArchive( xml::xistream& input )
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

    rArmedDestruction_                = 100.0 * rArmedDestruction;
    rArmedFixableWithEvacuation_      = 100.0 * rArmedFixableWithEvacuation;
    rArmedFixableWithoutEvacuation_   = 100.0 * rArmedFixableWithoutEvacuation;
    rUnarmedDestruction_              = 100.0 * rUnarmedDestruction;
    rUnarmedFixableWithEvacuation_    = 100.0 * rUnarmedFixableWithEvacuation;
    rUnarmedFixableWithoutEvacuation_ = 100.0 * rUnarmedFixableWithoutEvacuation;
}

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_Data::FireEffectProtectionInfos::WriteArchive
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
void ADN_Crowds_Data::FireEffectProtectionInfos::WriteArchive( xml::xostream& output ) const
{
    if( !GetCrossedElement() )
        return;
    output << xml::start( "protection" )
                << xml::attribute( "name", *this )
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
// Name: ADN_Crowds_Data::FireEffectProtectionInfos::~FireEffectProtectionInfos
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
ADN_Crowds_Data::FireEffectProtectionInfos::~FireEffectProtectionInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_Data::FireEffectInfos::FireEffectInfos
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
ADN_Crowds_Data::FireEffectInfos::FireEffectInfos( E_PopulationAttitude nAttitude )
    : ADN_RefWithName( ENT_Tr::ConvertFromPopulationAttitude( nAttitude, ENT_Tr::eToTr ) )
    , nAttitude_( nAttitude )
    , vProtectionInfos_( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetElement< ADN_Armors_Data >( eArmors ).GetArmorsInfos() )
    , rIntensityDensity_( 0. )
    , rIntensityFactor_ ( 0. )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_Data::FireEffectInfos::ReadArchive
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
void ADN_Crowds_Data::FireEffectInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "population-density", rIntensityDensity_ )
          >> xml::attribute( "intensity", rIntensityFactor_ )
          >> xml::list( "protection", *this, &ADN_Crowds_Data::FireEffectInfos::ReadProtection );
}

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_Data::FireEffectInfos::ReadProtection
// Created: AGE 2007-08-16
// -----------------------------------------------------------------------------
void ADN_Crowds_Data::FireEffectInfos::ReadProtection( xml::xistream& input )
{
    auto itProtection = std::find_if( vProtectionInfos_.begin(), vProtectionInfos_.end(),
                                      ADN_Tools::NameCmp( input.attribute< std::string >( "name" ) ) );
    assert( itProtection != vProtectionInfos_.end() );
    ( *itProtection )->ReadArchive( input );
}

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_Data::FireEffectInfos::WriteArchive
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
void ADN_Crowds_Data::FireEffectInfos::WriteArchive( xml::xostream& output ) const
{
    if( rIntensityDensity_.GetData() == 0. && rIntensityFactor_.GetData() == 0. )
        return;
    output << xml::start( "attrition-effect" )
             << xml::attribute( "population-density", rIntensityDensity_ )
             << xml::attribute( "intensity", rIntensityFactor_ )
             << xml::attribute( "population-attitude", ENT_Tr::ConvertFromPopulationAttitude( nAttitude_ ) );
    for( auto it = vProtectionInfos_.begin(); it != vProtectionInfos_.end(); ++it )
        ( *it )->WriteArchive( output );
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_Data::FireEffectInfos::~FireEffectInfos
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
ADN_Crowds_Data::FireEffectInfos::~FireEffectInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_Data::FireEffectRoeInfos::FireEffectRoeInfos
// Created: SBO 2005-11-21
// -----------------------------------------------------------------------------
ADN_Crowds_Data::FireEffectRoeInfos::FireEffectRoeInfos( E_PopulationRoe nRoe )
    : ADN_RefWithName( ENT_Tr::ConvertFromPopulationRoe( nRoe ) )
    , nRoe_( nRoe )
    , rAttritionSurface_( 0. )
    , rPH_( 0. )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_Data::FireEffectRoeInfos::~FireEffectRoeInfos
// Created: SBO 2005-11-21
// -----------------------------------------------------------------------------
ADN_Crowds_Data::FireEffectRoeInfos::~FireEffectRoeInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_Data::GetRoe
// Created: LGY 2011-02-15
// -----------------------------------------------------------------------------
unsigned int ADN_Crowds_Data::FireEffectRoeInfos::GetRoe()
{
    return nRoe_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_Data::FireEffectRoeInfos::ReadArchive
// Created: SBO 2005-11-21
// -----------------------------------------------------------------------------
void ADN_Crowds_Data::FireEffectRoeInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "attrition-surface", rAttritionSurface_ )
          >> xml::attribute( "ph", rPH_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_Data::FireEffectRoeInfos::WriteArchive
// Created: SBO 2005-11-21
// -----------------------------------------------------------------------------
void ADN_Crowds_Data::FireEffectRoeInfos::WriteArchive( xml::xostream& output ) const
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
// Name: ADN_Crowds_Data::SpeedEffectVolumeInfos::SpeedEffectVolumeInfos
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
ADN_Crowds_Data::SpeedEffectVolumeInfos::SpeedEffectVolumeInfos( ADN_Volumes_Data::VolumeInfos* ptr )
    : ADN_CrossedRef( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetElement< ADN_Volumes_Data >( eVolumes ).GetVolumesInfos(), ptr, true )
    , rDensity_( 0. )
    , rMaxSpeed_( 0. )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_Data::SpeedEffectVolumeInfos::ReadArchive
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
void ADN_Crowds_Data::SpeedEffectVolumeInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "population-density", rDensity_  )
          >> xml::attribute( "max-speed", rMaxSpeed_  );
}

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_Data::SpeedEffectVolumeInfos::WriteArchive
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
void ADN_Crowds_Data::SpeedEffectVolumeInfos::WriteArchive( xml::xostream& output ) const
{
    if( rDensity_ == 0. && rMaxSpeed_ == 0. )
        return;
    if( !GetCrossedElement() )
        return;
    output << xml::start( "unit" )
             << xml::attribute( "unit-size", *this )
             << xml::attribute( "population-density", rDensity_  )
             << xml::attribute( "max-speed", rMaxSpeed_  )
           << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_Data::SpeedEffectVolumeInfos::~SpeedEffectVolumeInfos
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
ADN_Crowds_Data::SpeedEffectVolumeInfos::~SpeedEffectVolumeInfos()
{
    // NOTHING
}

// =============================================================================
// ADN_Crowds_Data::SpeedEffectInfos
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_Data::SpeedEffectInfos::SpeedEffectInfos
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
ADN_Crowds_Data::SpeedEffectInfos::SpeedEffectInfos( E_PopulationAttitude nAttitude )
    : ADN_RefWithName( ENT_Tr::ConvertFromPopulationAttitude( nAttitude, ENT_Tr::eToTr ) )
    , nAttitude_( nAttitude )
    , vVolumeInfos_( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetElement< ADN_Volumes_Data >( eVolumes ).GetVolumesInfos() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_Data::SpeedEffectInfos::ReadArchive
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
void ADN_Crowds_Data::SpeedEffectInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::list( "unit", *this, &ADN_Crowds_Data::SpeedEffectInfos::ReadSpeedEffect );
}

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_Data::ReadSpeedEffect
// Created: AGE 2007-08-16
// -----------------------------------------------------------------------------
void ADN_Crowds_Data::SpeedEffectInfos::ReadSpeedEffect( xml::xistream& input )
{
    const std::string type = input.attribute< std::string >( "unit-size" );
    auto itVolume = std::find_if( vVolumeInfos_.begin(), vVolumeInfos_.end(),
                                  boost::bind( &ADN_Tools::CrossedRefNameCompare< SpeedEffectVolumeInfos >, _1, boost::cref( type ) ) );
    assert( itVolume != vVolumeInfos_.end() );
    ( *itVolume )->ReadArchive( input );
}

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_Data::SpeedEffectInfos::WriteArchive
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
void ADN_Crowds_Data::SpeedEffectInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "slowing-effect" )
             << xml::attribute( "population-attitude" ,ENT_Tr::ConvertFromPopulationAttitude( nAttitude_ ) );
    for( auto it = vVolumeInfos_.begin(); it != vVolumeInfos_.end(); ++it )
        ( *it )->WriteArchive( output );
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_Data::SpeedEffectInfos::~SpeedEffectInfos
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
ADN_Crowds_Data::SpeedEffectInfos::~SpeedEffectInfos()
{
    // NOTHING
}

// =============================================================================
// ADN_Crowds_Data::UrbanEffectInfos
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_Data::UrbanEffectInfos::UrbanEffectInfos
// Created: MMC 2011-03-30
// -----------------------------------------------------------------------------
ADN_Crowds_Data::UrbanEffectInfos::UrbanEffectInfos( E_PopulationAttitude nAttitude )
    : ADN_RefWithName( ENT_Tr::ConvertFromPopulationAttitude( nAttitude, ENT_Tr::eToTr ) )
    , nAttitude_( nAttitude )
    , rDensity_( 0.0 )
    , rTime_( "0s" )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_Data::UrbanEffectInfos::~UrbanEffectInfos
// Created: MMC 2011-03-30
// -----------------------------------------------------------------------------
ADN_Crowds_Data::UrbanEffectInfos::~UrbanEffectInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_Data::UrbanEffectInfos::ReadArchive
// Created: MMC 2011-03-30
// -----------------------------------------------------------------------------
void ADN_Crowds_Data::UrbanEffectInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "density", rDensity_ )
          >> xml::attribute( "time", rTime_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_Data::UrbanEffectInfos::WriteArchive
// Created: MMC 2011-03-30
// -----------------------------------------------------------------------------
void ADN_Crowds_Data::UrbanEffectInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "urban-destruction-effect" )
            << xml::attribute( "attitude", ENT_Tr::ConvertFromPopulationAttitude( nAttitude_ ) )
            << xml::attribute( "density", rDensity_ )
            << xml::attribute( "time", rTime_ )
           << xml::end;
}

// =============================================================================
// ADN_Crowds_Data::CrowdsInfos
// =============================================================================

// -----------------------------------------------------------------------------
// Name: CrowdsInfos::CrowdsInfos
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
ADN_Crowds_Data::CrowdsInfos::CrowdsInfos()
    : nId_( ADN_Crowds_Data::idManager_.GetNextId() )
    , ptrModel_( ADN_Workspace::GetWorkspace().GetModels().GetData().GetModels( eEntityType_Population ), 0 )
    , rConcentrationDensity_( 0. )
    , rMoveDensity_( 0. )
    , rMoveSpeed_( 0. )
    , repartition_( tools::translate( "Population_Data", "Crowd" ) )
    , armedIndividuals_( 0 )
    , decontaminationDelay_( "300s" )
    , vSpeedEffectInfos_()
    , vFireEffectInfos_()
    , vFireEffectRoeInfos_()
    , vUrbanEffectInfos_()
{
    Initialize();
}

// -----------------------------------------------------------------------------
// Name: CrowdsInfos::CrowdsInfos
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
ADN_Crowds_Data::CrowdsInfos::CrowdsInfos( unsigned int id )
    : nId_( id )
    , ptrModel_( ADN_Workspace::GetWorkspace().GetModels().GetData().GetModels( eEntityType_Population ), 0 )
    , rConcentrationDensity_( 0. )
    , rMoveDensity_( 0. )
    , rMoveSpeed_( 0. )
    , repartition_( tools::translate( "Population_Data", "Crowd" ) )
    , armedIndividuals_( 0 )
    , decontaminationDelay_( "300s" )
    , vSpeedEffectInfos_()
    , vFireEffectInfos_()
    , vFireEffectRoeInfos_()
    , vUrbanEffectInfos_()
{
    Initialize();
    ADN_Crowds_Data::idManager_.Lock( id );
}

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_Data::CrowdsInfos::Initialize
// Created: ABR 2012-09-18
// -----------------------------------------------------------------------------
void ADN_Crowds_Data::CrowdsInfos::Initialize()
{
    strName_.SetContext( ADN_Workspace::GetWorkspace().GetContext( eCrowds, "crowds" ) );
    for( int i = 0; i < eNbrPopulationAttitude; ++i )
    {
        E_PopulationAttitude attitude = static_cast< E_PopulationAttitude >( i );
        SpeedEffectInfos* pSpeedNew = new SpeedEffectInfos( attitude );
        std::auto_ptr< SpeedEffectInfos > spSpeedNew( pSpeedNew );
        vSpeedEffectInfos_.AddItem( spSpeedNew.release() );

        FireEffectInfos* pFireNew = new FireEffectInfos( attitude );
        std::auto_ptr< FireEffectInfos > spFireNew( pFireNew );
        vFireEffectInfos_.AddItem( spFireNew.release() );

        UrbanEffectInfos* pUrbanNew = new UrbanEffectInfos( attitude );
        std::auto_ptr< UrbanEffectInfos > spUrbanNew( pUrbanNew );
        vUrbanEffectInfos_.AddItem( spUrbanNew.release() );
    }
    for( int i = 2; i < eNbrPopulationRoe; ++i )
    {
        FireEffectRoeInfos* pFireRoeNew = new FireEffectRoeInfos( static_cast< E_PopulationRoe >( i ) );
        std::auto_ptr< FireEffectRoeInfos > spFireRoeNew( pFireRoeNew );
        vFireEffectRoeInfos_.AddItem( spFireRoeNew.release() );
    }
    BindExistenceTo( &ptrModel_ );
}

// -----------------------------------------------------------------------------
// Name: CrowdsInfos::~CrowdsInfos
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
ADN_Crowds_Data::CrowdsInfos::~CrowdsInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CrowdsInfos::CreateCopy
// Created: APE 2005-02-14
// -----------------------------------------------------------------------------
ADN_Crowds_Data::CrowdsInfos* ADN_Crowds_Data::CrowdsInfos::CreateCopy()
{
    CrowdsInfos* pCopy = new CrowdsInfos();

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
        pCopy->vUrbanEffectInfos_[ i ] = vUrbanEffectInfos_[ i ];
    }
    for( unsigned int i = 0; i < vFireEffectRoeInfos_.size(); ++i )
        pCopy->vFireEffectRoeInfos_[ i ] = vFireEffectRoeInfos_[ i ];

    return pCopy;
}

// -----------------------------------------------------------------------------
// Name: CrowdsInfos::ReadArchive
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
void ADN_Crowds_Data::CrowdsInfos::ReadArchive( xml::xistream& input )
{
    double rArmedIndividuals = 0.;
    input >> xml::attribute( "name", *this )
          >> xml::attribute( "concentration-density", rConcentrationDensity_ )
          >> xml::attribute( "moving-base-density", rMoveDensity_ )
          >> xml::attribute( "moving-speed", rMoveSpeed_ )
          >> xml::attribute( "decisional-model", ptrModel_ )
          >> xml::optional
          >> xml::attribute( "armed-individuals", rArmedIndividuals );

    input >> xml::start( "slowing-effects" )
            >> xml::list( "slowing-effect", *this, &ADN_Crowds_Data::CrowdsInfos::ReadSlowingEffect )
          >> xml::end
          >> xml::start( "attrition-effects" )
            >> xml::list( "attrition-effect", *this, &ADN_Crowds_Data::CrowdsInfos::ReadAttritionEffect )
          >> xml::end
          >> xml::start( "unit-fire-effects" )
            >> xml::list( "unit", *this, &ADN_Crowds_Data::CrowdsInfos::ReadFireEffect )
          >> xml::end
          >> xml::start( "urban-destruction-effects" )
            >> xml::list( "urban-destruction-effect", *this, &ADN_Crowds_Data::CrowdsInfos::ReadUrbanEffect )
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
// Name: ADN_Crowds_Data::CrowdsInfos::ReadSlowingEffect
// Created: AGE 2007-08-16
// -----------------------------------------------------------------------------
void ADN_Crowds_Data::CrowdsInfos::ReadSlowingEffect( xml::xistream& input )
{
    std::string strAttitude = input.attribute< std::string >( "population-attitude" );
    uint nAttitude = ENT_Tr::ConvertToPopulationAttitude( strAttitude );
    if( nAttitude == static_cast< E_PopulationAttitude >( -1 ) )
        throw MASA_EXCEPTION( tools::translate( "Population_Data",  "Crowd types - Invalid crowd attitude '%1'" ).arg( strAttitude.c_str() ).toStdString() );
    vSpeedEffectInfos_[ nAttitude ]->ReadArchive( input );
}

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_Data::CrowdsInfos::ReadAttritionEffect
// Created: AGE 2007-08-16
// -----------------------------------------------------------------------------
void ADN_Crowds_Data::CrowdsInfos::ReadAttritionEffect( xml::xistream& input )
{
    std::string strAttitude = input.attribute< std::string >( "population-attitude" );
    uint nAttitude = ENT_Tr::ConvertToPopulationAttitude( strAttitude );
    if( nAttitude == static_cast< E_PopulationAttitude >( -1 ) )
        throw MASA_EXCEPTION( tools::translate( "Population_Data",  "Crowd types - Invalid crowd attitude '%1'" ).arg( strAttitude.c_str() ).toStdString() );
    vFireEffectInfos_[ nAttitude ]->ReadArchive( input );
}

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_Data::CrowdsInfos::ReadFireEffect
// Created: AGE 2007-08-16
// -----------------------------------------------------------------------------
void ADN_Crowds_Data::CrowdsInfos::ReadFireEffect( xml::xistream& input )
{
    std::string strROE = input.attribute< std::string >( "rule-of-engagment" );
    uint nROE = ENT_Tr::ConvertToPopulationRoe( strROE );
    if( nROE == static_cast< E_PopulationRoe >( -1 ) )
        throw MASA_EXCEPTION( tools::translate( "Population_Data", "Crowd types - Invalid crowd 'rules of engagment' '%1'" ).arg( strROE.c_str() ).toStdString() );
    for( auto it = vFireEffectRoeInfos_.begin(); it != vFireEffectRoeInfos_.end(); ++it )
        if( ( *it )->GetRoe() == nROE )
            ( *it )->ReadArchive( input );
}

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_Data::CrowdsInfos::ReadUrbanEffect
// Created: MMC 2011-03-30
// -----------------------------------------------------------------------------
void ADN_Crowds_Data::CrowdsInfos::ReadUrbanEffect( xml::xistream& input )
{
    std::string strAttitude = input.attribute< std::string >( "attitude" );
    uint nAttitude = ENT_Tr::ConvertToPopulationAttitude( strAttitude );
    if( nAttitude == static_cast< E_PopulationAttitude >( -1 ) )
        throw MASA_EXCEPTION( tools::translate( "Population_Data",  "Crowd types - Invalid crowd attitude '%1'" ).arg( strAttitude.c_str() ).toStdString() );

    vUrbanEffectInfos_[ nAttitude ]->ReadArchive( input );
}

// -----------------------------------------------------------------------------
// Name: CrowdsInfos::CrowdsInfos::WriteArchive
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
void ADN_Crowds_Data::CrowdsInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "population" )
            << xml::attribute( "name", *this )
            << xml::attribute( "id", nId_ )
            << xml::attribute( "decisional-model", ptrModel_ )
            << xml::attribute( "concentration-density", rConcentrationDensity_ )
            << xml::attribute( "moving-base-density", rMoveDensity_ )
            << xml::attribute( "moving-speed", rMoveSpeed_ )
            << xml::attribute( "armed-individuals", armedIndividuals_.GetData() / 100. );

    output << xml::start( "slowing-effects" );
    for( auto it = vSpeedEffectInfos_.begin(); it != vSpeedEffectInfos_.end(); ++it )
        ( *it )->WriteArchive( output );
    output << xml::end;

    output << xml::start( "attrition-effects" );
    for( auto it = vFireEffectInfos_.begin(); it != vFireEffectInfos_.end(); ++it )
        ( *it )->WriteArchive( output );
    output << xml::end;

    output << xml::start( "unit-fire-effects" );
    for( auto it = vFireEffectRoeInfos_.begin(); it != vFireEffectRoeInfos_.end(); ++it )
        ( *it )->WriteArchive( output );
    output << xml::end;

    output << xml::start( "urban-destruction-effects" );
    for( auto it = vUrbanEffectInfos_.begin(); it != vUrbanEffectInfos_.end(); ++it )
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
// Name: ADN_Crowds_Data::CrowdsInfos::CheckValidity
// Created: JSR 2013-04-04
// -----------------------------------------------------------------------------
void ADN_Crowds_Data::CrowdsInfos::CheckValidity( ADN_ConsistencyChecker& checker, const std::string& name, int tab, int subTab /* = -1 */, const std::string& /* optional = "" */ )
{
    ptrModel_.CheckValidity( checker, name, tab, subTab, tools::translate( "ADN_Crowds_Data", "Behavior model" ).toStdString() );
    for( auto it = vFireEffectInfos_.begin(); it != vFireEffectInfos_.end(); ++it )
    {
        const FireEffectInfos* infos = ( *it );
        for( auto it2 = infos->vProtectionInfos_.begin(); it2 != infos->vProtectionInfos_.end(); ++it2 )
            ( *it2 )->CheckValidity( checker, name, tab, subTab, tools::translate( "ADN_Crowds_Data", "Attritions" ).toStdString() );
    }
    for( auto it = vSpeedEffectInfos_.begin(); it != vSpeedEffectInfos_.end(); ++it )
    {
        const SpeedEffectInfos* infos = ( *it );
        for( auto it2 = infos->vVolumeInfos_.begin(); it2 != infos->vVolumeInfos_.end(); ++it2 )
            ( *it2 )->CheckValidity( checker, name, tab, subTab, tools::translate( "ADN_Crowds_Data", "Speeds" ).toStdString() );
    }
    repartition_.CheckNoError( strName_.GetData().c_str(), checker, eCrowds );
}

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_Data::ReloadingSpeedEffectInfos::ReloadingSpeedEffectInfos
// Created: SBO 2005-11-14
// -----------------------------------------------------------------------------
ADN_Crowds_Data::ReloadingSpeedEffectInfos::ReloadingSpeedEffectInfos()
    : rDensity_ ( 0. )
    , rModifier_( 0. )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_Data::ReloadingSpeedEffectInfos::~ReloadingSpeedEffectInfos
// Created: SBO 2005-11-14
// -----------------------------------------------------------------------------
ADN_Crowds_Data::ReloadingSpeedEffectInfos::~ReloadingSpeedEffectInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_Data::ReloadingSpeedEffectInfos::ReadArchive
// Created: SBO 2005-11-14
// -----------------------------------------------------------------------------
void ADN_Crowds_Data::ReloadingSpeedEffectInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::start( "reloading-time-effect" )
            >> xml::attribute( "population-density", rDensity_ )
            >> xml::attribute( "modifier", rModifier_ )
          >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_Data::ReloadingSpeedEffectInfos::WriteArchive
// Created: SBO 2005-11-14
// -----------------------------------------------------------------------------
void ADN_Crowds_Data::ReloadingSpeedEffectInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "reloading-time-effect" )
            << xml::attribute( "population-density", rDensity_ )
            << xml::attribute( "modifier", rModifier_ )
          << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_Data constructor
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
ADN_Crowds_Data::ADN_Crowds_Data()
    : ADN_Data_ABC( eCrowds )
    , vCrowds_()
    , reloadingSpeedEffectInfos_()
    , timeBetweenNbcApplication_ ( "1h" )
{
    vCrowds_.AddUniquenessChecker( eError, duplicateName_, &ADN_Tools::NameExtractor );
}

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_Data destructor
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
ADN_Crowds_Data::~ADN_Crowds_Data()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_Data::FilesNeeded
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
void ADN_Crowds_Data::FilesNeeded( tools::Path::T_Paths& vFiles ) const
{
    vFiles.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szPopulation_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_Data::ReadArchive
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
void ADN_Crowds_Data::ReadArchive( xml::xistream& input )
{
    std::string time = "1h";
    input >> xml::start( "populations" )
          >> xml::optional >> xml::start( "time-between-nbc-applications")
          >> xml::attribute( "delay", time ) >> xml::end;
    reloadingSpeedEffectInfos_.ReadArchive( input );
    timeBetweenNbcApplication_ = time;
    input >> xml::list( "population", *this, &ADN_Crowds_Data::ReadPopulation )
          >> xml::end;
    vCrowds_.CheckValidity();
}

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_Data::ReadPopulation
// Created: AGE 2007-08-16
// -----------------------------------------------------------------------------
void ADN_Crowds_Data::ReadPopulation( xml::xistream& input )
{
    std::auto_ptr< CrowdsInfos > spNew( new CrowdsInfos( input.attribute< unsigned int >( "id" ) ) );
    spNew->ReadArchive( input );
    vCrowds_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_Data::WriteArchive
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
void ADN_Crowds_Data::WriteArchive( xml::xostream& output ) const
{
    if( vCrowds_.GetErrorStatus() == eError )
        throw MASA_EXCEPTION( GetInvalidDataErrorMsg() );

    output << xml::start( "populations" );
    tools::SchemaWriter schemaWriter;
    schemaWriter.WritePhysicalSchema( output, "Crowds" );
    reloadingSpeedEffectInfos_.WriteArchive( output );
    output << xml::start( "time-between-nbc-applications" )
            << xml::attribute( "delay", timeBetweenNbcApplication_.GetData() )
           << xml::end;
    for( auto it = vCrowds_.begin(); it != vCrowds_.end(); ++it )
        ( *it )->WriteArchive( output );
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_Data::GetCrowdsThatUse
// Created: SBO 2006-09-11
// -----------------------------------------------------------------------------
QStringList ADN_Crowds_Data::GetCrowdsThatUse( ADN_Models_Data::ModelInfos& model )
{
    QStringList result;
    for( auto it = vCrowds_.begin(); it != vCrowds_.end(); ++it )
        if( ( *it )->ptrModel_.GetData() == &model )
            result << ( *it )->strName_.GetData().c_str();
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_Data::CheckDatabaseValidity
// Created: JSR 2013-04-04
// -----------------------------------------------------------------------------
void ADN_Crowds_Data::CheckDatabaseValidity( ADN_ConsistencyChecker& checker ) const
{
    ADN_Data_ABC::CheckDatabaseValidity( checker );
    for( auto it = vCrowds_.begin(); it != vCrowds_.end(); ++it )
        ( *it )->CheckValidity( checker, ( *it )->strName_.GetData(), eCrowds );
}
