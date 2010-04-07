// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __WeatherLayer_ABC_h_
#define __WeatherLayer_ABC_h_

#include "clients_gui/Layer_ABC.h"

namespace weather
{
    class PHY_Meteo;
}

namespace gui
{

// =============================================================================
/** @class  WeatherLayer_ABC
    @brief  WeatherLayer_ABC
*/
// Created: HBD 2010-04-06
// =============================================================================
class WeatherLayer_ABC : public Layer_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             WeatherLayer_ABC() {}
    virtual ~WeatherLayer_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual const weather::PHY_Meteo* Pick( const geometry::Point2f& terrainCoordinates ) const = 0;
    //@}
};

}

#endif // __WeatherLayer_ABC_h_
