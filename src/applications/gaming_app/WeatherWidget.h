// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __WeatherWidget_h_
#define __WeatherWidget_h_

#include "clients_gui/WeatherWidget.h"

namespace actions
{
    class MagicAction;
}

namespace kernel
{
    class OrderParameter;
}

namespace tools
{
    template< typename Item > class Iterator;
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
             WeatherWidget( QWidget* parent, const QString& title );
    virtual ~WeatherWidget();
    //@}

    //! @name Operations
    //@{
    void CreateParameters( actions::MagicAction& action, tools::Iterator< const kernel::OrderParameter& >& it );
    //@}
};

#endif // __WeatherWidget_h_
