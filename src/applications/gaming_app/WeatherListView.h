// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __WeatherListView_h_
#define __WeatherListView_h_

#include "clients_gui/WeatherListView.h"
#include <queue>

namespace kernel
{
    class CoordinateConverter_ABC;
    class Time_ABC;
}

class MeteoModel;

// =============================================================================
/** @class  WeatherListView
@brief  Local weathers list
*/
// Created: SBO 2006-12-20
// =============================================================================
class WeatherListView : public gui::WeatherListView
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    WeatherListView( QWidget* parent, const kernel::CoordinateConverter_ABC& converter, const kernel::Time_ABC& simulation );
    virtual ~WeatherListView();
    //@}

    //! @name Operations
    //@{
    void Update( const MeteoModel& model );
    unsigned int PopTrashedWeather();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void DeleteItem();
    //@}

private slots:
    //! @name Slots
    //@{
    virtual void CreateItem();
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Time_ABC&     simulation_;
    std::queue< unsigned int > trashedWeather_;
    //@}
};

#endif // __WeatherListView_h_
