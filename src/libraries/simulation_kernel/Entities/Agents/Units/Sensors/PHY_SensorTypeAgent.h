// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/Sensors/PHY_SensorTypeAgent.h $
// $Author: Nld $
// $Modtime: 29/11/04 17:57 $
// $Revision: 5 $
// $Workfile: PHY_SensorTypeAgent.h $
//
// *****************************************************************************

#ifndef __PHY_SensorTypeAgent_h_
#define __PHY_SensorTypeAgent_h_

#include "Entities/Agents/Units/Categories/PHY_Volume.h"
#include "Meteo/RawVisionData/PHY_RawVisionData.h"
#include "MT_Tools/MT_Vector2DTypes.h"

namespace xml
{
    class xistream;
}

class PHY_PerceptionLevel;
class PHY_RoleInterface_Posture;
class PHY_RoleInterface_Posture;
class MIL_AgentPion;
class MIL_Agent_ABC;
class MIL_PopulationConcentration;
class MIL_PopulationFlow;
class DEC_Knowledge_Agent;
class PHY_SensorType;

namespace urban
{
    class TerrainObject_ABC;
}
// =============================================================================
// @class  PHY_SensorTypeAgent
// Created: JVT 2004-08-03
// Modified: JVT 2004-09-28
// =============================================================================
class PHY_SensorTypeAgent : private boost::noncopyable
{
public:
     PHY_SensorTypeAgent( const PHY_SensorType& type, xml::xistream& xis );
    virtual ~PHY_SensorTypeAgent();

    //! @name Accessors
    //@{
          double        GetSquareProximityDistance() const;
          double        GetMaxDistance            () const;
          double        GetAngle                  () const;
          double        GetFactor                 ( const PHY_Volume& volume ) const;
          double GetUrbanBlockFactor( const urban::TerrainObject_ABC& target ) const;
          bool            CanScan                   () const;
          bool            CanDetectFirer            ( double distance ) const;
    const PHY_SensorType& GetType                   () const;
          unsigned int    GetDelay                  () const;
    //@}

    //! @name Operations
    //@{
    const PHY_PerceptionLevel& ComputePerception( const MIL_Agent_ABC& perceiver, const MT_Vector2D& target, double rSensorHeight ) const;
    const PHY_PerceptionLevel& ComputePerception( const MIL_Agent_ABC& perceiver, const MIL_Agent_ABC& target, double rSensorHeight ) const;
    const PHY_PerceptionLevel& ComputePerception( const MIL_Agent_ABC& perceiver, const DEC_Knowledge_Agent& target, double rSensorHeight ) const;
    const PHY_PerceptionLevel& ComputePerception( const MIL_Agent_ABC& perceiver, const MIL_PopulationConcentration& target, double rSensorHeight ) const;
    const PHY_PerceptionLevel& ComputePerception( const MIL_Agent_ABC& perceiver, const MIL_PopulationFlow& target, double rSensorHeight, T_PointVector& shape ) const;
    double ComputePerceptionAccuracy( const MIL_Agent_ABC& perceiver, const MIL_PopulationFlow& target, double rSensorHeight ) const;
    const double IdentificationDistance   () const;
    const double ReconnoissanceDistance   () const;
    const double ComputeIdentificationDist( const MIL_Agent_ABC& perceiver, const MIL_Agent_ABC& target ) const;
    const double ComputeRecognitionDist   ( const MIL_Agent_ABC& perceiver, const MIL_Agent_ABC& target ) const;
    const double ComputeDetectionDist     ( const MIL_Agent_ABC& perceiver, const MIL_Agent_ABC& target ) const;
    const double RayTrace                 ( const MT_Vector2D& vSource, const MT_Vector2D& vTarget, double sensorHeight ) const;
    //@}

public:
    //! @name Types
    //@{
    typedef std::vector< double > T_FactorVector;
    typedef std::map< unsigned, double > T_FactorMap;
    //@}

private:
    //! @name Init
    //@{
    void InitializeAngle             ( xml::xistream& xis );
    void InitializeLimitedToSensors  ( xml::xistream& xis ); // LTO
    void InitializeDistances         ( xml::xistream& xis );
    void InitializeEnvironmentFactors( xml::xistream& xis );
    void InitializePopulationFactors ( xml::xistream& xis );
    void InitializeUrbanBlockFactors ( xml::xistream& xis );
    //@}

    //! @name Tools
    //@{
    PHY_RawVisionData::E_VisionObject ConvertObjectIdxToEnvironnement( unsigned int val );
    unsigned int                              ConvertEnvironementToObjectIdx ( PHY_RawVisionData::E_VisionObject obj );

    const PHY_PerceptionLevel& RayTrace                 ( const MT_Vector2D& vSource, double rSourceAltitude, const MT_Vector2D& vTarget, double rTargetAltitude, double rDistanceMaxModificator ) const;
    double                   ComputeEnvironementFactor( PHY_RawVisionData::envBits nEnv ) const;
    double                   ComputeExtinction        ( const PHY_RawVisionDataIterator& env, double rDistanceModificator, double rInitialCoef, bool bIsAroundBU ) const;
    bool                       ComputeUrbanExtinction   ( const MT_Vector2D& vSource, const MT_Vector2D& vTarget, double& rVisionNRJ ) const;
    bool                       ComputeGlobalUrbanExtinction( const MT_Vector2D& vSource, const MT_Vector2D& vTarget, double& rVisionNRJ ) const;
    const double             ComputeDistanceModificator( const MIL_Agent_ABC& perceiver, const MIL_Agent_ABC& target ) const;
    const PHY_PerceptionLevel& InterpretExtinction     ( double rExtinction ) const;

    double GetSourceFactor    ( const MIL_Agent_ABC&       source ) const;
    double GetTargetFactor    ( const MIL_Agent_ABC&       target ) const;
    double GetTargetFactor    ( const DEC_Knowledge_Agent& target ) const;

    bool ContainsSensorFromLimitedList( const MIL_Agent_ABC& target ) const; // LTO

    double GetPopulationFactor( double rDensity ) const;
    //@}

    //! @name Helpers
    //@{
    void ReadDistance            ( xml::xistream& xis );
    void ReadLimitedToSensorsList( xml::xistream& xis ); // LTO
    void ReadTerrainModifier     ( xml::xistream& xis );
    void ReadUrbanBlockModifier  ( xml::xistream& xis, unsigned int& visionUrbanBlockMaterial );
    //@}

private:
    const PHY_SensorType& type_;

    double rAngle_;
    bool     bScanningAllowed_;
    double rRecognitionFirerDist_;

    // Distances
    double rSquareProximityDist_;
    double rIdentificationDist_;
    double rRecognitionDist_;
    double rDetectionDist_;

    // Modificateurs
    T_FactorVector volumeFactors_;
    T_FactorVector precipitationFactors_;
    T_FactorVector lightingFactors_;
    T_FactorVector postureSourceFactors_;
    T_FactorVector postureTargetFactors_;
    T_FactorMap    environmentFactors_;
    T_FactorVector urbanBlockFactors_;

    // Population
    double rPopulationDensity_;
    double rPopulationFactor_;

    // Limitation aux autres capteurs
    // LTO begin
    bool                       isLimitedToSensors_;
    std::vector< std::string > limitedToSensorsList_;
    // LTO end
};

#endif // __PHY_SensorTypeAgent_h_
