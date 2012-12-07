// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __PHY_SensorTypeDisaster_h_
#define __PHY_SensorTypeDisaster_h_

#include <map>
#include <string>

namespace xml
{
    class xistream;
}
class PHY_PerceptionLevel;
class DEC_Knowledge_Object;
class MIL_Object_ABC;
class MT_Vector2D;

// =============================================================================
/** @class  PHY_SensorTypeDisaster
    @brief  PHY Sensor type disaster
*/
// Created: LGY 2012-12-05
// =============================================================================
class PHY_SensorTypeDisaster : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit PHY_SensorTypeDisaster( xml::xistream& xis );
    virtual ~PHY_SensorTypeDisaster();
    //@}

    //! @name Operations
    //@{
    const PHY_PerceptionLevel& ComputePerception( const MT_Vector2D& position, const MIL_Object_ABC& object ) const;
    const PHY_PerceptionLevel& ComputePerception( const MT_Vector2D& position, const DEC_Knowledge_Object& knowledge ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void ReadDisaster( xml::xistream& xis );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, float > T_disasters;
    //@}

private:
    //! @name Member Data
    //@{
    T_disasters disasters_;
    //@}
};

#endif // __PHY_SensorTypeDisaster_h_
