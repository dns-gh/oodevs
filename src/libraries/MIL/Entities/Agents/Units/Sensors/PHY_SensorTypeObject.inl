// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Sensors/PHY_SensorTypeObject.inl $
// $Author: Jvt $
// $Modtime: 12/04/05 10:11 $
// $Revision: 2 $
// $Workfile: PHY_SensorTypeObject.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeObject::GetMaxDistance
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
MT_Float PHY_SensorTypeObject::GetMaxDistance() const
{
    return rMaxDistance_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorTypeObject::GetType
// Created: JVT 2005-04-12
// -----------------------------------------------------------------------------
inline
const PHY_SensorType& PHY_SensorTypeObject::GetType() const
{
    return type_;
}
