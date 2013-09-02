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

#include "MT_Tools/MT_String.h"

namespace xml
{
    class xistream;
}

class PHY_SensorTypeObject;
class PHY_SensorTypeAgent;
class PHY_SensorTypeDisaster;
class PHY_Sensor;
class ObjectTypeResolver_ABC;

// =============================================================================
// @class  PHY_SensorType
// Created: JVT 2004-08-03
// =============================================================================
class PHY_SensorType : private boost::noncopyable
{
public:
    //! @name Manager
    //@{
    static void Initialize( xml::xistream& xis, const ObjectTypeResolver_ABC& resolver);
    static void Terminate();

    static const PHY_SensorType* FindSensorType( const std::string& strType );
    static const PHY_SensorType* FindSensorType( const unsigned int nID );
    //@}

    //! @name Instanciation
    //@{
    PHY_Sensor& InstanciateSensor( double rHeight ) const;
    //@}

    //! @name Accessors
    //@{
    const PHY_SensorTypeObject*   GetTypeObject() const;
    const PHY_SensorTypeAgent*    GetTypeAgent() const;
    const PHY_SensorTypeDisaster* GetTypeDisaster() const;
    const std::string&            GetName() const;
          unsigned int            GetID() const;
    const unsigned int            GetDelay() const;
          bool                    IsActivatedOnRequest() const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, const PHY_SensorType* > T_SensorTypeMap;
    typedef T_SensorTypeMap::const_iterator              CIT_SensorTypeMap;
    //@}

private:
     PHY_SensorType( const std::string& strName, xml::xistream& xis, const ObjectTypeResolver_ABC& resolver );
    ~PHY_SensorType();

    //! @name Helpers
    //@{
    static void ReadSensor    ( xml::xistream& xis, const ObjectTypeResolver_ABC& resolver );
    void newSensorTypeAgent   ( xml::xistream& xis );
    void newSensorTypeObject  ( xml::xistream& xis, const ObjectTypeResolver_ABC& resolver );
    void newSensorTypeDisaster( xml::xistream& xis );
    //@}

private:
    //! @name Member Data
    //@{
    const unsigned int            nID_;
    const std::string             strName_;
    const PHY_SensorTypeObject*   pTypeObject_;
    const PHY_SensorTypeAgent*    pTypeAgent_;
    const PHY_SensorTypeDisaster* pTypeDisaster_;
    unsigned int                  delay_;
    bool                          activationOnRequest_;
    //@}

private:
    static T_SensorTypeMap sensorTypes_;
    static unsigned int nNextID_;
};

#endif // __PHY_SensorType_h_
