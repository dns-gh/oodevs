// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Radars/PHY_RadarType.cpp $
// $Author: Nld $
// $Modtime: 3/06/05 18:20 $
// $Revision: 2 $
// $Workfile: PHY_RadarType.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_RadarType.h"
#include "MIL_Time_ABC.h"
#include "PHY_RadarClass.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Entities/Agents/Roles/Communications/PHY_RoleInterface_Communications.h"
#include "Entities/Agents/Roles/Dotations/PHY_RoleInterface_Dotations.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Perception/PHY_RoleInterface_Perceiver.h"
#include "Entities/Agents/Units/Categories/PHY_Volume.h"
#include "Entities/Agents/Units/Dotations/PHY_ConsumptionType.h"
#include "Entities/Agents/Units/Sensors/DistanceModifiersHelpers.h"
#include "meteo/rawvisiondata/PHY_RawVisionData.h"
#include "meteo/PHY_Lighting.h"
#include "meteo/PHY_Precipitation.h"
#include "Tools/MIL_Tools.h"
#include "tools/Codec.h"
#include "Urban/PHY_MaterialCompositionType.h"
#include <boost/assign.hpp>

PHY_RadarType::T_RadarTypeMap PHY_RadarType::radarTypes_;
unsigned int PHY_RadarType::nNextID_ = 0;

namespace
{
    template< typename T >
    bool ReadPcAndBaseTime( xml::xistream& xis, const std::string& name, T& time )
    {
        int seconds = 0;
        const std::string timeString = xis.attribute( name, std::string() );
        if( tools::DecodeTime( timeString, seconds ) )
        {
            time = seconds;
            return true;
        }
        return false;
    }
};

