// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Sensors/PHY_Sensor.h $
// $Author: Nld $
// $Modtime: 2/05/05 13:44 $
// $Revision: 2 $
// $Workfile: PHY_Sensor.h $
//
// *****************************************************************************

#ifndef __PHY_Sensor_h_
#define __PHY_Sensor_h_

#include "MIL.h"

class PHY_SensorType;

// =============================================================================
// @class  PHY_Sensor
// Created: JVT 2004-08-03
// =============================================================================
class PHY_Sensor
{
    MT_COPYNOTALLOWED( PHY_Sensor )

public:
     PHY_Sensor( const PHY_SensorType& type, MT_Float rHeight );
    ~PHY_Sensor();

    //! @name Accessors
    //@{
          MT_Float        GetHeight() const;
    const PHY_SensorType& GetType  () const;
    //@}

private:
    const PHY_SensorType& type_;
    const MT_Float        rHeight_;
};

#include "PHY_Sensor.inl"

#endif // __PHY_Sensor_h_
