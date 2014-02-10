// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __WeatherWidget_h_
#define __WeatherWidget_h_

#include "clients_gui/WeatherWidget.h"

namespace gui
{
    struct WeatherParameters;
}

// =============================================================================
/** @class  WeatherWidget
    @brief  WeatherWidget
*/
// Created: SBO 2006-12-20
// =============================================================================
class WeatherWidget : public gui::WeatherWidget
{
public:
    //! @name Constructors/Destructor
    //@{
             WeatherWidget( const QString& objectName, QWidget* parent, const QString& title );
    virtual ~WeatherWidget();
    //@}

    //! @name Operations
    //@{
    gui::WeatherParameters CreateParameters();
    //@}
};

#endif // __WeatherWidget_h_
