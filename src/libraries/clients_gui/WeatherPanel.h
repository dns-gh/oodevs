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
    class RichPushButton;
    class RichGroupBox;
    template< typename T > class RichWidget;

// =============================================================================
/** @class  WeatherPanel
    @brief  WeatherPanel
*/
// Created: ABR 2011-05-30
// =============================================================================
class WeatherPanel : public InfoPanel_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             WeatherPanel( QWidget* parent,
                           PanelStack_ABC& panel,
                           const std::shared_ptr< WeatherLayer >& layer );
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
    std::shared_ptr< WeatherLayer > layer_;
    E_WeatherType           currentType_;

    QVBoxLayout*            headerLayout_;
    QWidget*                localWidget_;
    QWidget*                globalWidget_;
    RichGroupBox*           parametersGroup_;
    RichPushButton*         locationButton_;
    WeatherListView*        localWeathers_;
    WeatherWidget*          localWeatherWidget_;
    WeatherWidget*          globalWeatherWidget_;

    weather::MeteoLocal*    selectedLocal_;
    RichWidget< QDateTimeEdit >*       startTime_;
    RichWidget< QDateTimeEdit >*       endTime_;
    //@}

};

}
#endif // __gui_WeatherPanel_h_
