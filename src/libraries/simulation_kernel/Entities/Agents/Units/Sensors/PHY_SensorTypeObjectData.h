// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/Sensors/PHY_SensorTypeObjectData.h $
// $Author: Nld $
// $Modtime: 29/11/04 18:00 $
// $Revision: 3 $
// $Workfile: PHY_SensorTypeObjectData.h $
//
// *****************************************************************************

#ifndef __PHY_SensorTypeObjectData_h_
#define __PHY_SensorTypeObjectData_h_

#include "Entities/Agents/Units/Postures/PHY_Posture.h"

class MIL_Object_ABC;
class DEC_Knowledge_Object;
class MIL_Agent_ABC;
class PHY_PerceptionLevel;

// =============================================================================
// @class  PHY_SensorTypeObjectData
// Created: JVT 2004-08-03
// =============================================================================
class PHY_SensorTypeObjectData : private boost::noncopyable
{
public:
    explicit PHY_SensorTypeObjectData( xml::xistream& xis );
    virtual ~PHY_SensorTypeObjectData();

    //! @name Tools
    //@{
    double GetMaxDistance() const;
    //@}

    //! @name Operations
    //@{
    const PHY_PerceptionLevel& ComputePerception( const MIL_Agent_ABC& perceiver, const MIL_Object_ABC&   target, double rSensorHeight ) const;
    const PHY_PerceptionLevel& ComputePerception( const MIL_Agent_ABC& perceiver, const DEC_Knowledge_Object& target, double rSensorHeight ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< double > T_FactorVector;
    //@}

private:
    //! @name Helpers
    //@{
    double GetPopulationFactor( double rDensity ) const;
    double GetSourceFactor( const MIL_Agent_ABC& source ) const;
    void InitializePopulationFactors( xml::xistream& xis );
    //@}

private:
    double rDD_;
    T_FactorVector postureSourceFactors_;

    // Population
    double rPopulationDensity_;
    double rPopulationFactor_;
};

#endif // __PHY_SensorTypeObjectData_h_
