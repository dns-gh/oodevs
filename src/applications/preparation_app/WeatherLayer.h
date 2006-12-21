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

#include "clients_gui/Layer_ABC.h"

namespace kernel
{
    class GlTools_ABC;
}

namespace gui
{
    class ExclusiveEventStrategy;
}

class LocalWeather;

// =============================================================================
/** @class  WeatherLayer
    @brief  Weather layer
*/
// Created: SBO 2006-12-21
// =============================================================================
class WeatherLayer : public gui::Layer_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             WeatherLayer( kernel::GlTools_ABC& tools, gui::ExclusiveEventStrategy& eventStrategy );
    virtual ~WeatherLayer();
    //@}

    //! @name Operations
    //@{
    virtual void Paint( const geometry::Rectangle2f& viewport );
    void SetPosition( const LocalWeather& weather );
    void StartEdition( LocalWeather& weather );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    WeatherLayer( const WeatherLayer& );            //!< Copy constructor
    WeatherLayer& operator=( const WeatherLayer& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual bool HandleMousePress( QMouseEvent* event, const geometry::Point2f& point );
    virtual bool HandleMouseMove( QMouseEvent* event, const geometry::Point2f& point );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::GlTools_ABC& tools_;
    gui::ExclusiveEventStrategy& eventStrategy_;
    geometry::Point2f topLeft_;
    geometry::Point2f bottomRight_;
    bool firstPointSet_;
    bool isEditing_;
    LocalWeather* currentWeather_;
    //@}
};

#endif // __WeatherLayer_h_
