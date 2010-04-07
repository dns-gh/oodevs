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

#include "clients_gui/ValuedComboBox.h"
#include "clients_kernel/Types.h"

namespace Common
{
    class MsgMeteoAttributes;
}

// =============================================================================
/** @class  WeatherWidget
    @brief  WeatherWidget
*/
// Created: SBO 2006-12-20
// =============================================================================
class WeatherWidget : public QGroupBox
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             WeatherWidget( QWidget* parent, const QString& title );
    virtual ~WeatherWidget();
    //@}

    //! @name Operations
    //@{
    void Commit( Common::MsgMeteoAttributes& att ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    WeatherWidget( const WeatherWidget& );            //!< Copy constructor
    WeatherWidget& operator=( const WeatherWidget& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    QSpinBox* windDirection_;
    QSpinBox* windSpeed_;
    QSpinBox* cloudFloor_;
    QSpinBox* cloudCeiling_;
    QSpinBox* cloudDensity_;
    gui::ValuedComboBox< kernel::E_WeatherType >* type_;
    QLabel* startTimeLabel_;
    QLabel* endTimeLabel_;
    //@}
};

#endif // __WeatherWidget_h_
