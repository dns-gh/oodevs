// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __gui_WeatherPanel_h_
#define __gui_WeatherPanel_h_

#include <boost/noncopyable.hpp>
#include "InfoPanel_ABC.h"

namespace weather
{
    class MeteoLocal;
}

namespace gui
{
    class PanelStack_ABC;
    class WeatherLayer;
    class WeatherListView;
    class WeatherWidget;

// =============================================================================
/** @class  WeatherPanel
    @brief  WeatherPanel
*/
// Created: ABR 2011-05-30
// =============================================================================
class WeatherPanel : public InfoPanel_ABC
                   , private boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             WeatherPanel( QWidget* parent, PanelStack_ABC& panel, WeatherLayer& layer );
    virtual ~WeatherPanel();
    //@}

    //! @name Operations
    //@{
    virtual void Purge();
    //@}

protected:
    //! @name Helpers
    //@{
    void CreateLocalParameters();
    void CommitLocalWeather();
    //@}

protected:
    //! @name Types
    //@{
    enum E_WeatherType
    {
        eWeatherGlobal = 0,
        eWeatherLocal = 1
    };
    //@}

private:
    //! @name Helpers
    //@{
    void EnableLocalParameters( bool enabled, bool isCreated );
    //@}

private slots:
    //! @name Slots
    //@{
    virtual void hide();
    virtual void Commit() = 0;
    virtual void Reset () = 0;
    void SetPatchPosition();
    void OnTypeChanged( int selected );
    void LocalSelectionChanged();
    //@}

protected:
    //! @name Member data
    //@{
    WeatherLayer&           layer_;
    E_WeatherType           currentType_;

    Q3VBox*                 headerLayout_;
    Q3VBox*                 localLayout_;
    Q3VBox*                 globalLayout_;
    Q3GroupBox*             parametersGroup_;
    QPushButton*            locationButton_;
    WeatherListView*        localWeathers_;
    WeatherWidget*          localWidget_;
    WeatherWidget*          globalWidget_;

    weather::MeteoLocal*    selectedLocal_;
    QDateTimeEdit*          startTime_;
    QDateTimeEdit*          endTime_;
    Q3Button*               positionBtn_;
    //@}

};

}
#endif // __gui_WeatherPanel_h_
