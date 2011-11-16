// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __WeatherPanel_h_
#define __WeatherPanel_h_

#include "clients_gui/WeatherPanel.h"
#include "tools/ElementObserver_ABC.h"
#include "clients_kernel/Types.h"

namespace kernel
{
    class Controllers;
    class CoordinateConverter_ABC;
}

namespace gui
{
    class PanelStack_ABC;
    template< typename T > class ValuedComboBox;
    class WeatherLayer;
}
namespace weather
{
    class MeteoLocal;
}

class WeatherModel;
class WeatherWidget;

// =============================================================================
/** @class  WeatherPanel
    @brief  WeatherPanel
*/
// Created: SBO 2006-12-19
// =============================================================================
class WeatherPanel : public gui::WeatherPanel
                   , public tools::Observer_ABC
                   , public tools::ElementObserver_ABC< WeatherModel >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             WeatherPanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers, const kernel::CoordinateConverter_ABC& converter, gui::WeatherLayer& layer );
    virtual ~WeatherPanel();
    //@}

private slots:
    //! @name Slots
    //@{
    virtual void Commit();
    virtual void Reset();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const WeatherModel& model );
    virtual void NotifyDeleted( const WeatherModel& model );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    WeatherModel* currentModel_;

    QDateTimeEdit* time_;
    QTimeEdit* sunrise_;
    QTimeEdit* sunset_;
    gui::ValuedComboBox< E_LightingType >* dayLighting_;
    gui::ValuedComboBox< E_LightingType >* nightLighting_;
    QDateTimeEdit* endTime_;
    //@}
};

#endif // __WeatherPanel_h_
