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

#include "MIL.h"
#include "Entities/Agents/Units/Postures/PHY_Posture.h"

namespace xml
{
    class xistream;
}

class MIL_Object_ABC;
class DEC_Knowledge_Object;
class MIL_AgentPion;
class PHY_PerceptionLevel;
class PHY_RolePion_Posture;

// =============================================================================
// @class  PHY_SensorTypeObjectData
// Created: JVT 2004-08-03
// =============================================================================
class PHY_SensorTypeObjectData
{
    MT_COPYNOTALLOWED( PHY_SensorTypeObjectData )

public:
     PHY_SensorTypeObjectData( xml::xistream& xis );
    ~PHY_SensorTypeObjectData();

    //! @name Tools
    //@{
    MT_Float GetMaxDistance() const;
    //@}

    //! @name Operations
    //@{
    const PHY_PerceptionLevel& ComputePerception( const MIL_AgentPion& perceiver, const MIL_Object_ABC&   target, MT_Float rSensorHeight ) const;
    const PHY_PerceptionLevel& ComputePerception( const MIL_AgentPion& perceiver, const DEC_Knowledge_Object& target, MT_Float rSensorHeight ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< MT_Float > T_FactorVector;
    //@}

private:
    //! @name Tools
    //@{
    template< typename C > static void InitializeFactors( const C& container, const std::string& strTagName, T_FactorVector& factors, xml::xistream& xis );
    MT_Float GetPopulationFactor        ( MT_Float rDensity ) const;
    MT_Float GetSourceFactor            ( const MIL_AgentPion& source ) const;
    void     InitializePopulationFactors( xml::xistream& xis );
    //@}
    //! @name Hepers
    //@{
    struct LoadingWrapper;
    static void ReadPosture( xml::xistream& xis, const PHY_Posture::T_PostureMap& container, T_FactorVector& factors );
    //@}

private:
    MT_Float       rDD_;
    T_FactorVector postureSourceFactors_;

    // Population
    MT_Float rPopulationDensity_;
    MT_Float rPopulationFactor_;
};

#include "PHY_SensorTypeObjectData.inl"

#endif // __PHY_SensorTypeObjectData_h_
