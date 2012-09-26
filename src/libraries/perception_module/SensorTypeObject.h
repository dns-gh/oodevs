// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_PERCEPTION_SENSOR_TYPE_OBJECT_H
#define SWORD_PERCEPTION_SENSOR_TYPE_OBJECT_H

#include <vector>
#include <boost/noncopyable.hpp>

namespace xml
{
    class xistream;
}

namespace sword
{
namespace wrapper
{
    class View;
}
namespace perception
{
    class PerceptionLevel;
    class SensorTypeObjectData;
    class SensorType;

// =============================================================================
// @class  SensorTypeObject
// Created: JVT 2004-08-03
// =============================================================================
class SensorTypeObject : private boost::noncopyable
{
public:
             SensorTypeObject( const SensorType& type, xml::xistream& xis );
    virtual ~SensorTypeObject();

    //! @name Accessors
    //@{
          double      GetMaxDistance() const;
    const SensorType& GetType()        const;
    //@}

    //! @name Operations
    //@{
    const PerceptionLevel& ComputeObjectPerception( const wrapper::View& perceiver, const wrapper::View& target, double rSensorHeight ) const;
    const PerceptionLevel& ComputeKnowledgeObjectPerception( const wrapper::View& perceiver, const wrapper::View& target, double rSensorHeight ) const;
    //@}

    //! @name Helpers
    //@{
    void ReadObject( xml::xistream& xis );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< const SensorTypeObjectData* > T_ObjectDataVector;
    typedef T_ObjectDataVector::iterator             CIT_ObjectDataVector;
    //@}

private:
    const SensorType&  type_;
    T_ObjectDataVector objectData_;
    double             rMaxDistance_;
};

}
}

#endif // SWORD_PERCEPTION_SENSOR_TYPE_OBJECT_H
