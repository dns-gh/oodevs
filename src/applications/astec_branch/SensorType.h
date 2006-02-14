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
    MT_Float     GetAngle() const;
    //@}

    //! @name Operations
    //@{
    MT_Float GetMaxDistance   ( const Agent& agent ) const;
    MT_Float ComputeExtinction( const RawVisionData::Iterator& env, const Agent& srcAgent, MT_Float rCurrentNRJ ) const;
    MT_Float ComputeExtinction( const RawVisionData::Iterator& env, const Agent& srcAgent ) const;

    E_PerceptionResult InterpreteNRJ( MT_Float rNRJ ) const;

    MT_Float GetDistanceModificator( const Agent& agent ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< MT_Float > T_FactorVector;
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

    MT_Float GetPostureSourceFactor    ( const Agent& agent ) const;
    MT_Float ComputeEnvironementFactor ( RawVisionData::envBits nEnv ) const;
    //@}

private:
    std::string strName_;
    MT_Float rAngle_;
    bool     bScanningAllowed_;

    // Distances
    MT_Float rSquareProximityDist_;
    MT_Float rIdentificationDist_;
    MT_Float rRecognitionDist_;
    MT_Float rDetectionDist_;

    // Modificateurs
    T_FactorVector postureSourceFactors_;
    T_FactorVector lightingFactors_;
    T_FactorVector weatherFactors_;
    T_FactorVector environementFactors_;

    // Population
    MT_Float rPopulationDensity_;
    MT_Float rPopulationFactor_;

};

#include "SensorType.inl"

#endif // __SensorType_h_