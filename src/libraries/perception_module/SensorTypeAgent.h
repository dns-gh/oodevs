// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_PERCEPTION_SENSOR_TYPE_AGENT_H
#define SWORD_PERCEPTION_SENSOR_TYPE_AGENT_H

#include <boost/noncopyable.hpp>
#include "MT_Tools/MT_Vector2D.h"
#include <vector>
#include <map>

namespace xml
{
    class xistream;
}

class MT_Vector2D;

namespace sword
{
namespace wrapper
{
    class View;
}
namespace perception
{
    class PerceptionLevel;
    class SensorType;

// =============================================================================
// @class  SensorTypeAgent
// Created: JVT 2004-08-03
// Modified: JVT 2004-09-28
// =============================================================================
class SensorTypeAgent : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             SensorTypeAgent( const SensorType& type, xml::xistream& xis );
    virtual ~SensorTypeAgent();
    //@}

    //! @name Accessors
    //@{
          double       GetSquareProximityDistance() const;
          double       GetMaxDistance            () const;
          double       GetAngle                  () const;
          double       GetFactor                 ( size_t identifier ) const;
          double       GetUrbanBlockFactor       ( const wrapper::View& target ) const;
          bool         CanScan                   () const;
          bool         CanDetectFirer            ( double distance ) const;
    const SensorType&  GetType                   () const;
          unsigned int GetDelay                  () const;
    //@}

    //! @name Operations
    //@{
    const PerceptionLevel& ComputePointPerception( const wrapper::View& perceiver, const MT_Vector2D& target, double rSensorHeight ) const;
    const PerceptionLevel& ComputeAgentPerception( const wrapper::View& perceiver, const wrapper::View& target, double rSensorHeight ) const;
    const PerceptionLevel& ComputeConcentrationPerception( const wrapper::View& perceiver, const wrapper::View& target, double rSensorHeight ) const;
    const PerceptionLevel& ComputeFlowPerception( const wrapper::View& perceiver, const wrapper::View& target, double rSensorHeight, std::vector< MT_Vector2D >& shape ) const;
    double ComputePerceptionAccuracy( const wrapper::View& perceiver, const wrapper::View& target, double rSensorHeight ) const;
    const double IdentificationDistance   () const;
    const double ReconnoissanceDistance   () const;
    void ComputeDistances( const wrapper::View& perceiver, const wrapper::View& target, double& identification, double& recognition, double& detection ) const;
    const double RayTrace                 ( const MT_Vector2D& vSource, const MT_Vector2D& vTarget, double sensorHeight ) const;
    //@}

public:
    //! @name Types
    //@{
    typedef std::vector< double > T_FactorVector;
    typedef std::map< std::size_t, double > T_FactorMap;
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
    const PerceptionLevel& RayTrace                 ( const MT_Vector2D& vSource, double rSourceAltitude, const MT_Vector2D& vTarget, double rTargetAltitude, double rDistanceMaxModificator ) const;
    bool                   ComputeGlobalUrbanExtinction( const MT_Vector2D& vSource, const MT_Vector2D& vTarget, double& rVisionNRJ ) const;
    const double           ComputeDistanceModificator( const wrapper::View& perceiver, const wrapper::View& target ) const;
    const PerceptionLevel& InterpretExtinction     ( double rExtinction ) const;

    double GetSourceFactor    ( const wrapper::View& source ) const;
    double GetTargetFactor    ( const wrapper::View& target ) const;

    bool IsLimitedToSensors( const wrapper::View& target ) const;

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
    const SensorType& type_;

    double rAngle_;
    bool   bScanningAllowed_;
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

}
}

#endif // SWORD_PERCEPTION_SENSOR_TYPE_AGENT_H
