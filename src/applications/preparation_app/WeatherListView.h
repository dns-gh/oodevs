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

namespace kernel
{
    class CoordinateConverter_ABC;
}

class WeatherModel;

// =============================================================================
/** @class  WeatherListView
    @brief  Local weathers list
*/
// Created: SBO 2006-12-20
// =============================================================================
class WeatherListView : public gui::WeatherListView
{
public:
    //! @name Constructors/Destructor
    //@{
             WeatherListView( QWidget* parent, const kernel::CoordinateConverter_ABC& converter );
    virtual ~WeatherListView();
    //@}

    //! @name Operations
    //@{
    void CommitTo( WeatherModel& model );
    void Update( const WeatherModel& model );
    //@}

private slots:
    //! @name Slots
    //@{
    virtual void CreateItem();
    //@}

};

#endif // __WeatherListView_h_
