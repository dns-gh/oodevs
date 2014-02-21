//*****************************************************************************
// Created: JVT 02-08-30
//*****************************************************************************

#ifndef __SensorType_h_
#define __SensorType_h_

#include <boost/optional.hpp>

namespace xml
{
    class xistream;
};

namespace weather
{
    class Meteo;
}

namespace kernel
{
    class Agent_ABC;
    class Object_ABC;
    enum E_PerceptionResult;

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
    std::string GetName () const;
    float   GetAngle() const;
    const std::vector< float >& GetPostureSourceFactors() const;
    //@}

    //! @name Operations
    //@{
    float GetMaxDistance   ( float distanceModificator ) const;
    float ComputeExtinction( float distanceModificator, float rCurrentNRJ, bool inForest, bool inTown, bool inGround, float distance, const boost::optional< std::string >& material, const weather::Meteo* weather = 0 ) const;

    E_PerceptionResult InterpreteNRJ( float skyRock ) const;
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
    typedef std::map< std::string, float > T_MaterialFactorMap;
    //@}

private:
    //! @name
    //@{
    void InitializeDistances                ( xml::xistream& xis );
    void InitializeEnvironnementFactors     ( xml::xistream& xis );
    void InitializePostureSourceFactors     ( xml::xistream& xis );
    void InitializePrecipitationFactors     ( xml::xistream& xis );
    void InitializeLightingFactors          ( xml::xistream& xis );
    void ReadEnvironnementFactor            ( xml::xistream& xis );
    void ReadPostureFactor                  ( xml::xistream& xis );
    void ReadLightingModifier               ( xml::xistream& xis );
    void ReadPrecipitationModifier          ( xml::xistream& xis );
    void InitializeUrbanBlockMaterialFactors( xml::xistream& xis );
    void ReadUrbanBlockMaterialFactor       ( xml::xistream& xis );
    void InitializeAngle                    ( xml::xistream& xis );
    void ReadDistance                       ( xml::xistream& xis );

    float ComputeEnvironmentFactor          ( bool inForest, bool inTown, bool inGround ) const;
    bool ComputeUrbanExtinction             ( float& skyRock, float distance, const boost::optional< std::string >& material, const weather::Meteo* weather ) const;
    float GetWeatherModifier( const weather::Meteo* weather  ) const;
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
    std::map< std::string, float > precipitationFactors_;
    std::map< std::string, float > lightingFactors_;

    T_MaterialFactorMap urbanBlockFactors_;

    float factorInForest_;
    float factorInTown_;
    float factorInGround_;

    // Population
    float rPopulationDensity_;
    float rPopulationFactor_;

};

}

#endif // __SensorType_h_