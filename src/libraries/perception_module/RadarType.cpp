// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "RadarType.h"
#include "RadarClass.h"
#include "PerceptionLevel.h"
#include "tools/Codec.h"
#include "wrapper/Hook.h"
#include "wrapper/View.h"
#include "MT_Tools/MT_Vector2D.h"
#include <xeumeuleu/xml.hpp>

using namespace sword;
using namespace sword::perception;

DECLARE_HOOK( ConvertSecondsToSim, double, ( double seconds ) )
DECLARE_HOOK( GetConsumptionTypeSize, size_t, () )
DECLARE_HOOK( FindConsumptionType, void, ( const char* consumptionType, void(*callback)( unsigned int identifier, void* userData ), void* userData ) )
DECLARE_HOOK( GetConsumptionMode, unsigned int, ( const SWORD_Model* entity ) )
DECLARE_HOOK( IsUsingActiveRadar, bool, ( const SWORD_Model* entity ) )

RadarType::T_RadarTypeMap RadarType::radarTypes_;
unsigned int RadarType::nNextID_ = 0;

namespace
{
    template< typename T >
    bool ReadPcAndBaseTime( xml::xistream& xis, const std::string& name, T& time )
    {
        int seconds = 0;
        std::string timeString;
        xis >> xml::optional >> xml::attribute( name, timeString );
        if( tools::DecodeTime( timeString, seconds ) )
        {
            time = seconds;
            return true;
        }
        return false;
    }
};

// -----------------------------------------------------------------------------
// Name: RadarType::Initialize
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
void RadarType::Initialize( xml::xistream& xis )
{
    xis >> xml::start( "radars" )
            >> xml::list( "radar", &ReadRadar )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: RadarType::ReadRadar
// Created: ABL 2007-07-26
// -----------------------------------------------------------------------------
void RadarType::ReadRadar( xml::xistream& xis )
{
    std::string strRadarName;
    xis >> xml::attribute( "name", strRadarName );

    std::string strType;
    xis >> xml::attribute( "type", strType );
    strType = strType == "ecoute" ? "tapping" : // $$$$ _RC_ SLI 2012-06-06: remove this
              strType == "ecoute radar" ? "tapping-radar":
              strType;
    const RadarClass* pType = RadarClass::Find( strType );
    if( !pType )
        throw xml::exception( xis.context() + "Unknown radar type " + strType );

    const RadarType*& pRadarType = radarTypes_[ strRadarName ];
    if( pRadarType )
        throw xml::exception( xis.context() + "Radar " + strRadarName + " already exists" );
    pRadarType = new RadarType( strRadarName, *pType, xis );
}

// -----------------------------------------------------------------------------
// Name: RadarType::Terminate
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
void RadarType::Terminate()
{
    for( auto it = radarTypes_.begin(); it != radarTypes_.end(); ++it )
        delete it->second;
    radarTypes_.clear();
}

// -----------------------------------------------------------------------------
// Name: RadarType constructor
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
RadarType::RadarType( const std::string& strName, const RadarClass& radarClass, xml::xistream& xis )
    : nID_                  ( nNextID_++ )
    , strName_              ( strName )
    , class_                ( radarClass )
    , rRadius_              ( 0. )
    , rMinHeight_           ( -std::numeric_limits< double >::max() )
    , rMaxHeight_           (  std::numeric_limits< double >::max() )
    , detectableActivities_ ( GET_HOOK( GetConsumptionTypeSize )(), false )
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
}

// -----------------------------------------------------------------------------
// Name: RadarType destructor
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
RadarType::~RadarType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RadarType::InitializeRange
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
void RadarType::InitializeRange( xml::xistream& xis )
{
    xis >> xml::attribute( "action-range", rRadius_ )
        >> xml::optional >> xml::attribute( "min-height", rMinHeight_ )
        >> xml::optional >> xml::attribute( "max-height", rMaxHeight_ );

    if( rRadius_ < 0 )
        throw xml::exception( xis.context() + "radar: action-range < 0" );

    if( rMinHeight_ != -std::numeric_limits< double >::max() && rMinHeight_ < 0 )
        throw xml::exception( xis.context() + "radar: min-height < 0" );
    if( rMaxHeight_ != std::numeric_limits< double >::max() && rMaxHeight_ < rMinHeight_ )
        throw xml::exception( xis.context() + "radar: max-height < min-height" );
}

