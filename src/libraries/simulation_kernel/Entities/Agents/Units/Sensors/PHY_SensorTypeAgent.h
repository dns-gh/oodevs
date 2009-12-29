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
    ~PHY_SensorTypeAgent();

    //! @name Accessors
    //@{
          MT_Float        GetSquareProximityDistance() const;
          MT_Float        GetMaxDistance            () const;
          MT_Float        GetAngle                  () const;
          MT_Float        GetFactor                 ( const PHY_Volume& volume ) const;
          bool            CanScan                   () const;
    const PHY_SensorType& GetType                   () const;
    //@}

    //! @name Operations
    //@{
    const PHY_PerceptionLevel& ComputePerception        ( const MIL_AgentPion& perceiver, const MT_Vector2D&                 target, MT_Float rSensorHeight ) const;
    const PHY_PerceptionLevel& ComputePerception        ( const MIL_AgentPion& perceiver, const MIL_Agent_ABC&               target, MT_Float rSensorHeight ) const;
    const PHY_PerceptionLevel& ComputePerception        ( const MIL_AgentPion& perceiver, const DEC_Knowledge_Agent&         target, MT_Float rSensorHeight ) const;
    const PHY_PerceptionLevel& ComputePerception        ( const MIL_AgentPion& perceiver, const MIL_PopulationConcentration& target, MT_Float rSensorHeight ) const;    
    const PHY_PerceptionLevel& ComputePerception        ( const MIL_AgentPion& perceiver, const MIL_PopulationFlow&          target, MT_Float rSensorHeight, T_PointVector& shape ) const;
          MT_Float             ComputePerceptionAccuracy( const MIL_AgentPion& perceiver, const MIL_PopulationFlow&          target, MT_Float rSensorHeight ) const;
    const PHY_PerceptionLevel& ComputePerception        ( const MIL_AgentPion& perceiver, const urban::TerrainObject_ABC& target, MT_Float rSensorHeight ) const;
    //@}

public:
    //! @name Types
    //@{
    typedef std::vector< MT_Float > T_FactorVector;
    //@}

private:
    //! @name Init
    //@{
    void InitializeAngle             ( xml::xistream& xis );
    void InitializeDistances         ( xml::xistream& xis );
    void InitializeEnvironmentFactors( xml::xistream& xis );
    void InitializePopulationFactors ( xml::xistream& xis );
    //@}

    //! @name Tools
    //@{
    PHY_RawVisionData::E_VisionObject ConvertObjectIdxToEnvironnement( uint val );
    uint                              ConvertEnvironementToObjectIdx ( PHY_RawVisionData::E_VisionObject obj );

    const PHY_PerceptionLevel& RayTrace                 ( const MT_Vector2D& vSource, MT_Float rSourceAltitude, const MT_Vector2D& vTarget, MT_Float rTargetAltitude, MT_Float rDistanceMaxModificator ) const;
    MT_Float                   ComputeEnvironementFactor( PHY_RawVisionData::envBits nEnv ) const;
    MT_Float                   ComputeExtinction        ( const PHY_RawVisionDataIterator& env, MT_Float rDistanceModificator, MT_Float rInitialCoef ) const;
    const PHY_PerceptionLevel& InterpreteExtinction     ( MT_Float rExtinction ) const;
    const PHY_PerceptionLevel& InterpreteTerrainObjectExtinction( MT_Float rExtinction ) const;

    MT_Float GetSourceFactor( const MIL_AgentPion&       source ) const;
    MT_Float GetTargetFactor( const MIL_Agent_ABC&       target ) const;
    MT_Float GetTargetFactor( const DEC_Knowledge_Agent& target ) const;

    MT_Float GetPopulationFactor( MT_Float rDensity ) const;
    //@}

    //! @name Helpers
    //@{
    void ReadDistance       ( xml::xistream& xis );
    void ReadTerrainModifier( xml::xistream& xis, unsigned int& visionObject );
    //@}

private:
    const PHY_SensorType& type_;
    
    MT_Float rAngle_;
    bool     bScanningAllowed_;

    // Distances
    MT_Float rSquareProximityDist_;
    MT_Float rIdentificationDist_;
    MT_Float rRecognitionDist_;
    MT_Float rDetectionDist_;

    // Modificateurs
    T_FactorVector volumeFactors_;
    T_FactorVector precipitationFactors_;
    T_FactorVector lightingFactors_;
    T_FactorVector postureSourceFactors_;
    T_FactorVector postureTargetFactors_;
    T_FactorVector environmentFactors_;   

    // Population
    MT_Float rPopulationDensity_;
    MT_Float rPopulationFactor_;    
};

#endif // __PHY_SensorTypeAgent_h_
