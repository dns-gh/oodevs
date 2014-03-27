//*****************************************************************************
//
// $Created: JDY 03-06-25 $
// $input: /MVW_v10/Build/SDK/ADN/Src/ADN_Sensors_Data.cpp $
// $Author: Nld $
// $Modtime: 3/05/05 18:28 $
// $Revision: 15 $
// $Workfile: ADN_Sensors_Data.cpp $
//
//*****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Sensors_Data.h"
#include "ADN_Radars_Data.h"
#include "ADN_Workspace.h"
#include "ADN_Project_Data.h"
#include "ADN_Objects_Data.h"
#include "ADN_Tools.h"
#include "ADN_Tr.h"
#include "ADN_WorkspaceElement.h"
#include "clients_kernel/XmlTranslations.h"
#include "ENT/ENT_Tr.h"

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::LimitedToSensorsInfos::LimitedToSensorsInfos
// Created: JSR 2010-03-16
// LTO
// -----------------------------------------------------------------------------
ADN_Sensors_Data::LimitedToSensorsInfos::LimitedToSensorsInfos()
{
    // NOTHING
}
// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::LimitedToSensorsInfos::~LimitedToSensorsInfos
// Created: JSR 2010-03-16
// LTO
// -----------------------------------------------------------------------------
ADN_Sensors_Data::LimitedToSensorsInfos::~LimitedToSensorsInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::LimitedToSensorsInfos::CreateCopy
// Created: JSR 2010-03-16
// LTO
// -----------------------------------------------------------------------------
ADN_Sensors_Data::LimitedToSensorsInfos* ADN_Sensors_Data::LimitedToSensorsInfos::CreateCopy()
{
    LimitedToSensorsInfos* newInfos = new LimitedToSensorsInfos();
    newInfos->strName_ = strName_.GetData();
    return newInfos;
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::LimitedToSensorsInfos::ReadArchive
// Created: JSR 2010-03-16
// LTO
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::LimitedToSensorsInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "name", *this );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::LimitedToSensorsInfos::WriteArchive
// Created: JSR 2010-03-16
// LTO
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::LimitedToSensorsInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "sensor" )
                << xml::attribute( "name", *this )
           << xml::end;
}

// =============================================================================
//
// =============================================================================

//-----------------------------------------------------------------------------
// Name: ModificatorPostureInfos::ModificatorPostureInfos
// Created: JDY 03-09-29
//-----------------------------------------------------------------------------
ADN_Sensors_Data::ModificatorPostureInfos::ModificatorPostureInfos( const E_UnitPosture& e )
    : eType_( e )
    , rCoeff_( 1 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ModificatorPostureInfos::ReadArchive
// Created: APE 2004-11-23
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::ModificatorPostureInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "value", rCoeff_ );
}

// -----------------------------------------------------------------------------
// Name: ModificatorPostureInfos::WriteArchive
// Created: APE 2004-11-23
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::ModificatorPostureInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "distance-modifier" )
            << xml::attribute( "type", ADN_Tools::ComputePostureScriptName( eType_ ) )
            << xml::attribute( "value", rCoeff_ )
           << xml::end;
}

// =============================================================================
//
// =============================================================================

//-----------------------------------------------------------------------------
// Name: TargetInfos::TargetInfos
// Created: JDY 03-07-04
//-----------------------------------------------------------------------------
ADN_Sensors_Data::TargetInfos::TargetInfos()
    : ADN_CrossedRef( ADN_Workspace::GetWorkspace().GetObjects().GetData().GetObjectInfos(), 0, true )
    , rDistanceDetection_( 0 )
    , populationInfos_()
{
    // Initialize the posture modificator infos
    for( int i = 0 ; i < eNbrUnitPosture ; ++i )
        vModifStance_.AddItem( new ModificatorPostureInfos( static_cast< E_UnitPosture >( i ) ) );
}

