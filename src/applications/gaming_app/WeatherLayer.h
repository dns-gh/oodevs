// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __WeatherLayer_h_
#define __WeatherLayer_h_

#include "clients_gui/WeatherLayer_ABC.h"
#include "tools/ElementObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class GlTools_ABC;
}

namespace gui
{
    class TerrainPicker;
}

class AmmoEffect;
class MeteoModel;

// =============================================================================
/** @class  WeatherLayer
    @brief  WeatherLayer
*/
// Created: AGE 2006-04-04
// =============================================================================
class WeatherLayer : public gui::WeatherLayer_ABC
                   , public tools::Observer_ABC
                   , public tools::ElementObserver_ABC< AmmoEffect >
{
public:
    //! @name Constructors/Destructor
    //@{
             WeatherLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, gui::TerrainPicker& picker, const MeteoModel& meteoModel );
    virtual ~WeatherLayer();
    //@}

    //! @name Operations
    //@{
    virtual void                      Paint( const geometry::Rectangle2f& );
    virtual const weather::PHY_Meteo* Pick ( const geometry::Point2f& terrainCoordinates ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    WeatherLayer( const WeatherLayer& );            //!< Copy constructor
    WeatherLayer& operator=( const WeatherLayer& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyCreated( const AmmoEffect& effect );
    virtual void NotifyDeleted( const AmmoEffect& effect );
    //@}

    //! @name Types
    //@{
    typedef std::vector< const AmmoEffect* >  T_Effects;
    typedef T_Effects::iterator              IT_Effects;
    typedef T_Effects::const_iterator       CIT_Effects;
    //@}

private:
    //! @name Member data
    //@{
    gui::TerrainPicker&       picker_;
    kernel::Controllers& controllers_;
    const kernel::GlTools_ABC& tools_;
    T_Effects effects_;
    const MeteoModel& meteoModel_;
    //@}
};

#endif // __WeatherLayer_h_