// -----------------------------------------------------------------------------
// Name: RadarType::InitializeActivities
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
void RadarType::InitializeActivities( xml::xistream& xis )
{
    bool bIsActivity = false;
    xis >> xml::list( "detectable-activities", *this, &RadarType::ReadDetectableActivity, bIsActivity );
    if( !bIsActivity )
    {
        std::fill( detectableActivities_.begin(), detectableActivities_.end(), true );
        return;
    }
}

// -----------------------------------------------------------------------------
// Name: RadarType::ReadDetectableActivity
// Created: ABL 2007-07-26
// -----------------------------------------------------------------------------
void RadarType::ReadDetectableActivity( xml::xistream& xis, bool& bIsActivity )
{
    bIsActivity = true;
    std::fill( detectableActivities_.begin(), detectableActivities_.end(), false );

    xis >> xml::list( "detectable-activity", *this, &RadarType::ReadActivity );
}

namespace
{
    template< typename T >
    struct ConsumptionFinder : private boost::noncopyable
    {
        ConsumptionFinder( T& detectableActivities, bool value )
            : detectableActivities_( detectableActivities )
            , value_               ( value )
        {}
        static void Notify( unsigned int identifier, void* userData )
        {
            ConsumptionFinder< T >* that = static_cast< ConsumptionFinder< T >* >( userData );
            that->detectableActivities_[ identifier ] = that->value_;
        }
    private:
        T& detectableActivities_;
        bool value_;
    };
}

// -----------------------------------------------------------------------------
// Name: RadarType::ReadActivity
// Created: ABL 2007-07-26
// -----------------------------------------------------------------------------
void RadarType::ReadActivity( xml::xistream& xis )
{
    std::string activityType;
    bool bValue = false;
    xis >> xml::attribute( "type", activityType )
        >> xml::optional >> xml::attribute( "value", bValue );
    ConsumptionFinder< T_ActivityVector > finder( detectableActivities_, bValue );
    GET_HOOK( FindConsumptionType )( activityType.c_str(), &ConsumptionFinder< T_ActivityVector >::Notify, &finder );
}

// -----------------------------------------------------------------------------
// Name: RadarType::InitializeAcquisitionTimes
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
void RadarType::InitializeAcquisitionTimes( xml::xistream& xis )
{
    bool bIsTime = false;
    xis >> xml::list( "acquisition-times", *this, &RadarType::ReadAcquisitionTime, bIsTime );

    if( !bIsTime )
    {
        rPcDetectionTime_       = rDetectionTime_       = std::numeric_limits< double >::max();
        rPcRecognitionTime_     = rRecognitionTime_     = 0;
        rPcIdentificationTime_  = rIdentificationTime_  = std::numeric_limits< double >::max();
        return;
    }
}

// -----------------------------------------------------------------------------
// Name: RadarType::ReadAcquisitionTime
// Created: ABL 2007-07-26
// -----------------------------------------------------------------------------
void RadarType::ReadAcquisitionTime( xml::xistream& xis, bool& bIsTime )
{
    bIsTime = true;
    bool bIsPCTime = false;
    xis >> xml::list( "acquisition-time", *this, &RadarType::ReadTime, bIsPCTime );

    if( !bIsPCTime )
    {
        rPcDetectionTime_       = rDetectionTime_;
        rPcRecognitionTime_     = rRecognitionTime_;
        rPcIdentificationTime_  = rIdentificationTime_;
    }
}

