// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __WeatherLayer_h_
#define __WeatherLayer_h_

#include "clients_gui/WeatherLayer.h"
#include "tools/ElementObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class GlTools_ABC;
}

namespace gui
{
    class TerrainPicker;
    class ExclusiveEventStrategy;
}

class AmmoEffect;
class MeteoModel;

// =============================================================================
/** @class  WeatherLayer
    @brief  WeatherLayer
*/
// Created: AGE 2006-04-04
// =============================================================================
class WeatherLayer : public gui::WeatherLayer
                   , public tools::Observer_ABC
                   , public tools::ElementObserver_ABC< AmmoEffect >
{
public:
    //! @name Constructors/Destructor
    //@{
             WeatherLayer( kernel::GlTools_ABC& tools, gui::ExclusiveEventStrategy& eventStrategy, kernel::Controllers& controllers, const MeteoModel& meteoModel, gui::TerrainPicker& picker );
    virtual ~WeatherLayer();
    //@}

    //! @name Operations
    //@{
    virtual void                  Paint( const geometry::Rectangle2f& );
    virtual const weather::Meteo* Pick ( const geometry::Point2f& terrainCoordinates ) const;
    //@}

private:
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
    kernel::Controllers& controllers_;
    const MeteoModel&    meteoModel_;
    T_Effects            effects_;
    weather::Meteo*      currentMeteo_;
    //@}
};

#endif // __WeatherLayer_h_
