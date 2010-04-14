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

#include "clients_gui/ValuedComboBox.h"
#include "clients_kernel/Types.h"

namespace Common
{
    class MsgMeteoAttributes;
    class MsgMissionParameters;
}

namespace actions
{
    class Parameter_ABC;
}

namespace kernel
{
    class OrderParameter;
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
    void Commit( Common::MsgMissionParameters& att ) const;
    actions::Parameter_ABC& CreateParameter( const kernel::OrderParameter& parameter );
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
