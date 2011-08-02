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

#include <boost/noncopyable.hpp>
#include "clients_gui/ValuedComboBox.h"
#include "clients_kernel/Types.h"

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
class WeatherWidget : public Q3GroupBox
                    , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             WeatherWidget( QWidget* parent, const QString& title );
    virtual ~WeatherWidget();
    //@}

    //! @name Operations
    //@{
    void Update( const weather::Meteo& meteo );
    void CommitTo( weather::Meteo& meteo ) const;
    void Clear();
    //@}

protected:
    //! @name Member data
    //@{
    QSpinBox* windDirection_;
    QSpinBox* windSpeed_;
    QSpinBox* cloudFloor_;
    QSpinBox* cloudCeiling_;
    QSpinBox* cloudDensity_;
    gui::ValuedComboBox< E_WeatherType >* type_;
    //@}
};

}

#endif // __gui_WeatherWidget_h_
