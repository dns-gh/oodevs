// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Sensors/PHY_SensorType.h $
// $Author: Jvt $
// $Modtime: 14/04/05 16:11 $
// $Revision: 3 $
// $Workfile: PHY_SensorType.h $
//
// *****************************************************************************

#ifndef __PHY_SensorType_h_
#define __PHY_SensorType_h_

#include "MIL.h"

class PHY_SensorTypeObject;
class PHY_SensorTypeAgent;
class PHY_Sensor;

// =============================================================================
// @class  PHY_SensorType
// Created: JVT 2004-08-03
// =============================================================================
class PHY_SensorType
{
    MT_COPYNOTALLOWED( PHY_SensorType )

public:
    //! @name Manager
    //@{
    static void Initialize( MIL_InputArchive& archive );
    static void Terminate ();

    static const PHY_SensorType* FindSensorType( const std::string& strType );
    static const PHY_SensorType* FindSensorType( const uint         nID     );
    //@}

    //! @name Instanciation
    //@{
    PHY_Sensor& InstanciateSensor( MT_Float rHeight ) const;
    //@}

    //! @name Accessors
    //@{
    const PHY_SensorTypeObject* GetTypeObject() const;
    const PHY_SensorTypeAgent*  GetTypeAgent () const;
    const std::string&          GetName      () const;
          uint                  GetID        () const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, const PHY_SensorType*, sCaseInsensitiveLess > T_SensorTypeMap;
    typedef T_SensorTypeMap::const_iterator                                      CIT_SensorTypeMap;
    //@}

private:
     PHY_SensorType( const std::string& strName, MIL_InputArchive& archive );
    ~PHY_SensorType();
    
private:
    const uint                  nID_;
    const std::string           strName_;
    const PHY_SensorTypeObject* pTypeObject_;
    const PHY_SensorTypeAgent*  pTypeAgent_;

private:
    static T_SensorTypeMap sensorTypes_;
    static uint            nNextID_;
};

#include "PHY_SensorType.inl"

#endif // __PHY_SensorType_h_
