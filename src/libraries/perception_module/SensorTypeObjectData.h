// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_PERCEPTION_SENSOR_TYPE_OBJECT_DATA_H
#define SWORD_PERCEPTION_SENSOR_TYPE_OBJECT_DATA_H

#include <vector>
#include <boost/noncopyable.hpp>

namespace xml
{
    class xistream;
}

class MIL_Object_ABC;
class DEC_Knowledge_Object;
class MT_Vector2D;

namespace sword
{
namespace wrapper
{
    class View;
}
namespace perception
{
    class PerceptionLevel;

// =============================================================================
// @class  SensorTypeObjectData
// Created: JVT 2004-08-03
// =============================================================================
class SensorTypeObjectData : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit SensorTypeObjectData( xml::xistream& xis );
    virtual ~SensorTypeObjectData();
    //@}

    //! @name Tools
    //@{
    double GetMaxDistance() const;
    //@}

    //! @name Operations
    //@{
    const PerceptionLevel& ComputePerception( const wrapper::View& perceiver, const MIL_Object_ABC& target, double rSensorHeight ) const;
    const PerceptionLevel& ComputePerception( const wrapper::View& perceiver, const DEC_Knowledge_Object& target, double rSensorHeight ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< double > T_FactorVector;
    //@}

private:
    //! @name Tools
    //@{
    void InitializeFactors( const std::string& strTagName, T_FactorVector& factors, xml::xistream& xis );
    double GetPopulationFactor        ( double rDensity ) const;
    double GetSourceFactor            ( const wrapper::View& source ) const;
    void     InitializePopulationFactors( xml::xistream& xis );
    template< typename Object >
    const PerceptionLevel& ComputePerception( const wrapper::View& perceiver, const Object& target, bool(*intersectWithCircle) ( const Object& object, const MT_Vector2D& center, double radius ) ) const;
    //@}

    //! @name Helpers
    //@{
    void ReadPosture( xml::xistream& xis, T_FactorVector& factors ) const;
    //@}

private:
    //! @name Member data
    //@{
    double       rDD_;
    T_FactorVector postureSourceFactors_;

    // Population
    double rPopulationDensity_;
    double rPopulationFactor_;
    //@}
};

}
}

#endif // SWORD_PERCEPTION_SENSOR_TYPE_OBJECT_DATA_H
