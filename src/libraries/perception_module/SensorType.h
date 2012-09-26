// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_PERCEPTION_SENSOR_TYPE_H
#define SWORD_PERCEPTION_SENSOR_TYPE_H

#include <string>
#include <map>
#include <boost/noncopyable.hpp>

namespace xml
{
    class xistream;
}

namespace sword
{
namespace perception
{
    class SensorTypeObject;
    class SensorTypeAgent;
    class Sensor;

// =============================================================================
/** @class  SensorType
    @brief  Sensor Type
*/
// Created: SLI 2012-04-12
// =============================================================================
class SensorType : private boost::noncopyable
{
public:
    //! @name Manager
    //@{
    static void Initialize( xml::xistream& xis );
    static void Terminate ();

    static const SensorType* FindSensorType( const std::string& strType );
    static const SensorType* FindSensorType( const unsigned int         nID     );
    //@}

    //! @name Instanciation
    //@{
    Sensor& InstanciateSensor( double rHeight ) const;
    //@}

    //! @name Accessors
    //@{
    const SensorTypeObject* GetTypeObject() const;
    const SensorTypeAgent*  GetTypeAgent () const;
    const std::string&      GetName      () const;
          unsigned int      GetID        () const;
    const unsigned int      GetDelay     () const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, const SensorType* > T_SensorTypeMap;
    typedef T_SensorTypeMap::const_iterator              CIT_SensorTypeMap;
    //@}

private:
    //! @name Constructors/Destructor
    //@{
     SensorType( const std::string& strName, xml::xistream& xis );
    ~SensorType();
    //@}

    //! @name Helpers
    //@{
    static void ReadSensor  ( xml::xistream& xis );
    void newSensorTypeAgent ( xml::xistream& xis );
    void newSensorTypeObject( xml::xistream& xis );
    //@}

private:
    //! @name Data members
    //@{
    const unsigned int      nID_;
    const std::string       strName_;
    const SensorTypeObject* pTypeObject_;
    const SensorTypeAgent*  pTypeAgent_;
    unsigned int            delay_;
    static T_SensorTypeMap sensorTypes_;
    static unsigned int    nNextID_;
    //@}
};

}
}

#endif // SWORD_PERCEPTION_SENSOR_TYPE_H
