// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Sensors/PHY_SensorTypeAgent.cpp $
// $Author: Jvt $
// $Modtime: 23/03/05 16:19 $
// $Revision: 6 $
// $Workfile: PHY_SensorTypeAgent.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_SensorTypeAgent.h"
#include "Entities/Agents/Units/Postures/PHY_Posture.h"
#include "Entities/Agents/Roles/Posture/PHY_RoleInterface_Posture.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/HumanFactors/PHY_RoleInterface_HumanFactors.h"
#include "Entities/Agents/Roles/Population/PHY_RoleInterface_Population.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Populations/MIL_PopulationConcentration.h"
#include "Entities/Populations/MIL_PopulationFlow.h"
#include "Meteo/PHY_Precipitation.h"
#include "Meteo/PHY_Lighting.h"
#include "Meteo/RawVisionData/PHY_RawVisionDataIterator.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Tools/MIL_Tools.h"
#include <xeumeuleu/xml.h>

namespace
{
   template< typename C >
   class Loader
   {
   public:
        explicit Loader( const C& container )
            : container_( container )
        {}

        void ReadFactor( xml::xistream& xis, PHY_SensorTypeAgent::T_FactorVector& factors )
        {
            std::string containerType;
            xis >> xml::attribute( "type", containerType );

            C::const_iterator it = container_.find( containerType );
            if( it != container_.end() )
            {
                assert( factors.size() > it->second->GetID() );
                MT_Float& rFactor = factors[ it->second->GetID() ];

                xis >> xml::attribute( "value", rFactor );
                if( rFactor < 0 || rFactor > 1 )
                    xis.error( "distance-modifier: value not in [0..1]" );
            }
            else
                xis.error( "distance-modifier: unknow type" );
        }
   private:
       const C& container_;
    };

   template<>
   class Loader< PHY_Posture::T_PostureMap >
   {
   public:
        explicit Loader( const PHY_Posture::T_PostureMap& container )
            : container_( container )
        {}

        void ReadFactor( xml::xistream& xis, PHY_SensorTypeAgent::T_FactorVector& factors )
        {
            std::string containerType;
            xis >> xml::attribute( "type", containerType );

            PHY_Posture::CIT_PostureMap it = container_.find( containerType );
            if( it != container_.end() )
            {
                if( !it->second->CanModifyDetection() )
                    return;
                assert( factors.size() > it->second->GetID() );
                MT_Float& rFactor = factors[ it->second->GetID() ];

                xis >> xml::attribute( "value", rFactor );
                if( rFactor < 0 || rFactor > 1 )
                    xis.error( "distance-modifier: value not in [0..1]" );
            }
            else
                xis.error( "distance-modifier: unknow type" );
        }

   private:
       const PHY_Posture::T_PostureMap& container_;
    };

