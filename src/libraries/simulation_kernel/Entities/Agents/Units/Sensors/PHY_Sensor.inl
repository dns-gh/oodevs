// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Sensors/PHY_Sensor.inl $
// $Author: Nld $
// $Modtime: 19/10/04 13:54 $
// $Revision: 1 $
// $Workfile: PHY_Sensor.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_Sensor::GetHeight
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
inline
MT_Float PHY_Sensor::GetHeight() const
{
    return rHeight_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Sensor::GetType
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
inline
const PHY_SensorType& PHY_Sensor::GetType() const
{
    return type_;
}
