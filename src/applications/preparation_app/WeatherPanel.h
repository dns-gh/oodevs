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

#include "clients_gui/WeatherPanel.h"
#include "clients_kernel/Types.h"
#include <tools/ElementObserver_ABC.h>

namespace kernel
{
    class Controllers;
    class CoordinateConverter_ABC;
    class ModelLoaded;
}

namespace gui
{
    class PanelStack_ABC;
    template< typename T > class ValuedComboBox;
    class WeatherLayer;
    template< typename T > class RichWidget;
    template< typename T > class RichWidget;
}
namespace weather
{
    class MeteoLocal;
}

class WeatherModel;

// =============================================================================
/** @class  WeatherPanel
    @brief  WeatherPanel
*/
// Created: SBO 2006-12-19
// =============================================================================
class WeatherPanel : public gui::WeatherPanel
                   , public tools::Observer_ABC
                   , public tools::ElementObserver_ABC< WeatherModel >
                   , public tools::ElementObserver_ABC< kernel::ModelLoaded >
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
    void OnSunRiseChanged( const QTime &date );
    void OnSunSetChanged( const QTime &date );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const WeatherModel& model );
    virtual void NotifyDeleted( const WeatherModel& model );
    virtual void NotifyUpdated( const kernel::ModelLoaded& );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    WeatherModel* currentModel_;

    gui::RichWidget< QDateTimeEdit >* time_;
    gui::RichWidget< QTimeEdit >* sunrise_;
    gui::RichWidget< QTimeEdit >* sunset_;
    gui::ValuedComboBox< E_LightingType >* dayLighting_;
    gui::ValuedComboBox< E_LightingType >* nightLighting_;
    QDateTimeEdit* endTime_;
    //@}
};

#endif // __WeatherPanel_h_
