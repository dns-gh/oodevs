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

namespace sword
{
    class WeatherAttributes;
    class MissionParameters;
}

namespace actions
{
    class MagicAction;
}

namespace kernel
{
    class OrderParameter;
}

namespace weather
{
    class PHY_Meteo;
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
    void Update( const weather::PHY_Meteo& globalMeteo );
    void Commit( sword::WeatherAttributes& att ) const;
    void Commit( sword::MissionParameters& att ) const;
    void CreateParameters( actions::MagicAction& action, tools::Iterator< const kernel::OrderParameter& >& it );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    WeatherWidget( const WeatherWidget& );            //!< Copy constructor
    WeatherWidget& operator=( const WeatherWidget& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    QSpinBox* windDirection_;
    QSpinBox* windSpeed_;
    QSpinBox* cloudFloor_;
    QSpinBox* cloudCeiling_;
    QSpinBox* cloudDensity_;
    gui::ValuedComboBox< E_WeatherType >* type_;
    QLabel* startTimeLabel_;
    QLabel* endTimeLabel_;
    //@}
};

#endif // __WeatherWidget_h_
