// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_PERCEPTION_SENSOR_H
#define SWORD_PERCEPTION_SENSOR_H

#include <boost/noncopyable.hpp>

namespace sword
{
namespace perception
{
    class SensorType;

// =============================================================================
/** @class  Sensor
    @brief  Sensor
*/
// Created: SLI 2012-04-12
// =============================================================================
class Sensor : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Sensor( const SensorType& type, double rHeight );
    virtual ~Sensor();
    //@}

    //! @name Accessors
    //@{
          double        GetHeight() const;
    const SensorType& GetType  () const;
    //@}

private:
    //! @name Data members
    //@{
    const SensorType& type_;
    const double      rHeight_;
    //@}
};

}
}

#endif // SWORD_PERCEPTION_SENSOR_H