    template< typename C >
    void InitializeFactors( const C& container, const std::string& strTagName, PHY_SensorTypeAgent::T_FactorVector& factors, xml::xistream& xis )
    {
        typedef typename Loader< C > T_Loader;
        T_Loader loader( container );
        xis >> xml::start( strTagName )
                >> xml::list( "distance-modifier", loader, &T_Loader::ReadFactor, factors )
            >> xml::end();
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent constructor
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
PHY_SensorTypeAgent::PHY_SensorTypeAgent( const PHY_SensorType& type, xml::xistream& xis )
    : type_                ( type )
    , volumeFactors_       ( PHY_Volume       ::GetVolumes       ().size(), 0. )
    , precipitationFactors_( PHY_Precipitation::GetPrecipitations().size(), 0. )
    , lightingFactors_     ( PHY_Lighting     ::GetLightings     ().size(), 0. )
    , postureSourceFactors_( PHY_Posture      ::GetPostures      ().size(), 0. )
    , postureTargetFactors_( PHY_Posture      ::GetPostures      ().size(), 0. )
    , environmentFactors_  ( PHY_RawVisionData::eNbrVisionObjects         , 0. )
    , rPopulationDensity_  ( 1. )
    , rPopulationFactor_   ( 1. )
{
    InitializeAngle        ( xis );
    InitializeDistances    ( xis );

    xis >> xml::start( "distance-modifiers" );

    InitializeFactors( PHY_Volume       ::GetVolumes        (), "size-modifiers"  , volumeFactors_       , xis );
    InitializeFactors( PHY_Precipitation::GetPrecipitations (), "precipitation-modifiers", precipitationFactors_, xis );
    InitializeFactors( PHY_Lighting     ::GetLightings      (), "visibility-modifiers"  , lightingFactors_     , xis );
    InitializeFactors( PHY_Posture      ::GetPostures       (), "source-posture-modifiers", postureSourceFactors_, xis );
    InitializeFactors( PHY_Posture      ::GetPostures       (), "target-posture-modifiers" , postureTargetFactors_, xis );

    InitializeEnvironmentFactors( xis );
    InitializePopulationFactors ( xis );

    xis >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent destructor
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
PHY_SensorTypeAgent::~PHY_SensorTypeAgent()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::InitializeAngle
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
void PHY_SensorTypeAgent::InitializeAngle( xml::xistream& xis )
{
    xis >> xml::attribute( "angle", rAngle_ )
        >> xml::attribute( "scanning", bScanningAllowed_ );

    rAngle_ *= ( MT_PI / 180. );
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::InitializeDistances
// Created: NLD 2004-08-06
// Modified: JVT 2004-09-28
// -----------------------------------------------------------------------------
void PHY_SensorTypeAgent::InitializeDistances( xml::xistream& xis )
{
    xis >> xml::start( "base-distances" )
            >> xml::attribute( "close-range", rSquareProximityDist_ );
    rSquareProximityDist_ = MIL_Tools::ConvertMeterToSim( rSquareProximityDist_ );
    rSquareProximityDist_ *= rSquareProximityDist_;

    xis     >> xml::list( "base-distance", *this, &PHY_SensorTypeAgent::ReadDistance )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::ReadDistance
// Created: ABL 2007-07-25
// -----------------------------------------------------------------------------
void PHY_SensorTypeAgent::ReadDistance( xml::xistream& xis )
{
    std::string distanceType;
    xis >> xml::attribute( "level", distanceType );
    if( distanceType == "identification" )
    {
        xis >> xml::attribute( "distance", rIdentificationDist_ );
        if( rIdentificationDist_ < 0 )
            xis.error( "base-distance: identification distance < 0" );
        rIdentificationDist_ = MIL_Tools::ConvertMeterToSim( rIdentificationDist_ );
    }
    else if( distanceType == "recognition" )
    {
        xis >> xml::attribute( "distance", rRecognitionDist_ );
        if( rRecognitionDist_ < rIdentificationDist_ )
            xis.error( "base-distance: recognition distance < identification distance" );
        rRecognitionDist_ = MIL_Tools::ConvertMeterToSim( rRecognitionDist_ );
    }
    else if( distanceType == "detection" )
    {
        xis >> xml::attribute( "distance", rDetectionDist_ );
        if( rDetectionDist_ < rRecognitionDist_ )
            xis.error( "base-distance: detection distance < recognition distance" );
        rDetectionDist_ = MIL_Tools::ConvertMeterToSim( rDetectionDist_ );
    }
    else
        xis.error( "base-distance: unknow distance level" );
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::InitializeEnvironmentFactors
// Created: NLD 2004-08-06
// -----------------------------------------------------------------------------
void PHY_SensorTypeAgent::InitializeEnvironmentFactors( xml::xistream& xis )
{
    unsigned int visionObject = 0;
    xis >> xml::start( "terrain-modifiers" )
            >> xml::list( "distance-modifier", *this, &PHY_SensorTypeAgent::ReadTerrainModifier, visionObject )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::ReadTerrainModifier
// Created: ABL 2007-07-25
// -----------------------------------------------------------------------------
void PHY_SensorTypeAgent::ReadTerrainModifier( xml::xistream& xis, unsigned int& visionObject )
{
    assert( environmentFactors_.size() > visionObject );  // $$$$ _RC_ ABL 2007-07-27: use exception instead
    MT_Float& rFactor = environmentFactors_[ visionObject ];
    std::string terrainType;
    xis >> xml::attribute( "type", terrainType )
        >> xml::attribute( "value", rFactor );
    if( rFactor < 0 || rFactor > 1 )
        xis.error( "terrain-modifier: value not in [0..1]" );
    ++visionObject;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::InitializePopulationFactors
// Created: NLD 2005-10-27
// -----------------------------------------------------------------------------
    void PHY_SensorTypeAgent::InitializePopulationFactors( xml::xistream& xis )
{
    xis >> xml::start( "population-modifier" )
            >> xml::attribute( "density", rPopulationDensity_ )
            >> xml::attribute( "modifier", rPopulationFactor_ )
        >> xml::end();

    if( rPopulationDensity_ < 0 )
        xis.error( "population-modifier: density < 0" );
    if( rPopulationFactor_ < 0 || rPopulationFactor_ > 1 )
        xis.error( "population-modifier: modifier not in [0..1]" );
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::GetPopulationFactor
// Created: NLD 2005-10-28
// -----------------------------------------------------------------------------
inline
MT_Float PHY_SensorTypeAgent::GetPopulationFactor( MT_Float rDensity ) const
{
    if( rDensity == 0. || rPopulationDensity_ == 0. )
        return 1.;
    return std::min( 1., rPopulationFactor_ * rPopulationDensity_ / rDensity );
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::GetSourceFactor
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
MT_Float PHY_SensorTypeAgent::GetSourceFactor( const MIL_AgentPion& source ) const
{
    // Posture
    const PHY_RoleInterface_Posture& sourcePosture = source.GetRole< PHY_RoleInterface_Posture >();

    const uint nOldPostureIdx = sourcePosture.GetLastPosture   ().GetID();
    const uint nCurPostureIdx = sourcePosture.GetCurrentPosture().GetID();

    assert( postureSourceFactors_.size() > nOldPostureIdx );
    assert( postureSourceFactors_.size() > nCurPostureIdx );

    MT_Float rModificator =   postureSourceFactors_[ nOldPostureIdx ] + sourcePosture.GetPostureCompletionPercentage()
                          * ( postureSourceFactors_[ nCurPostureIdx ] - postureSourceFactors_[ nOldPostureIdx ] );

    // Elongation
    rModificator *= sourcePosture.GetElongationFactor();

    // Human factors
    rModificator *= source.GetRole< PHY_RoleInterface_HumanFactors >().GetSensorDistanceModificator();

    // Population
    const MT_Float rPopulationDensity = source.GetRole< PHY_RoleInterface_Population >().GetCollidingPopulationDensity();
    rModificator *= GetPopulationFactor( rPopulationDensity );

    return rModificator;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::GetTargetFactor
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
MT_Float PHY_SensorTypeAgent::GetTargetFactor( const MIL_Agent_ABC& target ) const
{
    const PHY_RoleInterface_Posture& targetPosture = target.GetRole< PHY_RoleInterface_Posture >();

    const uint nOldPostureIdx = targetPosture.GetLastPosture   ().GetID();
    const uint nCurPostureIdx = targetPosture.GetCurrentPosture().GetID();

    assert( postureTargetFactors_.size() > nOldPostureIdx );
    assert( postureTargetFactors_.size() > nCurPostureIdx );
    return postureTargetFactors_[ nOldPostureIdx ] + targetPosture.GetPostureCompletionPercentage() * ( postureTargetFactors_[ nCurPostureIdx ] - postureTargetFactors_[ nOldPostureIdx ] );
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::GetTargetFactor
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
MT_Float PHY_SensorTypeAgent::GetTargetFactor( const DEC_Knowledge_Agent& target ) const
{
    const uint nOldPostureIdx = target.GetLastPosture   ().GetID();
    const uint nCurPostureIdx = target.GetCurrentPosture().GetID();

    assert( postureTargetFactors_.size() > nOldPostureIdx );
    assert( postureTargetFactors_.size() > nCurPostureIdx );
    return postureTargetFactors_[ nOldPostureIdx ] + target.GetPostureCompletionPercentage() * ( postureTargetFactors_[ nCurPostureIdx ] - postureTargetFactors_[ nOldPostureIdx ] );
}

//-----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::ComputeEnvironementFactor
// Created: JVT 03-04-28
//-----------------------------------------------------------------------------
inline
MT_Float PHY_SensorTypeAgent::ComputeEnvironementFactor( PHY_RawVisionData::envBits nEnv ) const
{
    MT_Float res = nEnv & PHY_RawVisionData::eVisionEmpty ? environmentFactors_[ 0 ] : 1.;

    for( uint mask = 1, idx = 1; idx < PHY_RawVisionData::eNbrVisionObjects; mask <<= 1, ++idx )
        if( mask & nEnv )
            res *= environmentFactors_[ idx ];
    return res;
}

//-----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::ComputeExtinction
// Created: JVT 02-11-20
// Last modified: JVT 04-02-12
//-----------------------------------------------------------------------------
MT_Float PHY_SensorTypeAgent::ComputeExtinction( const PHY_RawVisionDataIterator& env, MT_Float rDistanceModificator, MT_Float rVisionNRJ ) const
{
    assert( rVisionNRJ <= rDetectionDist_ );
    assert( rVisionNRJ > 0 );

    // Prise en compte de l'�clairement
    rDistanceModificator *= lightingFactors_[ env.GetLighting().GetID() ];
    // Prise en compte des pr�cipitations
    rDistanceModificator *= precipitationFactors_ [ env.GetPrecipitation().GetID() ];
    // Prise en compte des objets
    rDistanceModificator *= ComputeEnvironementFactor( env.GetCurrentEnv() );
    return rDistanceModificator <= MT_Epsilon ? -1. : rVisionNRJ - env.Length() / rDistanceModificator ;
}

//-----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::InterpreteExtinction
// Created: JVT 02-07-12
// Last modified: JVT 03-01-27
//-----------------------------------------------------------------------------
inline
const PHY_PerceptionLevel& PHY_SensorTypeAgent::InterpreteExtinction( MT_Float rExtinction ) const
{
    if( rExtinction >= rDetectionDist_ - rIdentificationDist_ )
        return PHY_PerceptionLevel::identified_;
    if( rExtinction >= rDetectionDist_ - rRecognitionDist_ )
        return  PHY_PerceptionLevel::recognized_;
    if( rExtinction >= 0 )
         return PHY_PerceptionLevel::detected_;
    return PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::RayTrace
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_SensorTypeAgent::RayTrace( const MT_Vector2D& vSource, MT_Float rSourceAltitude, const MT_Vector2D& vTarget, MT_Float rTargetAltitude, MT_Float rDistanceMaxModificator ) const
{
    if( vSource.Distance( vTarget ) > GetMaxDistance() * rDistanceMaxModificator )
        return PHY_PerceptionLevel::notSeen_;

    const MT_Vector3D vSource3D( vSource.rX_, vSource.rY_, rSourceAltitude );
    const MT_Vector3D vTarget3D( vTarget.rX_, vTarget.rY_, rTargetAltitude );

    PHY_RawVisionDataIterator it( vSource3D, vTarget3D );
    MT_Float rVisionNRJ = it.End() ? std::numeric_limits< MT_Float >::max() : ComputeExtinction( it, rDistanceMaxModificator, rDetectionDist_ );

    while ( rVisionNRJ > 0 && !(++it).End() )
        rVisionNRJ = ComputeExtinction( it, rDistanceMaxModificator, rVisionNRJ );

    return InterpreteExtinction( rVisionNRJ );
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::ComputePerception
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_SensorTypeAgent::ComputePerception( const MIL_AgentPion& source, const MT_Vector2D& vTargetPos, MT_Float rSensorHeight ) const
{
    const MT_Float rDistanceMaxModificator = GetSourceFactor( source );

    const MT_Vector2D& vSourcePos      = source.GetRole< PHY_RoleInterface_Location >().GetPosition();
    const MT_Float     rSourceAltitude = source.GetRole< PHY_RoleInterface_Location >().GetAltitude() + rSensorHeight;
    const MT_Float     rTargetAltitude = MIL_Tools::GetAltitude( vTargetPos ) + 2;

    return RayTrace( vSourcePos, rSourceAltitude, vTargetPos, rTargetAltitude, rDistanceMaxModificator );
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::ComputePerception
// Created: NLD 2004-08-30
// Modified: JVT 2004-09-28
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_SensorTypeAgent::ComputePerception( const MIL_AgentPion& source, const MIL_Agent_ABC& target, MT_Float rSensorHeight ) const
{
    const MT_Vector2D& vTargetPos = target.GetRole< PHY_RoleInterface_Location >().GetPosition();

    if( target.IsDead() )
        return ComputePerception( source, vTargetPos, rSensorHeight );

    const PHY_Volume* pSignificantVolume = target.GetRole< PHY_RoleInterface_Composantes >().GetSignificantVolume( *this );
    if( !pSignificantVolume )
        return PHY_PerceptionLevel::notSeen_;

    MT_Float rDistanceMaxModificator  = GetFactor      ( *pSignificantVolume );
             rDistanceMaxModificator *= GetTargetFactor( target );
             rDistanceMaxModificator *= GetSourceFactor( source );

    const MT_Vector2D& vSourcePos      = source.GetRole< PHY_RoleInterface_Location >().GetPosition();
    const MT_Float     rSourceAltitude = source.GetRole< PHY_RoleInterface_Location >().GetAltitude() + rSensorHeight;
    const MT_Float     rTargetAltitude = target.GetRole< PHY_RoleInterface_Location >().GetAltitude() + 2;

    return RayTrace( vSourcePos, rSourceAltitude, vTargetPos, rTargetAltitude, rDistanceMaxModificator );
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::ComputePerception
// Created: NLD 2004-09-07
// Modified: JVT 2004-09-28
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_SensorTypeAgent::ComputePerception( const MIL_AgentPion& source, const DEC_Knowledge_Agent& target, MT_Float rSensorHeight ) const
{
    const MT_Vector2D& vTargetPos = target.GetPosition();

    if( target.IsDead() )
        return ComputePerception( source, vTargetPos, rSensorHeight );

    const PHY_Volume* pSignificantVolume = target.GetSignificantVolume( *this );
    if( !pSignificantVolume )
        return PHY_PerceptionLevel::notSeen_;

    MT_Float rDistanceMaxModificator  = GetFactor      ( *pSignificantVolume );
             rDistanceMaxModificator *= GetTargetFactor( target );
             rDistanceMaxModificator *= GetSourceFactor( source );

    const MT_Vector2D& vSourcePos      = source.GetRole< PHY_RoleInterface_Location >().GetPosition();
    const MT_Float     rSourceAltitude = source.GetRole< PHY_RoleInterface_Location >().GetAltitude() + rSensorHeight;
    const MT_Float     rTargetAltitude = target.GetAltitude() + 2;

    return RayTrace( vSourcePos, rSourceAltitude, vTargetPos, rTargetAltitude, rDistanceMaxModificator );
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::ComputePerception
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_SensorTypeAgent::ComputePerception( const MIL_AgentPion& source, const MIL_PopulationConcentration& target, MT_Float /*rSensorHeight*/ ) const
{
    const MT_Float     rDistanceMaxModificator = GetSourceFactor( source );
    const MT_Vector2D& vSourcePos              = source.GetRole< PHY_RoleInterface_Location >().GetPosition();

    if( rDistanceMaxModificator == 0. || !target.Intersect2DWithCircle( vSourcePos, rDetectionDist_ * rDistanceMaxModificator ) )
        return PHY_PerceptionLevel::notSeen_;
    return PHY_PerceptionLevel::identified_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::ComputePerceptionAccuracy
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
MT_Float PHY_SensorTypeAgent::ComputePerceptionAccuracy( const MIL_AgentPion& source, const MIL_PopulationFlow& /*target*/, MT_Float /*rSensorHeight*/ ) const
{
    const MT_Float rDistanceMaxModificator = GetSourceFactor( source );
    return rDetectionDist_ * rDistanceMaxModificator;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::ComputePerception
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& PHY_SensorTypeAgent::ComputePerception( const MIL_AgentPion& source, const MIL_PopulationFlow& target, MT_Float /*rSensorHeight*/, T_PointVector& shape ) const
{
    const MT_Float     rDistanceMaxModificator = GetSourceFactor( source );
    const MT_Vector2D& vSourcePos              = source.GetRole< PHY_RoleInterface_Location >().GetPosition();

    if( rDistanceMaxModificator == 0. )
        return PHY_PerceptionLevel::notSeen_;

    if( !target.Intersect2DWithCircle( vSourcePos, rDetectionDist_ * rDistanceMaxModificator, shape ) )
        return PHY_PerceptionLevel::notSeen_;
    return PHY_PerceptionLevel::identified_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::GetSquareProximityDistance
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
MT_Float PHY_SensorTypeAgent::GetSquareProximityDistance() const
{
    return rSquareProximityDist_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::GetMaxDistance
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
MT_Float PHY_SensorTypeAgent::GetMaxDistance() const
{
    return rDetectionDist_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::GetAngle
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
MT_Float PHY_SensorTypeAgent::GetAngle() const
{
    return rAngle_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::GetType
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
const PHY_SensorType& PHY_SensorTypeAgent::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::GetFactor
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
MT_Float PHY_SensorTypeAgent::GetFactor( const PHY_Volume& volume ) const
{
    assert( volumeFactors_.size() > volume.GetID() );
    return volumeFactors_[ volume.GetID() ];
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::CanScan
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
bool PHY_SensorTypeAgent::CanScan() const
{
    return bScanningAllowed_;
}

//-----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::ConvertEnvironementToObjectIdx
// Created: JVT 03-04-28
//-----------------------------------------------------------------------------
uint PHY_SensorTypeAgent::ConvertEnvironementToObjectIdx( PHY_RawVisionData::E_VisionObject obj )
{
    if ( obj == PHY_RawVisionData::eVisionEmpty )
        return 0;

    uint res = 1;
    for ( uint idx = 1; !( idx & obj ); idx <<= 1 )
        ++res;
    return res;
}

//-----------------------------------------------------------------------------
// Name: PHY_SensorTypeAgent::ConvertObjectIdxToEnvironnement
// Created: JVT 03-08-07
//-----------------------------------------------------------------------------
PHY_RawVisionData::E_VisionObject PHY_SensorTypeAgent::ConvertObjectIdxToEnvironnement( uint val )
{
    return (PHY_RawVisionData::E_VisionObject)( val ? 1 << ( val - 1 ) : 0 );
}
