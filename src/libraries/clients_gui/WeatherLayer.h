// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __gui_WeatherLayer_h_
#define __gui_WeatherLayer_h_

#include "Layer.h"

namespace kernel
{
    class Controllers;
}

namespace weather
{
    class Meteo;
    class MeteoLocal;
}

namespace gui
{
    class ExclusiveEventStrategy;
    class GlTools_ABC;

// =============================================================================
/** @class  WeatherLayer
    @brief  WeatherLayer
*/
// Created: ABR 2011-06-07
// =============================================================================
class WeatherLayer : public Layer
                   , public std::enable_shared_from_this< WeatherLayer >
{

public:
    //! @name Constructors/Destructor
    //@{
             WeatherLayer( kernel::Controllers& controllers,
                           GlTools_ABC& tools,
                           ExclusiveEventStrategy& eventStrategy );
    virtual ~WeatherLayer();
    //@}

    //! @name Operations
    //@{
    virtual void Paint( const geometry::Rectangle2f& viewport );
    void SetPosition( const weather::MeteoLocal& weather );
    void StartEdition( weather::MeteoLocal& weather );
    virtual const weather::Meteo* Pick( const geometry::Point2f& terrainCoordinates ) const;
    void Clear();
    //@}

private:
    //! @name Helpers
    //@{
    virtual bool HandleMousePress( QMouseEvent* event, const geometry::Point2f& point );
    virtual bool HandleMouseMove( QMouseEvent* event, const geometry::Point2f& point );
    //@}

protected:
    //! @name Member data
    //@{
    bool displaying_;
    //@}

private:
    //! @name Member data
    //@{
    ExclusiveEventStrategy&     eventStrategy_;
    geometry::Point2f           topLeft_;
    geometry::Point2f           bottomRight_;
    bool                        firstPointSet_;
    bool                        isEditing_;
    weather::MeteoLocal*        currentMeteoLocal_;
    //@}
};

}

#endif // __gui_WeatherLayer_h_
