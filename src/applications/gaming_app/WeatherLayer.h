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
#include <tools/ElementObserver_ABC.h>

namespace kernel
{
    class Controllers;
    class Profile_ABC;
}

namespace gui
{
    class ExclusiveEventStrategy;
    class GlTools_ABC;
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
class WeatherLayer : public gui::WeatherLayer
                   , public tools::ElementObserver_ABC< AmmoEffect >
{
public:
    //! @name Constructors/Destructor
    //@{
             WeatherLayer( gui::GlTools_ABC& tools, gui::ExclusiveEventStrategy& eventStrategy, kernel::Controllers& controllers, const MeteoModel& meteoModel, gui::TerrainPicker& pickerr, const kernel::Profile_ABC& profile );
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
    const MeteoModel& meteoModel_;
    const kernel::Profile_ABC& profile_;
    T_Effects effects_;
    std::unique_ptr< weather::Meteo > currentMeteo_;
    gui::TerrainPicker& picker_;
    //@}
};

#endif // __WeatherLayer_h_
