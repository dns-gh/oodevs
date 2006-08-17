//*****************************************************************************
// Created: JVT 02-08-30
//*****************************************************************************

#ifndef __SensorType_h_
#define __SensorType_h_

//#include "Types.h"
//#include "Perception_Def.h"

class Agent_ABC;
enum E_PerceptionResult;

namespace xml { class xistream; };

//*****************************************************************************
// Created: JVT 02-08-30
// Last modified: JVT 02-12-13
//*****************************************************************************
class SensorType
{
public:
             SensorType( const std::string& name, xml::xistream& xis );
    virtual ~SensorType();

    //! @name Accessors
    //@{
    const std::string& GetName () const;
    float              GetAngle() const;
    //@}

    //! @name Operations
    //@{
    float GetMaxDistance   ( float distanceModificator ) const;
    float ComputeExtinction( float distanceModificator, float rCurrentNRJ, bool inForest, bool inTown, bool inGround, float distance ) const;
    float ComputeExtinction( float distanceModificator, bool inForest, bool inTown, bool inGround, float distance ) const;

    E_PerceptionResult InterpreteNRJ( float skyRock ) const;

    float GetDistanceModificator( const Agent_ABC& agent ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SensorType( const SensorType& );
    SensorType& operator=( const SensorType& );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< float > T_FactorVector;
    //@}

private:
    //! @name 
    //@{
    void InitializePopulationFactors   ( xml::xistream& xis );
    void InitializeDistances           ( xml::xistream& xis );
    void InitializePostureSourceFactors( xml::xistream& xis );
    void InitializeWeatherFactors      ( xml::xistream& xis );
    void InitializeLightingFactors     ( xml::xistream& xis );
    void InitializeEnvironnementFactors( xml::xistream& xis );
    void InitializeAngle               ( xml::xistream& xis );

    float GetPostureSourceFactor    ( const Agent_ABC& agent ) const;
    float ComputeEnvironementFactor ( bool inForest, bool inTown, bool inGround ) const;
    //@}

private:
    std::string strName_;
    float rAngle_;
    bool bScanningAllowed_;

    // Distances
    float rSquareProximityDist_;
    float rIdentificationDist_;
    float rRecognitionDist_;
    float rDetectionDist_;

    // Modificateurs
    T_FactorVector postureSourceFactors_;
    T_FactorVector lightingFactors_;
    T_FactorVector weatherFactors_;

    float factorInForest_;
    float factorInTown_;
    float factorInGround_;

    // Population
    float rPopulationDensity_;
    float rPopulationFactor_;

};

#endif // __SensorType_h_