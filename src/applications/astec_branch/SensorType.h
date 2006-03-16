//*****************************************************************************
// Created: JVT 02-08-30
//*****************************************************************************

#ifndef __SensorType_h_
#define __SensorType_h_

#include "Types.h"
#include "RawVisionData.h"

class Agent;
enum E_PerceptionResult;

namespace xml { class xistream; };

//*****************************************************************************
// Created: JVT 02-08-30
// Last modified: JVT 02-12-13
//*****************************************************************************
class SensorType
{
    MT_COPYNOTALLOWED( SensorType )

public:
    SensorType( xml::xistream& xis );
//    SensorType( const std::string& strName, InputArchive& archive );
    ~SensorType();

    //! @name Accessors
    //@{
    const std::string& GetName () const;
    float              GetAngle() const;
    //@}

    //! @name Operations
    //@{
    float GetMaxDistance   ( const Agent& agent ) const;
    float ComputeExtinction( const RawVisionData::Iterator& env, const Agent& srcAgent, MT_Float rCurrentNRJ ) const;
    float ComputeExtinction( const RawVisionData::Iterator& env, const Agent& srcAgent ) const;

    E_PerceptionResult InterpreteNRJ( float skyRock ) const;

    float GetDistanceModificator( const Agent& agent ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< float > T_FactorVector;
    //@}

private:
    //! @name 
    //@{
    void InitializePopulationFactors   ( InputArchive& archive );
    void InitializeDistances           ( InputArchive& archive );
    void InitializePostureSourceFactors( InputArchive& archive );
    void InitializeWeatherFactors      ( InputArchive& archive );
    void InitializeLightingFactors     ( InputArchive& archive );
    void InitializeEnvironnementFactors( InputArchive& archive );
    void InitializeAngle               ( InputArchive& archive );

    float GetPostureSourceFactor    ( const Agent& agent ) const;
    float ComputeEnvironementFactor ( RawVisionData::envBits nEnv ) const;
    //@}

private:
    std::string strName_;
    float rAngle_;
    bool     bScanningAllowed_;

    // Distances
    float rSquareProximityDist_;
    float rIdentificationDist_;
    float rRecognitionDist_;
    float rDetectionDist_;

    // Modificateurs
    T_FactorVector postureSourceFactors_;
    T_FactorVector lightingFactors_;
    T_FactorVector weatherFactors_;
    T_FactorVector environementFactors_;

    // Population
    float rPopulationDensity_;
    float rPopulationFactor_;

};

#include "SensorType.inl"

#endif // __SensorType_h_