// -----------------------------------------------------------------------------
// Name: RadarType::ReadTime
// Created: ABL 2007-07-26
// -----------------------------------------------------------------------------
void RadarType::ReadTime( xml::xistream& xis, bool& bIsPCTime )
{
    std::string acquisitionType;
    xis >> xml::attribute( "level", acquisitionType );

    if( acquisitionType == "detection" )
    {
        if( ReadPcAndBaseTime( xis, "base-time", rDetectionTime_ ) )
            if( rDetectionTime_ < 0 )
                throw xml::exception( xis.context() + "detection acquisition-time: base-time < 0" );
            else
                rDetectionTime_ = GET_HOOK( ConvertSecondsToSim )( rDetectionTime_ );

        if( ReadPcAndBaseTime( xis, "command-post-time", rPcDetectionTime_ ) )
            if( rPcDetectionTime_ < 0 )
                throw xml::exception( xis.context() + "detection acquisition-time: command-post-time < 0" );
            else
            {
                bIsPCTime = true;
                rPcDetectionTime_ = GET_HOOK( ConvertSecondsToSim )( rPcDetectionTime_ );
            }
    }
    else if( acquisitionType == "recognition" )
    {
        if( ReadPcAndBaseTime( xis, "base-time", rRecognitionTime_ ) )
            if( rRecognitionTime_ < rDetectionTime_ )
                throw xml::exception( xis.context() + "recognition acquisition-time: base-time < detection base-time" );
            else
                rRecognitionTime_ = GET_HOOK( ConvertSecondsToSim )( rRecognitionTime_ );

        if( ReadPcAndBaseTime( xis, "command-post-time", rPcRecognitionTime_ ) )
            if( rPcRecognitionTime_ < rPcDetectionTime_ )
                throw xml::exception( xis.context() + "recognition acquisition-time: command-post-time < detection command-post-time" );
            else
                rPcRecognitionTime_ = GET_HOOK( ConvertSecondsToSim )( rPcRecognitionTime_ );
    }
    else if( acquisitionType == "identification" )
    {
        if( ReadPcAndBaseTime( xis, "base-time", rIdentificationTime_ ) )
            if( rIdentificationTime_ < rRecognitionTime_ )
                throw xml::exception( xis.context() + "identification acquisition-time: base-time < recognition base-time" );
            else
                rIdentificationTime_ = GET_HOOK( ConvertSecondsToSim )( rIdentificationTime_ );

        if( ReadPcAndBaseTime( xis, "command-post-time", rPcIdentificationTime_ ) )
            if( rPcIdentificationTime_ < rPcRecognitionTime_ )
                throw xml::exception( xis.context() + "identification acquisition-time: command-post-time < recognition command-post-time" );
            else
                rPcIdentificationTime_ = GET_HOOK( ConvertSecondsToSim )( rPcIdentificationTime_ );
    }
    else
        throw xml::exception( xis.context() + "Unknown acquisition-time: " + acquisitionType );
}

// -----------------------------------------------------------------------------
// Name: RadarType::CanAcquire
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
bool RadarType::CanAcquire( const wrapper::View& perceiver, const wrapper::View& target ) const
{
    const MT_Vector2D locationPerceiver( perceiver[ "movement/position/x" ], perceiver[ "movement/position/y" ] );
    const MT_Vector2D locationTarget( target[ "movement/position/x" ], target[ "movement/position/y" ] );
    if( locationPerceiver.Distance( locationTarget ) > rRadius_ )
        return false;
    if( !detectableActivities_[ GET_HOOK( GetConsumptionMode )( target ) ] )
        return false;

    const double rTargetHeight = target[ "movement/height" ];
    if( rTargetHeight > rMaxHeight_ || rTargetHeight < rMinHeight_ )
        return false;

    if( class_ == RadarClass::tapping_ && !target[ "can-emit" ] )
        return false;

    if( class_ == RadarClass::tappingRadar_ && !GET_HOOK( IsUsingActiveRadar )( target ) )
        return false;

    return true;
}

// -----------------------------------------------------------------------------
// Name: RadarType::ComputeAcquisitionLevel
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
const PerceptionLevel& RadarType::ComputeAcquisitionLevel( const wrapper::View& target, unsigned int nFirstAcquisitionTimeStep, unsigned int currentTick ) const
{
    const unsigned int nTimePerceived = currentTick - nFirstAcquisitionTimeStep;
    if( target[ "is-pc" ] )
    {
        if( nTimePerceived >= rPcIdentificationTime_ )
            return PerceptionLevel::identified_;
        if( nTimePerceived >= rPcRecognitionTime_ )
            return PerceptionLevel::recognized_;
        if( nTimePerceived >= rPcDetectionTime_ )
            return PerceptionLevel::detected_;
        return PerceptionLevel::notSeen_;
    }
    else
    {
        if( nTimePerceived >= rIdentificationTime_ )
            return PerceptionLevel::identified_;
        if( nTimePerceived >= rRecognitionTime_ )
            return PerceptionLevel::recognized_;
        if( nTimePerceived >= rDetectionTime_ )
            return PerceptionLevel::detected_;
        return PerceptionLevel::notSeen_;
    }
}

// -----------------------------------------------------------------------------
// Name: RadarType::FindRadarType
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
const RadarType* RadarType::Find( const std::string& strType )
{
    CIT_RadarTypeMap it = radarTypes_.find( strType );
    return it == radarTypes_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: RadarType::GetName
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
const std::string& RadarType::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: RadarType::GetClass
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
const RadarClass& RadarType::GetClass() const
{
    return class_;
}

// -----------------------------------------------------------------------------
// Name: RadarType::GetRadius
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
double RadarType::GetRadius() const
{
    return rRadius_;
}
