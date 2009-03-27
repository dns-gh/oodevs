// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/entities/agents/units/sensors/PHY_SensorTypeObject.h $
// $Author: Jvt $
// $Modtime: 12/04/05 10:11 $
// $Revision: 3 $
// $Workfile: PHY_SensorTypeObject.h $
//
// *****************************************************************************

#ifndef __PHY_SensorTypeObject_h_
#define __PHY_SensorTypeObject_h_

#include "MIL.h"

namespace xml
{
    class xistream;
}

class MIL_Object_ABC;
class DEC_Knowledge_Object;
class MIL_AgentPion;
class PHY_PerceptionLevel;
class PHY_SensorTypeObjectData;
class PHY_SensorType;

// =============================================================================
// @class  PHY_SensorTypeObject
// Created: JVT 2004-08-03
// =============================================================================
class PHY_SensorTypeObject
{
    MT_COPYNOTALLOWED( PHY_SensorTypeObject )

public:
     PHY_SensorTypeObject( const PHY_SensorType& type, xml::xistream& xis );
    ~PHY_SensorTypeObject();

    //! @name Accessors
    //@{
          MT_Float        GetMaxDistance() const;
    const PHY_SensorType& GetType()        const;
    //@}

    //! @name Operations
    //@{
    const PHY_PerceptionLevel& ComputePerception( const MIL_AgentPion& perceiver, const MIL_Object_ABC&   target, MT_Float rSensorHeight ) const;
    const PHY_PerceptionLevel& ComputePerception( const MIL_AgentPion& perceiver, const DEC_Knowledge_Object& target, MT_Float rSensorHeight ) const;
    //@}

    //! @name Helpers
    //@{
    void ReadObject( xml::xistream& xis );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< const PHY_SensorTypeObjectData* > T_ObjectDataVector;
    typedef T_ObjectDataVector::iterator                   CIT_ObjectDataVector;
    //@}

private:
    const PHY_SensorType& type_;
    T_ObjectDataVector    objectData_;
    MT_Float              rMaxDistance_;
};

#include "PHY_SensorTypeObject.inl"

#endif // __PHY_SensorTypeObject_h_