// -----------------------------------------------------------------------------
// Name: PHY_RadarType::Initialize
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
void PHY_RadarType::Initialize( xml::xistream& xis, const MIL_Time_ABC& time )
{
    xis >> xml::start( "radars" )
            >> xml::list( "radar", boost::bind( &PHY_RadarType::ReadRadar, _1, boost::cref( time ) ) )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: PHY_RadarType::ReadRadar
// Created: ABL 2007-07-26
// -----------------------------------------------------------------------------
void PHY_RadarType::ReadRadar( xml::xistream& xis, const MIL_Time_ABC& time )
{
    const std::string strRadarName = xis.attribute< std::string >( "name" );
    const std::string strType = xis.attribute< std::string >( "type" );
    const PHY_RadarClass* pType = PHY_RadarClass::Find( strType );
    if( !pType )
        throw MASA_EXCEPTION( xis.context() + "Unknown radar type " + strType );
    const PHY_RadarType*& pRadarType = radarTypes_[ strRadarName ];
    if( pRadarType )
        throw MASA_EXCEPTION( xis.context() + "Radar " + strRadarName + " already exists" );
    pRadarType = new PHY_RadarType( strRadarName, *pType, time, xis );
}

// -----------------------------------------------------------------------------
// Name: PHY_RadarType::Terminate
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
void PHY_RadarType::Terminate()
{
    for( auto it = radarTypes_.begin(); it != radarTypes_.end(); ++it )
        delete it->second;
    radarTypes_.clear();
}

// -----------------------------------------------------------------------------
// Name: PHY_RadarType constructor
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
PHY_RadarType::PHY_RadarType( const std::string& strName, const PHY_RadarClass& radarClass, const MIL_Time_ABC& time, xml::xistream& xis )
    : nID_                  ( nNextID_++ )
    , strName_              ( strName )
    , class_                ( radarClass )
    , time_                 ( time )
    , rRadius_              ( 0. )
    , rMinHeight_           ( -std::numeric_limits< double >::max() )
    , rMaxHeight_           (  std::numeric_limits< double >::max() )
    , detectableActivities_ ( PHY_ConsumptionType::GetConsumptionTypes().size(), false )
    , volumeFactors_        ( PHY_Volume::GetVolumes().size(), 1 )
    , precipitationFactors_ ( weather::PHY_Precipitation::GetPrecipitations().size(), 1 )
    , lightingFactors_      ( weather::PHY_Lighting::GetLightings().size(), 1 )
    , urbanFactors_         ( PHY_MaterialCompositionType::Count(), 0 )
    , rDetectionTime_       ( std::numeric_limits< double >::max() )
    , rRecognitionTime_     ( std::numeric_limits< double >::max() )
    , rIdentificationTime_  ( std::numeric_limits< double >::max() )
    , rPcDetectionTime_     ( std::numeric_limits< double >::max() )
    , rPcRecognitionTime_   ( std::numeric_limits< double >::max() )
    , rPcIdentificationTime_( std::numeric_limits< double >::max() )
{
    InitializeRange           ( xis );
    InitializeActivities      ( xis );
    InitializeAcquisitionTimes( xis );
    if( xis.has_child( "distance-modifiers" ) )
    {
        xis >> xml::start( "distance-modifiers" );
        distance_modifiers::InitializeFactors( PHY_Volume::GetVolumes(), "size-modifiers", volumeFactors_, xis );
        distance_modifiers::InitializeFactors( weather::PHY_Precipitation::GetPrecipitations (), "precipitation-modifiers", precipitationFactors_, xis );
        distance_modifiers::InitializeFactors( weather::PHY_Lighting::GetLightings(), "visibility-modifiers", lightingFactors_, xis );
        distance_modifiers::InitializeTerrainModifiers( xis, environmentFactors_ );
        distance_modifiers::InitializeUrbanFactors( xis, urbanFactors_ );
        xis >> xml::end;
    }
    else
    {

        environmentFactors_ = boost::assign::map_list_of( PHY_RawVisionData::eVisionEmpty, 1 )
                                                ( PHY_RawVisionData::eVisionForest, 1 )
                                                ( PHY_RawVisionData::eVisionUrban, 1 )
                                                ( PHY_RawVisionData::eVisionGround, 1 );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RadarType destructor
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
PHY_RadarType::~PHY_RadarType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RadarType::InitializeRange
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
void PHY_RadarType::InitializeRange( xml::xistream& xis )
{
    xis >> xml::attribute( "action-range", rRadius_ )
        >> xml::optional >> xml::attribute( "min-height", rMinHeight_ )
        >> xml::optional >> xml::attribute( "max-height", rMaxHeight_ );

    if( rRadius_ < 0 )
        throw MASA_EXCEPTION( xis.context() + "radar: action-range < 0" );
    rRadius_ = MIL_Tools::ConvertMeterToSim( rRadius_ );

    if( rMinHeight_ != -std::numeric_limits< double >::max() && rMinHeight_ < 0 )
        throw MASA_EXCEPTION( xis.context() + "radar: min-height < 0" );
    if( rMaxHeight_ != std::numeric_limits< double >::max() && rMaxHeight_ < rMinHeight_ )
        throw MASA_EXCEPTION( xis.context() + "radar: max-height < min-height" );
}

// -----------------------------------------------------------------------------
// Name: PHY_RadarType::InitializeActivities
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
void PHY_RadarType::InitializeActivities( xml::xistream& xis )
{
    bool bIsActivity = false;
    xis >> xml::list( "detectable-activities", *this, &PHY_RadarType::ReadDetectableActivity, bIsActivity );
    if( !bIsActivity )
    {
        std::fill( detectableActivities_.begin(), detectableActivities_.end(), true );
        return;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RadarType::ReadDetectableActivity
// Created: ABL 2007-07-26
// -----------------------------------------------------------------------------
void PHY_RadarType::ReadDetectableActivity( xml::xistream& xis, bool& bIsActivity )
{
    bIsActivity = true;
    std::fill( detectableActivities_.begin(), detectableActivities_.end(), false );
    xis >> xml::list( "detectable-activity", *this, &PHY_RadarType::ReadActivity );
}

// -----------------------------------------------------------------------------
// Name: PHY_RadarType::ReadActivity
// Created: ABL 2007-07-26
// -----------------------------------------------------------------------------
void PHY_RadarType::ReadActivity( xml::xistream& xis )
{
    const PHY_ConsumptionType::T_ConsumptionTypeMap& consumptionTypes = PHY_ConsumptionType::GetConsumptionTypes();

    auto it = consumptionTypes.find( xis.attribute< std::string >( "type" ) );
    if( it != consumptionTypes.end() )
    {
        const PHY_ConsumptionType& conso = *it->second;
        detectableActivities_[ conso.GetID() ] = xis.attribute< bool >( "value", false );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RadarType::InitializeAcquisitionTimes
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
void PHY_RadarType::InitializeAcquisitionTimes( xml::xistream& xis )
{
    bool bIsTime = false;
    xis >> xml::list( "acquisition-times", *this, &PHY_RadarType::ReadAcquisitionTime, bIsTime );

    if( !bIsTime )
    {
        rDetectionTime_ = std::numeric_limits< double >::max();
        rPcDetectionTime_ = std::numeric_limits< double >::max();
        rRecognitionTime_ = 0;
        rPcRecognitionTime_ = 0;
        rIdentificationTime_ = std::numeric_limits< double >::max();
        rPcIdentificationTime_ = std::numeric_limits< double >::max();
        return;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RadarType::ReadAcquisitionTime
// Created: ABL 2007-07-26
// -----------------------------------------------------------------------------
void PHY_RadarType::ReadAcquisitionTime( xml::xistream& xis, bool& bIsTime )
{
    bIsTime = true;
    bool bIsPCTime = false;
    xis >> xml::list( "acquisition-time", *this, &PHY_RadarType::ReadTime, bIsPCTime );

    if( !bIsPCTime )
    {
        rPcDetectionTime_       = rDetectionTime_;
        rPcRecognitionTime_     = rRecognitionTime_;
        rPcIdentificationTime_  = rIdentificationTime_;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RadarType::ReadTime
// Created: ABL 2007-07-26
// -----------------------------------------------------------------------------
void PHY_RadarType::ReadTime( xml::xistream& xis, bool& bIsPCTime )
{
    const std::string acquisitionType = xis.attribute< std::string >( "level" );

    if( acquisitionType == "detection" )
    {
        if( ReadPcAndBaseTime( xis, "base-time", rDetectionTime_ ) )
            if( rDetectionTime_ < 0 )
                throw MASA_EXCEPTION( xis.context() + "detection acquisition-time: base-time < 0" );
            else
                rDetectionTime_ = MIL_Tools::ConvertSecondsToSim( rDetectionTime_ );

        if( ReadPcAndBaseTime( xis, "command-post-time", rPcDetectionTime_ ) )
            if( rPcDetectionTime_ < 0 )
                throw MASA_EXCEPTION( xis.context() + "detection acquisition-time: command-post-time < 0" );
            else
            {
                bIsPCTime = true;
                rPcDetectionTime_ = MIL_Tools::ConvertSecondsToSim( rPcDetectionTime_ );
            }
    }
    else if( acquisitionType == "recognition" )
    {
        if( ReadPcAndBaseTime( xis, "base-time", rRecognitionTime_ ) )
            if( rRecognitionTime_ < rDetectionTime_ )
                throw MASA_EXCEPTION( xis.context() + "recoginition acquisition-time: base-time < detection base-time" );
            else
                rRecognitionTime_ = MIL_Tools::ConvertSecondsToSim( rRecognitionTime_ );

        if( ReadPcAndBaseTime( xis, "command-post-time", rPcRecognitionTime_ ) )
            if( rPcRecognitionTime_ < rPcDetectionTime_ )
                throw MASA_EXCEPTION( xis.context() + "recognition acquisition-time: command-post-time < detection command-post-time" );
            else
                rPcRecognitionTime_ = MIL_Tools::ConvertSecondsToSim( rPcRecognitionTime_ );
    }
    else if( acquisitionType == "identification" )
    {
        if( ReadPcAndBaseTime( xis, "base-time", rIdentificationTime_ ) )
            if( rIdentificationTime_ < rRecognitionTime_ )
                throw MASA_EXCEPTION( xis.context() + "identification acquisition-time: base-time < recognition base-time" );
            else
                rIdentificationTime_ = MIL_Tools::ConvertSecondsToSim( rIdentificationTime_ );

        if( ReadPcAndBaseTime( xis, "command-post-time", rPcIdentificationTime_ ) )
            if( rPcIdentificationTime_ < rPcRecognitionTime_ )
                throw MASA_EXCEPTION( xis.context() + "identification acquisition-time: command-post-time < recognition command-post-time" );
            else
                rPcIdentificationTime_ = MIL_Tools::ConvertSecondsToSim( rPcIdentificationTime_ );
    }
    else
        throw MASA_EXCEPTION( xis.context() + "Unknown acquisition-time: " + acquisitionType );
}

// -----------------------------------------------------------------------------
// Name: PHY_RadarType::CanAcquire
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
bool PHY_RadarType::CanAcquire( const MIL_Agent_ABC& perceiver, const MIL_Agent_ABC& target ) const
{
    const PHY_RoleInterface_Location& roleLocationPerceiver = perceiver.GetRole< PHY_RoleInterface_Location >();
    const PHY_RoleInterface_Location& roleLocationTarget    = target   .GetRole< PHY_RoleInterface_Location >();

    if( roleLocationPerceiver.GetPosition().Distance( roleLocationTarget.GetPosition() ) > rRadius_ )
        return false;

    if( !detectableActivities_[ target.GetRole< dotation::PHY_RoleInterface_Dotations >().GetConsumptionMode().GetID() ] )
        return false;

    const double rTargetHeight = target.GetRole< PHY_RoleInterface_Location >().GetHeight();
    if( rTargetHeight > rMaxHeight_ || rTargetHeight < rMinHeight_ )
        return false;

    if( class_ == PHY_RadarClass::tapping_ && !target.CallRole( &PHY_RoleInterface_Communications::CanEmit, true )  )
        return false;

    if( class_ == PHY_RadarClass::tappingRadar_ && !target.CallRole( &PHY_RoleInterface_Perceiver::IsUsingActiveRadar, false ) )
        return false;

    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RadarType::ComputeAcquisitionLevel
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_RadarType::ComputeAcquisitionLevel( const MIL_Agent_ABC& target, unsigned int nFirstAcquisitionTimeStep ) const
{
    const unsigned int nTimePerceived = time_.GetCurrentTimeStep() - nFirstAcquisitionTimeStep;
    if( target.IsPC() )
    {
        if( nTimePerceived >= rPcIdentificationTime_ )
            return PHY_PerceptionLevel::identified_;
        if( nTimePerceived >= rPcRecognitionTime_ )
            return PHY_PerceptionLevel::recognized_;
        if( nTimePerceived >= rPcDetectionTime_ )
            return PHY_PerceptionLevel::detected_;
        return PHY_PerceptionLevel::notSeen_;
    }
    else
    {
        if( nTimePerceived >= rIdentificationTime_ )
            return PHY_PerceptionLevel::identified_;
        if( nTimePerceived >= rRecognitionTime_ )
            return PHY_PerceptionLevel::recognized_;
        if( nTimePerceived >= rDetectionTime_ )
            return PHY_PerceptionLevel::detected_;
        return PHY_PerceptionLevel::notSeen_;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RadarType::FindRadarType
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
const PHY_RadarType* PHY_RadarType::Find( const std::string& strType )
{
    auto it = radarTypes_.find( strType );
    return it == radarTypes_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_RadarType::GetName
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
const std::string& PHY_RadarType::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RadarType::GetClass
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
const PHY_RadarClass& PHY_RadarType::GetClass() const
{
    return class_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RadarType::GetRadius
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
double PHY_RadarType::GetRadius() const
{
    return rRadius_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RadarType::ComputeEnvironmentFactor
// Created: LDC 2014-01-27
// -----------------------------------------------------------------------------
double PHY_RadarType::ComputeEnvironmentFactor( unsigned char nEnv ) const
{
    double res = nEnv & PHY_RawVisionData::eVisionEmpty ? environmentFactors_.find( 0 )->second : 1.;
    for( unsigned int mask = 1, idx = 1; idx < PHY_RawVisionData::eNbrVisionObject; mask <<= 1, ++idx )
        if( mask & nEnv )
            res *= environmentFactors_.find( mask )->second;
    return res;
}

// -----------------------------------------------------------------------------
// Name: PHY_RadarType::GetVolumeFactor
// Created: JSR 2014-04-02
// -----------------------------------------------------------------------------
double PHY_RadarType::GetVolumeFactor( const PHY_Volume& volume ) const
{
    return volumeFactors_.at( volume.GetID() );
}

// -----------------------------------------------------------------------------
// Name: PHY_RadarType::GetPrecipitationFactor
// Created: JSR 2014-04-03
// -----------------------------------------------------------------------------
double PHY_RadarType::GetPrecipitationFactor( const weather::PHY_Precipitation& precipitation ) const
{
    return precipitationFactors_.at( precipitation.GetID() );
}

// -----------------------------------------------------------------------------
// Name: PHY_RadarType::GetLightingFactor
// Created: JSR 2014-04-03
// -----------------------------------------------------------------------------
double PHY_RadarType::GetLightingFactor( const weather::PHY_Lighting& lighting ) const
{
    return lightingFactors_.at( lighting.GetID() );
}

// -----------------------------------------------------------------------------
// Name: std::vector< double >& PHY_RadarType::GetUrbanFactors
// Created: JSR 2014-04-03
// -----------------------------------------------------------------------------
const std::vector< double >& PHY_RadarType::GetUrbanFactors() const
{
    return urbanFactors_;
}
