//*****************************************************************************
// Created: JVT 02-08-30
//*****************************************************************************

#ifndef __MOS_SensorType_h_
#define __MOS_SensorType_h_

#include "MOS_Types.h"
#include "MOS_RawVisionData.h"

class MOS_Agent;
enum E_PerceptionResult;

//*****************************************************************************
// Created: JVT 02-08-30
// Last modified: JVT 02-12-13
//*****************************************************************************
class MOS_SensorType
{
    MT_COPYNOTALLOWED( MOS_SensorType )

public:
    MOS_SensorType( const std::string& strName, MT_InputArchive_ABC& archive );
    ~MOS_SensorType();

    //! @name Accessors
    //@{
    const std::string& GetName () const;
    MT_Float     GetAngle() const;
    //@}

    //! @name Operations
    //@{
    MT_Float GetMaxDistance   ( const MOS_Agent& agent ) const;
    MT_Float ComputeExtinction( const MOS_RawVisionData::Iterator& env, const MOS_Agent& srcAgent, MT_Float rCurrentNRJ ) const;
    MT_Float ComputeExtinction( const MOS_RawVisionData::Iterator& env, const MOS_Agent& srcAgent ) const;

    E_PerceptionResult InterpreteNRJ( MT_Float rNRJ ) const;

    MT_Float GetDistanceModificator( const MOS_Agent& agent ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< MT_Float > T_FactorVector;
    //@}

private:
    //! @name 
    //@{
    void InitializeDistances           ( MT_InputArchive_ABC& archive );
    void InitializePostureSourceFactors( MT_InputArchive_ABC& archive );
    void InitializeWeatherFactors      ( MT_InputArchive_ABC& archive );
    void InitializeLightingFactors     ( MT_InputArchive_ABC& archive );
    void InitializeEnvironnementFactors( MT_InputArchive_ABC& archive );
    void InitializeAngle               ( MT_InputArchive_ABC& archive );

    MT_Float GetPostureSourceFactor    ( const MOS_Agent& agent ) const;
    MT_Float ComputeEnvironementFactor ( MOS_RawVisionData::envBits nEnv ) const;
    //@}

private:
    const std::string strName_;
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
};

#include "MOS_SensorType.inl"

#endif // __MOS_SensorType_h_