// -----------------------------------------------------------------------------
// Name: TargetInfos::~TargetInfos
// Created: APE 2004-12-13
// -----------------------------------------------------------------------------
ADN_Sensors_Data::TargetInfos::~TargetInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TargetInfos::CreateCopy
// Created: APE 2004-11-26
// -----------------------------------------------------------------------------
ADN_Sensors_Data::TargetInfos* ADN_Sensors_Data::TargetInfos::CreateCopy()
{
    TargetInfos* pNew = new TargetInfos();
    pNew->SetCrossedElement( GetCrossedElement() );
    pNew->strName_ = strName_.GetData();
    pNew->rDistanceDetection_ = rDistanceDetection_.GetData();
    pNew->populationInfos_.CopyFrom( populationInfos_ );
    for( unsigned int i = 0; i < vModifStance_.size(); ++i )
    {
        ModificatorPostureInfos* pNewModif = new ModificatorPostureInfos( vModifStance_[ i ]->eType_ );
        pNewModif->rCoeff_ = vModifStance_[ i ]->rCoeff_.GetData();
        pNew->vModifStance_[ i ] = pNewModif;
    }
    return pNew;
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::TargetInfos::ReadPosture
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::TargetInfos::ReadPosture( xml::xistream& input )
{
    const std::string type = input.attribute< std::string >( "type" );
    for( unsigned i = 0; i < eNbrUnitPosture; ++i )
        if( type == ADN_Tools::ComputePostureScriptName( static_cast< E_UnitPosture >( i ) ) )
        {
            vModifStance_.at( i )->ReadArchive( input );
            return;
        }
    throw MASA_EXCEPTION( tools::translate( "Sensor_Data", "Sensors - Invalid stance '%1'" ).arg( type.c_str() ).toStdString() );
}

// -----------------------------------------------------------------------------
// Name: TargetInfos::ReadArchive
// Created: APE 2004-11-23
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::TargetInfos::ReadArchive( xml::xistream& input )
{
    ADN_Objects_Data_ObjectInfos* pObject = ADN_Workspace::GetWorkspace().GetObjects().GetData().FindObject( input.attribute< std::string >( "type" ) );
    SetCrossedElement( pObject );
    input >> xml::attribute( "detection-distance", rDistanceDetection_ );
    populationInfos_.ReadArchive( input );
    input >> xml::start( "source-posture-modifiers" )
            >> xml::list( "distance-modifier", *this, &ADN_Sensors_Data::TargetInfos::ReadPosture )
          >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: TargetInfos::WriteArchive
// Created: APE 2004-11-23
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::TargetInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "object" )
             << xml::attribute( "type", GetCrossedElement() ? GetCrossedElement()->strType_.GetData() : "" )
             << xml::attribute( "detection-distance", rDistanceDetection_ );
    populationInfos_.WriteArchive( output );

    output     << xml::start( "source-posture-modifiers" );
    for( auto it = vModifStance_.begin(); it != vModifStance_.end(); ++it )
        ( *it )->WriteArchive( output );
    output     << xml::end
           << xml::end;
}

// =============================================================================
//
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::DisasterInfos
// Created: LGY 2012-12-03
// -----------------------------------------------------------------------------
ADN_Sensors_Data::DisasterInfos::DisasterInfos()
    : ADN_CrossedRef( ADN_Workspace::GetWorkspace().GetDisasters().GetData().GetDisastersInfos(), 0, true )
    , rDetectionThreshold_( 0)
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::~DisasterInfos
// Created: LGY 2012-12-03
// -----------------------------------------------------------------------------
ADN_Sensors_Data::DisasterInfos::~DisasterInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::CreateCopy
// Created: LGY 2012-12-03
// -----------------------------------------------------------------------------
ADN_Sensors_Data::DisasterInfos* ADN_Sensors_Data::DisasterInfos::CreateCopy()
{
    DisasterInfos* pNew = new DisasterInfos();
    pNew->SetCrossedElement( GetCrossedElement() );
    pNew->rDetectionThreshold_ = rDetectionThreshold_.GetData();
    return pNew;
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::ReadArchive
// Created: LGY 2012-12-03
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::DisasterInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "type", *this )
          >> xml::attribute( "detection-threshold", rDetectionThreshold_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::WriteArchive
// Created: LGY 2012-12-03
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::DisasterInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "disaster" )
             << xml::attribute( "type", *this )
             << xml::attribute( "detection-threshold", rDetectionThreshold_ )
           << xml::end;
}

// =============================================================================
// ADN_Sensor_Data::PopulationInfos
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::PopulationInfos::PopulationInfos
// Created: SBO 2005-11-21
// -----------------------------------------------------------------------------
ADN_Sensors_Data::PopulationInfos::PopulationInfos()
    : rDensity_( 0 )
    , rModifier_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::PopulationInfos::~PopulationInfos
// Created: SBO 2005-11-21
// -----------------------------------------------------------------------------
ADN_Sensors_Data::PopulationInfos::~PopulationInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::PopulationInfos::GetItemName
// Created: SBO 2005-11-21
// -----------------------------------------------------------------------------
std::string ADN_Sensors_Data::PopulationInfos::GetItemName()
{
    return strName_.GetData();
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::PopulationInfos::CopyFrom
// Created: SBO 2005-11-21
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::PopulationInfos::CopyFrom( PopulationInfos& populationInfo )
{
    rDensity_  = populationInfo.rDensity_ .GetData();
    rModifier_ = populationInfo.rModifier_.GetData();
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::PopulationInfos::ReadArchive
// Created: SBO 2005-11-21
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::PopulationInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::start( "population-modifier" )
            >> xml::attribute( "density", rDensity_ )
            >> xml::attribute( "modifier", rModifier_ )
          >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::PopulationInfos::WriteArchive
// Created: SBO 2005-11-21
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::PopulationInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "population-modifier" )
            << xml::attribute( "density", rDensity_ )
            << xml::attribute( "modifier", rModifier_ )
           << xml::end;
}

