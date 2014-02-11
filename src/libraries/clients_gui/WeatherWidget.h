// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __gui_WeatherWidget_h_
#define __gui_WeatherWidget_h_

#include "RichGroupBox.h"
#include "ValuedComboBox.h"
#include "ENT/ENT_Enums.h"

namespace gui
{
    struct WeatherParameters;
}

namespace weather
{
    class Meteo;
}

namespace gui
{
    template< typename T > class ValuedComboBox;

// =============================================================================
/** @class  WeatherWidget
    @brief  WeatherWidget
*/
// Created: ABR 2011-05-30
// =============================================================================
class WeatherWidget : public gui::RichGroupBox
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
    void Update( const weather::Meteo& meteo );
    void CommitTo( weather::Meteo& meteo ) const;
    void Clear();
    //@}

protected:
    //! @name Member data
    //@{
    QDial* windDirection_;
    QSpinBox* windSpeed_;
    QSpinBox* temperature_;
    QSpinBox* cloudFloor_;
    QSpinBox* cloudCeiling_;
    QSpinBox* cloudDensity_;
    gui::ValuedComboBox< E_WeatherType >* type_;
    //@}
};

}

#endif // __gui_WeatherWidget_h_
