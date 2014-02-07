// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __InfoButtonsWidget_h_
#define __InfoButtonsWidget_h_

#include <tools/SelectionObserver_ABC.h>
#include <tools/ElementObserver_ABC.h>

namespace kernel
{
    class Controllers;
    class Profile_ABC;
    class Entity_ABC;
}

namespace sword
{
    class ListLogisticRequestsAck;
}

namespace gui
{
    class ItemFactory_ABC;
    class DisplayExtractor;
}

namespace tools
{
    class ExerciseConfig;
}

class Model;
class LogisticDialog_ABC;
class Simulation;
class LogMedicalConsigns;
class LogFuneralConsigns;
class LogMaintenanceConsigns;
class LogSupplyConsigns;
class LogMedicalConsign;
class LogFuneralConsign;
class LogMaintenanceConsign;
class LogSupplyConsign;
class LogisticsConsign_ABC;
class LogisticsModel;
class SimulationController;

// =============================================================================
/** @class  InfoButtonsWidget
    @brief  InfoButtonsWidget
*/
// Created: SBO 2007-02-05
// =============================================================================
class InfoButtonsWidget : public Q3GroupBox
                        , public tools::Observer_ABC
                        , public tools::SelectionObserver< kernel::Entity_ABC >
                        , public tools::ElementObserver_ABC< LogMedicalConsigns >
                        , public tools::ElementObserver_ABC< LogSupplyConsigns >
                        , public tools::ElementObserver_ABC< LogFuneralConsigns >
                        , public tools::ElementObserver_ABC< LogMaintenanceConsigns >
                        , public tools::ElementObserver_ABC< LogMedicalConsign >
                        , public tools::ElementObserver_ABC< LogMaintenanceConsign >
                        , public tools::ElementObserver_ABC< LogFuneralConsign >
                        , public tools::ElementObserver_ABC< LogSupplyConsign >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             InfoButtonsWidget( QWidget* widget,
                                kernel::Controllers& controllers,
                                const tools::ExerciseConfig& config,
                                gui::ItemFactory_ABC& factory,
                                gui::DisplayExtractor& extractor,
                                Model& model,
                                const Simulation& simulation,
                                const kernel::Profile_ABC& profile,
                                SimulationController& simulationController );
    virtual ~InfoButtonsWidget();
    //@}

private:
    //! @name Operations
    //@{
    virtual void NotifySelected( const kernel::Entity_ABC* element );

    virtual void NotifyUpdated( const LogMedicalConsigns& consigns );
    virtual void NotifyUpdated( const LogMaintenanceConsigns& consigns );
    virtual void NotifyUpdated( const LogSupplyConsigns& consigns );
    virtual void NotifyUpdated( const LogFuneralConsigns& consigns );

    virtual void NotifyUpdated( const LogMedicalConsign& consign );
    virtual void NotifyUpdated( const LogMaintenanceConsign& consign );
    virtual void NotifyUpdated( const LogSupplyConsign& consign );
    virtual void NotifyUpdated( const LogFuneralConsign& consign );
    //@}

    //! @name Helpers
    //@{
    template< typename Dialog >
    void AddButton( const QPixmap& pixmap, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    template< typename Dialog >
    void AddLogisticButton( const QPixmap& pixmap, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory,
                    gui::DisplayExtractor& extractor, const kernel::Profile_ABC& profile, Model& model );
    template< typename Dialog >
    void AddLogisticButton( const QPixmap& pixmap, kernel::Controllers& controllers
                  , gui::DisplayExtractor& extractor, const kernel::Profile_ABC& profile, Model& model );
    void AddButton( QDialog* dialog, const QPixmap& pixmap, const QString& tooltips, const char* toggleSlot, const char* toggleSignal );
    void FillRequests( const sword::ListLogisticRequestsAck& message );
    template< typename Extension >
    void Update( const Extension& consigns );
    void UpdateConsign( const LogisticsConsign_ABC& consign );
    //@}

private slots:
    //! @name Helpers
    //@{
    void OnUpdate();
    void Refresh();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const Simulation& simulation_;
    SimulationController& simulationController_;
    LogisticsModel& historyModel_;
    std::vector< LogisticDialog_ABC* > logisticDialogs_;
    const kernel::Entity_ABC* element_;
    std::set< unsigned long > entities_;
    bool hasChanged_;
    QTimer* timer_;
    unsigned int lastTick_;
    //@}
};

#endif // __InfoButtonsWidget_h_