// =============================================================================
// ADN_Sensor_Data::SensorInfos
// =============================================================================

//-----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::SensorInfos
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Sensors_Data::SensorInfos::SensorInfos()
    : bCanDetectAgents_    ( false )
    , bCanDetectObjects_   ( false )
    , bCanDetectDisasters_ ( false )
    , bCanScan_            ( false )
    , rFirerDetectionRange_( 0 )
    , rAngle_              ( 0 )
    , bLimitedToSensors_   ( false ) // LTO
    , rDistProximity_      ( 0 )
    , rDistDetection_      ( 0 )
    , rDistReco_           ( 0 )
    , rDistIdent_          ( 0 )
    , vModifSizes_         ( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetElement< ADN_Volumes_Data >( eVolumes ).GetVolumesInfos() )
    , vModifIlluminations_ ( false )
    , vModifWeather_       ( false )
    , vModifEnvironments_  ( false )
    , vModifUrbanBlocks_   ( ADN_Workspace::GetWorkspace().GetUrban().GetData().GetMaterialsInfos() )
    , vModifStance_        ( false )
    , vModifTargetStance_  ( false )
    , detectionDelay_      ( "0h" )
    , activatedOnRequest_  ( false )
{
    strName_.SetContext( ADN_Workspace::GetWorkspace().GetContext( eSensors, "sensors" ) );

    // initialize illumination modificator infos
    unsigned int i = 0;
    for( i = 0; i< eNbrLightingType; ++i )
    {
        ModificatorIlluminationInfos* pInfo = new ModificatorIlluminationInfos( static_cast< E_LightingType >( i ) );
        vModifIlluminations_.AddItem( pInfo );
    }

    // initialize meteo modificator infos
    for( i = 0; i< eNbrSensorWeatherModifiers; ++i )
    {
        ModificatorMeteoInfos* pInfo = new ModificatorMeteoInfos( static_cast< E_SensorWeatherModifiers >( i) );
        vModifWeather_.AddItem( pInfo );
    }

    // initialize environment modificator infos
    for( i = 0; i< eNbrVisionObject; ++i )
    {
        ModificatorEnvironmentInfos* pInfo = new ModificatorEnvironmentInfos( static_cast< E_VisionObject >( i) );
        vModifEnvironments_.AddItem( pInfo );
    }

    // initialize posture modificator infos
    for( i = 0; i< eNbrUnitPosture; ++i )
    {
        ModificatorPostureInfos* pInfo1 = new ModificatorPostureInfos( static_cast< E_UnitPosture >( i) );
        vModifStance_.AddItem( pInfo1 );
        ModificatorPostureInfos* pInfo2 = new ModificatorPostureInfos( static_cast< E_UnitPosture >( i ) );
        vModifTargetStance_.AddItem( pInfo2 );
    }
}

