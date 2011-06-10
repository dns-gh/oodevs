// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __weather_PHY_RawVisionData_ABC_h_
#define __weather_PHY_RawVisionData_ABC_h_

#include <geometry/Types.h>
#include <boost/shared_ptr.hpp>

namespace weather
{
    class Meteo;

// =============================================================================
/** @class  PHY_RawVisionData_ABC
    @brief  PHY Raw vision data declaration
*/
// Created: SLG 2010-03-18
// =============================================================================
class PHY_RawVisionData_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             PHY_RawVisionData_ABC() {}
    virtual ~PHY_RawVisionData_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void RegisterMeteoPatch  ( const geometry::Point2d& topLeft, const geometry::Point2d& bottomRight, boost::shared_ptr< Meteo > weather ) = 0;
    virtual void UnregisterMeteoPatch( const geometry::Point2d& topleft, const geometry::Point2d& bottomRight ) = 0;
    //@}
};

}

#endif // __weather_PHY_RawVisionData_ABC_h_
