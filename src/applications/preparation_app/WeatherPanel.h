// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __WeatherPanel_h_
#define __WeatherPanel_h_

#include "clients_gui/InfoPanel_ABC.h"
#include "clients_gui/ValuedComboBox.h"
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
}

class WeatherModel;
class WeatherWidget;
class LocalWeathersList;
class LocalWeather;
class WeatherLayer;

// =============================================================================
/** @class  WeatherPanel
    @brief  WeatherPanel
*/
// Created: SBO 2006-12-19
// =============================================================================
class WeatherPanel : public gui::InfoPanel_ABC
                   , public tools::Observer_ABC
                   , public tools::ElementObserver_ABC< WeatherModel >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             WeatherPanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers, const kernel::CoordinateConverter_ABC& converter, WeatherLayer& layer );
    virtual ~WeatherPanel();
    //@}

private slots:
    //! @name Slots
    //@{
    void Commit();
    void Reset ();
    void LocalSelectionChanged();
    void SetPatchPosition();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    WeatherPanel( const WeatherPanel& );            //!< Copy constructor
    WeatherPanel& operator=( const WeatherPanel& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const WeatherModel& model );
    virtual void NotifyDeleted( const WeatherModel& model );
    void CommitLocalWeather();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    WeatherModel* currentModel_;
    WeatherLayer& layer_;
    QDateTimeEdit* time_;
    QTimeEdit* sunrise_;
    QTimeEdit* sunset_;
    gui::ValuedComboBox< kernel::E_DayLightingType >* dayLighting_;
    gui::ValuedComboBox< kernel::E_NightLightingType >* nightLighting_;
    WeatherWidget* globalWeather_;
    QVBox* localWeatherBox_;
    LocalWeathersList* localWeathers_;
    LocalWeather* selectedLocal_;
    WeatherWidget* localWeather_;
    QLabel* startTimeLabel_;
    QDateTimeEdit* startTime_;
    QLabel* endTimeLabel_;
    QDateTimeEdit* endTime_;
    //@}
};

#endif // __WeatherPanel_h_
