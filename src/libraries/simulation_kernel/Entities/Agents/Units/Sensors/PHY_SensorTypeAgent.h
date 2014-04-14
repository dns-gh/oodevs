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

#include "Meteo/RawVisionData/PHY_RawVisionData.h"
#include "MT_Tools/MT_Vector2DTypes.h"

class PHY_PerceptionLevel;
class MIL_Agent_ABC;
class MIL_PopulationConcentration;
class MIL_PopulationFlow;
class MIL_UrbanObject_ABC;
class PHY_SensorType;
class PHY_Volume;

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
    double GetSquareProximityDistance() const;
    double GetMaxDistance() const;
    double GetAngle() const;
    double GetVolumeFactor( const PHY_Volume& volume ) const;
    double GetUrbanBlockFactor( const MIL_UrbanObject_ABC& target ) const;
    bool CanScan() const;
    bool CanDetectFirer( double distance ) const;
    const PHY_SensorType& GetType() const;
    unsigned int GetDelay() const;
    //@}

    //! @name Operations
    //@{
    const PHY_PerceptionLevel& ComputePerception( const MIL_Agent_ABC& perceiver, const MT_Vector2D& target, double rSensorHeight ) const;
    const PHY_PerceptionLevel& ComputePerception( const MIL_Agent_ABC& perceiver, const MIL_Agent_ABC& target, double rSensorHeight ) const;
    const PHY_PerceptionLevel& ComputePerception( const MIL_Agent_ABC& perceiver, const MIL_PopulationConcentration& target, double rSensorHeight ) const;
    const PHY_PerceptionLevel& ComputePerception( const MIL_Agent_ABC& perceiver, const MIL_PopulationFlow& target, double rSensorHeight, T_PointVector& shape ) const;
    double ComputePerceptionAccuracy( const MIL_Agent_ABC& perceiver, const MIL_PopulationFlow& target, double rSensorHeight ) const;
    const double IdentificationDistance() const;
    const double ReconnoissanceDistance() const;
    void ComputeDistances( const MIL_Agent_ABC& perceiver, const MIL_Agent_ABC& target, double& identification, double& recognition, double& detection ) const;
    const double RayTrace( const MT_Vector2D& vSource, const MT_Vector2D& vTarget, double sensorHeight, bool posted ) const;
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
    void InitializePopulationFactors ( xml::xistream& xis );
    //@}

    //! @name Tools
    //@{
    PHY_RawVisionData::E_VisionObject ConvertObjectIdxToEnvironnement( unsigned int val );
    unsigned int ConvertEnvironmentToObjectIdx ( PHY_RawVisionData::E_VisionObject obj );

    const PHY_PerceptionLevel& RayTrace( const MT_Vector2D& vSource, double rSourceAltitude, const MT_Vector2D& vTarget, double rTargetAltitude, double rDistanceMaxModificator, bool posted ) const;
    double ComputeEnvironmentFactor( envBits nEnv ) const;
    double ComputeExtinction( const PHY_RawVisionDataIterator& env, double rDistanceModificator, double rInitialCoef, bool bIsAroundBU ) const;
    const double ComputeDistanceModificator( const MIL_Agent_ABC& perceiver, const MIL_Agent_ABC& target ) const;
    const PHY_PerceptionLevel& InterpretExtinction( double rExtinction ) const;

    double GetSourceFactor( const MIL_Agent_ABC& source ) const;
    double GetTargetFactor( const MIL_Agent_ABC& target ) const;

    bool IsLimitedToSensors( const MIL_Agent_ABC& target ) const;

    double GetPopulationFactor( double rDensity ) const;
    //@}

    //! @name Helpers
    //@{
    void ReadDistance            ( xml::xistream& xis );
    void ReadLimitedToSensorsList( xml::xistream& xis ); // LTO
    //@}

private:
    const PHY_SensorType& type_;

    double rAngle_;
    bool bScanningAllowed_;
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
    T_FactorVector urbanFactors_;

    // Population
    double rPopulationDensity_;
    double rPopulationFactor_;

    // Limitation aux autres capteurs
    // LTO begin
    bool                       isLimitedToSensors_;
    std::vector< std::string > limitedToSensorsList_;
    // LTO end
};

namespace boost
{
namespace archive
{
    template< class Archive >
    void save( Archive& ar, const PHY_SensorTypeAgent* t )
    {
        unsigned int id = t ? t->GetType().GetID() : std::numeric_limits< unsigned int >::max();
        ar << id;
    }
    template< class Archive >
    void load( Archive& ar, const PHY_SensorTypeAgent*& t )
    {
        unsigned int id;
        ar >> id;
        auto type = PHY_SensorType::FindSensorType( id );
        if( ! type )
        {
            t = 0;
            return;
        }
        t = type->GetTypeAgent();
    }
}
}

#endif // __PHY_SensorTypeAgent_h_
