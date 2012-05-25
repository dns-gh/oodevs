// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef PHY_SensorTypeAgent_ABC_h
#define PHY_SensorTypeAgent_ABC_h

#include <boost/noncopyable.hpp>

class PHY_Volume;

// =============================================================================
/** @class  PHY_SensorTypeAgent_ABC
    @brief  PHY Sensor type agent definition
*/
// Created: SLI 2012-05-23
// =============================================================================
class PHY_SensorTypeAgent_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             PHY_SensorTypeAgent_ABC() {}
    virtual ~PHY_SensorTypeAgent_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual double GetFactor( const PHY_Volume& volume ) const = 0;
    //@}
};

#endif // PHY_SensorTypeAgent_ABC_h