//-----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::SensorInfos
// Created: JDY 03-07-15
//-----------------------------------------------------------------------------
ADN_Sensors_Data::SensorInfos::~SensorInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::CreateCopy
// Created: AGN 2003-11-03
// -----------------------------------------------------------------------------
ADN_Sensors_Data::SensorInfos* ADN_Sensors_Data::SensorInfos::CreateCopy()
{
    ADN_Sensors_Data::SensorInfos* pCopy = new ADN_Sensors_Data::SensorInfos();
    pCopy->rDistProximity_      = rDistProximity_.GetData();
    pCopy->rDistDetection_      = rDistDetection_.GetData();
    pCopy->rDistReco_           = rDistReco_.GetData();
    pCopy->rDistIdent_          = rDistIdent_.GetData();
    pCopy->rAngle_              = rAngle_.GetData();
    pCopy->bCanScan_            = bCanScan_.GetData();
    pCopy->rFirerDetectionRange_= rFirerDetectionRange_.GetData();
    pCopy->bCanDetectAgents_    = bCanDetectAgents_.GetData();
    pCopy->bLimitedToSensors_   = bLimitedToSensors_.GetData(); // LTO
    pCopy->bCanDetectObjects_   = bCanDetectObjects_.GetData();
    pCopy->bCanDetectDisasters_ = bCanDetectDisasters_.GetData();
    pCopy->detectionDelay_      = detectionDelay_.GetData();
    pCopy->activatedOnRequest_  = activatedOnRequest_.GetData();
    pCopy->populationInfos_.CopyFrom( populationInfos_ );

    for( auto itTarget = vTargets_.begin(); itTarget != vTargets_.end(); ++itTarget )
    {
        TargetInfos* pNewInfo = (*itTarget)->CreateCopy();
        pCopy->vTargets_.AddItem( pNewInfo );
    }

    for( auto it = vDisasters_.begin(); it != vDisasters_.end(); ++it )
    {
        DisasterInfos* pNewInfo = (*it)->CreateCopy();
        pCopy->vDisasters_.AddItem( pNewInfo );
    }

    // LTO begin
    pCopy->vLimitedToSensorsInfos_.reserve( vLimitedToSensorsInfos_.size() );
    for( auto it = vLimitedToSensorsInfos_.begin(); it != vLimitedToSensorsInfos_.end(); ++it )
    {
        LimitedToSensorsInfos* newInfos = (*it)->CreateCopy();
        pCopy->vLimitedToSensorsInfos_.AddItem( newInfos );
    }
    // LTO end

    unsigned int i;
    for( i = 0; i < eNbrLightingType; ++i )
        pCopy->vModifIlluminations_[ i ]->rCoeff_ = vModifIlluminations_[ i ]->rCoeff_.GetData();

    // initialize meteo modificator infos
    for( i = 0; i < eNbrSensorWeatherModifiers; ++i )
        pCopy->vModifWeather_[ i ]->rCoeff_ = vModifWeather_[ i ]->rCoeff_.GetData();

    // initialize environment modificator infos
    for( i= 0; i < eNbrVisionObject; ++i )
        pCopy->vModifEnvironments_[ i ]->rCoeff_ = vModifEnvironments_[ i ]->rCoeff_.GetData();

    // initialize posture modificator infos
    for( i = 0; i < eNbrUnitPosture; ++i )
    {
        pCopy->vModifStance_[ i ]->rCoeff_ = vModifStance_[ i ]->rCoeff_.GetData();
        pCopy->vModifTargetStance_[ i ]->rCoeff_ = vModifTargetStance_[ i ]->rCoeff_.GetData();
    }

    for( auto itSizeModif = vModifSizes_.begin(); itSizeModif != vModifSizes_.end(); ++itSizeModif )
        pCopy->vModifSizes_[ std::distance( vModifSizes_.begin(), itSizeModif ) ]->rCoeff_ = (*itSizeModif)->rCoeff_.GetData();

    for( auto itUrbanBlockModif = vModifUrbanBlocks_.begin(); itUrbanBlockModif != vModifUrbanBlocks_.end(); ++itUrbanBlockModif )
        pCopy->vModifUrbanBlocks_[ std::distance( vModifUrbanBlocks_.begin(), itUrbanBlockModif ) ]->rCoeff_ = (*itUrbanBlockModif)->rCoeff_.GetData();

    return pCopy;
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::ReadLimitedToSensorsList
// Created: JSR 2010-03-16
// LTO
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::SensorInfos::ReadLimitedToSensorsList( xml::xistream& input )
{
    bLimitedToSensors_ = true;

    std::auto_ptr< LimitedToSensorsInfos > spNew( new LimitedToSensorsInfos() );
    spNew->ReadArchive( input );
    vLimitedToSensorsInfos_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::SensorInfos::ReadBaseDistance
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::SensorInfos::ReadBaseDistance( xml::xistream& input )
{
    const std::string level = input.attribute< std::string >( "level" );
    if( level == "identification" )
        input >> xml::attribute( "distance", rDistIdent_ );
    else if( level == "recognition" )
        input >> xml::attribute( "distance", rDistReco_ );
    else if( level == "detection" )
        input >> xml::attribute( "distance", rDistDetection_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::SensorInfos::ReadSize
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::SensorInfos::ReadSize( xml::xistream& input )
{
    const std::string type = input.attribute< std::string >( "type" );
    auto it = std::find_if( vModifSizes_.begin(), vModifSizes_.end(),
                            boost::bind( &ADN_Tools::CrossedRefNameCompare< ModificatorSizeInfos >, _1, boost::cref( type ) ) );
    if( it == vModifSizes_.end() )
        throw MASA_EXCEPTION( tools::translate( "Sensor_Data", "Sensors - Invalid unit volume '%1'" ).arg( type.c_str() ).toStdString() );
    (*it)->ReadArchive( input );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::SensorInfos::ReadPrecipitation
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::SensorInfos::ReadPrecipitation( xml::xistream& input )
{
    const std::string type = input.attribute< std::string >( "type" );
    for( unsigned i = 0; i < eNbrSensorWeatherModifiers; ++i )
        if( ADN_Tools::Scriptify( ADN_Tr::ConvertFromSensorWeatherModifiers( static_cast< E_SensorWeatherModifiers >( i ) ) ) == type )
        {
            vModifWeather_.at( i )->ReadArchive( input );
            return;
        }
    throw MASA_EXCEPTION(tools::translate( "Sensor_Data", "Sensors - Invalid weather '%1'" ).arg( type.c_str() ).toStdString() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::SensorInfos::ReadVisibility
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::SensorInfos::ReadVisibility( xml::xistream& input )
{
    E_LightingType n = ENT_Tr::ConvertToLightingType( input.attribute< std::string >( "type" ) );
    vModifIlluminations_.at( n )->ReadArchive( input );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::SensorInfos::ReadSourcePosture
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::SensorInfos::ReadSourcePosture( xml::xistream& input )
{
    const std::string type = input.attribute< std::string >( "type" );
    for( unsigned i = 0; i < eNbrUnitPosture; ++i )
        if( type == ADN_Tools::ComputePostureScriptName( static_cast< E_UnitPosture >( i ) ) )
        {
            vModifStance_.at( i )->ReadArchive( input );
            return;
        }
    throw MASA_EXCEPTION( tools::translate( "Sensor_Data", "Sensors - Invalid stance '%1'" ).arg( type.c_str() ).toStdString() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::SensorInfos::ReadTargetPosture
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::SensorInfos::ReadTargetPosture( xml::xistream& input )
{
    const std::string type = input.attribute< std::string >( "type" );
    for( unsigned i = 0; i < eNbrUnitPosture; ++i )
        if( type == ADN_Tools::ComputePostureScriptName( static_cast< E_UnitPosture >( i ) ) )
        {
            vModifTargetStance_.at( i )->ReadArchive( input );
            return;
        }
    throw MASA_EXCEPTION( tools::translate( "Sensor_Data", "Sensors - Invalid stance '%1'" ).arg( type.c_str() ).toStdString() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::SensorInfos::ReadTerrain
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::SensorInfos::ReadTerrain( xml::xistream& input )
{
    E_VisionObject n = ADN_Tr::ConvertToVisionObject( input.attribute< std::string >( "type" ) );
    vModifEnvironments_.at( n )->ReadArchive( input );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::SensorInfos::ReadTerrain
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::SensorInfos::ReadUrbanBlockMaterial( xml::xistream& input )
{
    const std::string type = input.attribute< std::string >( "type" );
    auto it = std::find_if( vModifUrbanBlocks_.begin(), vModifUrbanBlocks_.end(), ModificatorUrbanBlockInfos::Cmp( type ) );
    if( it == vModifUrbanBlocks_.end() )
        throw MASA_EXCEPTION( tools::translate( "Sensor_Data", "Sensors - Invalid unit volume '%1'" ).arg( type.c_str() ).toStdString() );
    ( *it )->ReadArchive( input );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::SensorInfos::ReadUnitDetection
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::SensorInfos::ReadUnitDetection( xml::xistream& input )
{
    bCanDetectAgents_ = true;

    input >> xml::attribute( "scanning", bCanScan_ )
          >> xml::attribute( "firer-detection-distance", rFirerDetectionRange_ )
          >> xml::attribute( "angle", rAngle_ )
          >> xml::optional >> xml::start( "limited-to-sensors" ) // LTO
            >> xml::list( "sensor", *this, &ADN_Sensors_Data::SensorInfos::ReadLimitedToSensorsList ) // LTO
          >> xml::end // LTO
          >> xml::start( "base-distances" )
            >> xml::attribute( "close-range", rDistProximity_ )
            >> xml::list( "base-distance", *this, &ADN_Sensors_Data::SensorInfos::ReadBaseDistance )
          >> xml::end
          >> xml::start( "distance-modifiers" )
            >> xml::start( "size-modifiers" )
                >> xml::list( "distance-modifier", *this, &ADN_Sensors_Data::SensorInfos::ReadSize )
            >> xml::end
            >> xml::start( "precipitation-modifiers" )
                >> xml::list( "distance-modifier", *this, &ADN_Sensors_Data::SensorInfos::ReadPrecipitation )
            >> xml::end
            >> xml::start( "visibility-modifiers" )
                >> xml::list( "distance-modifier", *this, &ADN_Sensors_Data::SensorInfos::ReadVisibility )
            >> xml::end
            >> xml::start( "source-posture-modifiers" )
                >> xml::list( "distance-modifier", *this, &ADN_Sensors_Data::SensorInfos::ReadSourcePosture )
            >> xml::end
            >> xml::start( "target-posture-modifiers" )
                >> xml::list( "distance-modifier", *this, &ADN_Sensors_Data::SensorInfos::ReadTargetPosture )
            >> xml::end
            >> xml::start( "terrain-modifiers" )
                >> xml::list( "distance-modifier", *this, &ADN_Sensors_Data::SensorInfos::ReadTerrain )
            >> xml::end
            >> xml::start( "urbanBlock-material-modifiers" )
                >> xml::list( "distance-modifier", *this, &ADN_Sensors_Data::SensorInfos::ReadUrbanBlockMaterial )
            >> xml::end;
    populationInfos_.ReadArchive( input );
    input >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::SensorInfos::ReadItem
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::SensorInfos::ReadItem( const std::string& name, xml::xistream& input )
{
    if( name == "unit-detection" )
        ReadUnitDetection( input );
    else if( name == "object-detection" )
        ReadObjectDetection( input );
    else if( name == "disaster-detection" )
        ReadDisasterDetection( input );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::SensorInfos::ReadObject
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::SensorInfos::ReadObject( xml::xistream& input )
{
    std::auto_ptr<TargetInfos> spNew( new TargetInfos() );
    spNew->ReadArchive( input );
    vTargets_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::SensorInfos::ReadObjectDetection
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::SensorInfos::ReadObjectDetection( xml::xistream& input )
{
    bCanDetectObjects_ = true;
    input >> xml::list( "object", *this, &ADN_Sensors_Data::SensorInfos::ReadObject );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::ReadDisasterDetection
// Created: LGY 2012-12-05
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::SensorInfos::ReadDisasterDetection( xml::xistream& input )
{
    bCanDetectDisasters_ = true;
    input >> xml::list( "disaster", *this, &ADN_Sensors_Data::SensorInfos::ReadDisaster );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::ReadDisaster
// Created: LGY 2012-12-05
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::SensorInfos::ReadDisaster( xml::xistream& input )
{
    std::auto_ptr< DisasterInfos > spNew( new DisasterInfos() );
    spNew->ReadArchive( input );
    vDisasters_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: SensorInfos::ReadArchive
// Created: APE 2004-11-24
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::SensorInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "name", *this )
          >> xml::attribute( "detection-delay", detectionDelay_ )
          >> xml::optional
          >> xml::attribute( "activation-on-request", activatedOnRequest_ )
          >> xml::list( *this, &ADN_Sensors_Data::SensorInfos::ReadItem );
}

// -----------------------------------------------------------------------------
// Name: SensorInfos::WriteArchive
// Created: APE 2004-11-24
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::SensorInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "sensor" )
            << xml::attribute( "name", *this )
            << xml::attribute( "detection-delay", detectionDelay_ );
    if( activatedOnRequest_.GetData() )
        output << xml::attribute( "activation-on-request", true );

    if( bCanDetectAgents_.GetData() )
    {
        output << xml::start( "unit-detection" )
                << xml::attribute( "scanning", bCanScan_ )
                << xml::attribute( "firer-detection-distance", rFirerDetectionRange_ )
                << xml::attribute( "angle", rAngle_ );

        // LTO begin
        if( bLimitedToSensors_.GetData() )
        {
            output << xml::start( "limited-to-sensors" );
            for( unsigned int i = 0; i < vLimitedToSensorsInfos_.size(); ++i )
                vLimitedToSensorsInfos_[ i ]->WriteArchive( output );
            output << xml::end;
        }
        // LTO end

        output  << xml::start( "base-distances" )
                    << xml::attribute( "close-range", rDistProximity_ )
                    << xml::start( "base-distance" )
                        << xml::attribute( "level", "identification" ) << xml::attribute( "distance", rDistIdent_ )
                    << xml::end
                    << xml::start( "base-distance" )
                        << xml::attribute( "level", "recognition" ) << xml::attribute( "distance", rDistReco_ )
                    << xml::end
                    << xml::start( "base-distance" )
                        << xml::attribute( "level", "detection" ) << xml::attribute( "distance", rDistDetection_ )
                    << xml::end
                << xml::end; //base-distances

        output << xml::start( "distance-modifiers" );

        populationInfos_.WriteArchive( output );

        output << xml::start( "size-modifiers" );
        for( auto it1 = vModifSizes_.begin(); it1 != vModifSizes_.end(); ++it1 )
            ( *it1 )->WriteArchive( output );
        output << xml::end;

        output << xml::start( "precipitation-modifiers" );
        for( auto it2 = vModifWeather_.begin(); it2 != vModifWeather_.end(); ++it2 )
            ( *it2 )->WriteArchive( output );
        output << xml::end;

        output << xml::start( "visibility-modifiers" );
        for( auto it3 = vModifIlluminations_.begin(); it3 != vModifIlluminations_.end(); ++it3 )
            ( *it3 )->WriteArchive( output );
        output << xml::end;

        output << xml::start( "source-posture-modifiers" );
        for( auto it4 = vModifStance_.begin(); it4 != vModifStance_.end(); ++it4 )
            ( *it4 )->WriteArchive( output );
        output << xml::end;

        output << xml::start( "target-posture-modifiers" );
        for( auto it5 = vModifTargetStance_.begin(); it5 != vModifTargetStance_.end(); ++it5 )
            ( *it5 )->WriteArchive( output );
        output << xml::end;

        output << xml::start( "terrain-modifiers" );
        for( auto it6 = vModifEnvironments_.begin(); it6 != vModifEnvironments_.end(); ++it6 )
            ( *it6 )->WriteArchive( output );
        output << xml::end;

        output << xml::start( "urbanBlock-material-modifiers" );
        for( auto it7 = vModifUrbanBlocks_.begin(); it7 != vModifUrbanBlocks_.end(); ++it7 )
            ( *it7 )->WriteArchive( output );
        output << xml::end;

        output << xml::end; // distance-modifiers
        output << xml::end; // unit-detection
    }

    if( bCanDetectObjects_.GetData() )
    {
        output << xml::start( "object-detection" );
        for( auto it = vTargets_.begin(); it != vTargets_.end(); ++it )
            ( *it )->WriteArchive( output );
        output << xml::end;
    }

    if( bCanDetectDisasters_.GetData() )
    {
        output << xml::start( "disaster-detection" );
        for( auto it = vDisasters_.begin(); it != vDisasters_.end(); ++it )
            ( *it )->WriteArchive( output );
        output << xml::end;
    }

    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::SensorInfos::CheckDatabaseValidity
// Created: JSR 2013-04-19
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::SensorInfos::CheckDatabaseValidity( ADN_ConsistencyChecker& checker ) const
{
    if( bCanDetectDisasters_.GetData() )
    {
        for( auto it = vDisasters_.begin(); it != vDisasters_.end(); ++it )
            ( *it )->CheckValidity( checker, strName_.GetData(), eSensors, -1, tools::translate( "ADN_Sensors_Data", "Disasters" ).toStdString() );
        for( auto it = vModifUrbanBlocks_.begin(); it != vModifUrbanBlocks_.end(); ++it )
            ( *it )->CheckValidity( checker, strName_.GetData(), eSensors, -1, tools::translate( "ADN_Sensors_Data", "UrbanBlock material modifiers" ).toStdString() );
        for( auto it = vModifSizes_.begin(); it != vModifSizes_.end(); ++it )
            ( *it )->CheckValidity( checker, strName_.GetData(), eSensors, -1, tools::translate( "ADN_Sensors_Data", "Target size" ).toStdString() );
    }
}

// -----------------------------------------------------------------------------
// Name: ALATInfos::ALATInfos
// Created: APE 2005-01-17
// -----------------------------------------------------------------------------
ADN_Sensors_Data::ALATInfos::ALATInfos()
{
    for( int n = 1; n < eNbrVisionObject; ++n )
        surveyTimes_[ n - 1 ] = "0s";
}

// -----------------------------------------------------------------------------
// Name: ALATInfos::~ALATInfos
// Created: APE 2005-01-17
// -----------------------------------------------------------------------------
ADN_Sensors_Data::ALATInfos::~ALATInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::ALATInfos::ReadTime
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::ALATInfos::ReadTime( xml::xistream& input )
{
    E_VisionObject n = ADN_Tr::ConvertToVisionObject( input.attribute< std::string >( "terrain" ) );
    surveyTimes_[ n-1 ] = input.attribute< std::string >( "time" );
}

// -----------------------------------------------------------------------------
// Name: ALATInfos::ReadArchive
// Created: APE 2005-01-17
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::ALATInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::start( "alat-monitoring-times" )
            >> xml::list( "alat-monitoring-time", *this, &ADN_Sensors_Data::ALATInfos::ReadTime )
          >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ALATInfos::WriteArchive
// Created: APE 2005-01-17
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::ALATInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "alat-monitoring-times" );
    for( int n = 1; n < eNbrVisionObject; ++n )
        output << xml::start( "alat-monitoring-time" )
                << xml::attribute( "terrain", ADN_Tr::ConvertFromVisionObject( static_cast< E_VisionObject >( n ) ) )
                << xml::attribute( "time", surveyTimes_[ n-1 ] )
               << xml::end;
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: CobraInfos::CobraInfos
// Created: APE 2005-01-17
// -----------------------------------------------------------------------------
ADN_Sensors_Data::CobraInfos::CobraInfos()
    : rRange_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CobraInfos::~CobraInfos
// Created: APE 2005-01-17
// -----------------------------------------------------------------------------
ADN_Sensors_Data::CobraInfos::~CobraInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CobraInfos::ReadArchive
// Created: APE 2005-01-17
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::CobraInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::start( "cobra-radar" )
            >> xml::attribute( "action-range", rRange_ )
          >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: CobraInfos::WriteArchive
// Created: APE 2005-01-17
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::CobraInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "cobra-radar" )
            << xml::attribute( "action-range", rRange_ )
           << xml::end;
}

//-----------------------------------------------------------------------------
// Name: ADN_Sensors_Data constructor
// Created: JDY 03-06-30
//-----------------------------------------------------------------------------
ADN_Sensors_Data::ADN_Sensors_Data()
    : ADN_Data_ABC( eSensors )
    , radarData_( new ADN_Radars_Data() )
{
    vSensors_.AddUniquenessChecker( eError, duplicateName_, &ADN_Tools::NameExtractor );
}

//-----------------------------------------------------------------------------
// Name: ADN_Sensors_Data destructor
// Created: JDY 03-06-30
//-----------------------------------------------------------------------------
ADN_Sensors_Data::~ADN_Sensors_Data()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::FilesNeeded
// Created: JDY 03-09-08
//-----------------------------------------------------------------------------
void ADN_Sensors_Data::FilesNeeded( tools::Path::T_Paths& files ) const
{
    files.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szSensors_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::ReadSensor
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::ReadSensor( xml::xistream& input )
{
    std::auto_ptr<SensorInfos> spNew( new SensorInfos() );
    spNew->ReadArchive( input );
    vSensors_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::ReadArchive
// Created: APE 2004-11-24
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::ReadArchive( xml::xistream& input )
{
    input >> xml::start( "sensors" )
            >> xml::start( "sensors" )
                >> xml::list( "sensor", *this, &ADN_Sensors_Data::ReadSensor )
            >> xml::end;
    alatInfos_ .ReadArchive( input );
    cobraInfos_.ReadArchive( input );
    radarData_->ReadArchive( input );
    input >> xml::end;
    vSensors_.CheckValidity();
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::WriteArchive
// Created: APE 2004-11-24
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::WriteArchive( xml::xostream& output ) const
{
    if( vSensors_.GetErrorStatus() == eError )
        throw MASA_EXCEPTION( tools::translate( "ADN_Sensors_Data", "Invalid data on tab '%1', subtab '%2'" )
                              .arg( ADN_Tr::ConvertFromWorkspaceElement( currentTab_ ).c_str() ).arg( tools::translate( "ADN_Sensors_Data", "Sensors" ) ).toStdString() );

    output << xml::start( "sensors" );
    tools::SchemaWriter schemaWriter;
    schemaWriter.WritePhysicalSchema( output, "Sensors" );
    alatInfos_ .WriteArchive( output );
    cobraInfos_.WriteArchive( output );
    radarData_->WriteArchive( output );

    output  << xml::start( "sensors" );
    for( auto it = vSensors_.begin(); it != vSensors_.end(); ++it )
        ( *it )->WriteArchive( output );
    output  << xml::end
           << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::GetSensorsThatUse
// Created: LGY 2012-05-29
// -----------------------------------------------------------------------------
QStringList ADN_Sensors_Data::GetSensorsThatUse( ADN_Objects_Data_ObjectInfos& object )
{
    QStringList result;
    for( auto it = vSensors_.begin(); it != vSensors_.end(); ++it )
    {
        SensorInfos* pComp = *it;
        for( auto itTarget = pComp->vTargets_.begin(); itTarget != pComp->vTargets_.end(); ++itTarget )
        {
            ADN_Objects_Data_ObjectInfos* infos = ( *itTarget )->GetCrossedElement();
            if( infos && infos->strName_.GetData() == object.strName_.GetData() )
                result << pComp->strName_.GetData().c_str();
        }
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::GetSensorsThatUse
// Created: LGY 2012-12-05
// -----------------------------------------------------------------------------
QStringList ADN_Sensors_Data::GetSensorsThatUse( ADN_Disasters_Data::DisasterInfos& disaster )
{
    QStringList result;
    for( auto it = vSensors_.begin(); it != vSensors_.end(); ++it )
    {
        SensorInfos* pComp = *it;
        for( auto it = pComp->vDisasters_.begin(); it != pComp->vDisasters_.end(); ++it )
        {
            ADN_Disasters_Data::DisasterInfos* infos = ( *it )->GetCrossedElement();
            if( infos && infos->strName_.GetData() == disaster.strName_.GetData() )
                result << pComp->strName_.GetData().c_str();
        }
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::CheckDatabaseValidity
// Created: JSR 2013-04-19
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::CheckDatabaseValidity( ADN_ConsistencyChecker& checker ) const
{
    ADN_Data_ABC::CheckDatabaseValidity( checker );
    for( auto it = vSensors_.begin(); it != vSensors_.end(); ++it )
        ( *it )->CheckDatabaseValidity( checker );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::GetRadars
// Created: LGY 2013-07-23
// -----------------------------------------------------------------------------
void ADN_Sensors_Data::LoadTranslations( const tools::Path& xmlFile, kernel::XmlTranslations* )
{
    ADN_Data_ABC::LoadTranslations( xmlFile );
    radarData_->LoadTranslations( xmlFile );
}
