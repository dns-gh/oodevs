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
public:
    //! @name Constructors/Destructor
    //@{
    WeatherListView( QWidget* parent, const kernel::CoordinateConverter_ABC& converter, const kernel::Time_ABC& simulation );
    virtual ~WeatherListView();
    //@}

    //! @name Operations
    //@{
    void Update( const MeteoModel& model );
    //@}

private slots:
    //! @name Slots
    //@{
    virtual void ContextMenuRequested( QListViewItem* item, const QPoint& point, int column );
    virtual void CreateItem();
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Time_ABC& simulation_;
    //@}
};

#endif // __WeatherListView_h_
