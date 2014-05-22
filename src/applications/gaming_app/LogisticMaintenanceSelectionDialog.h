// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __LogisticMaintenanceSelectionDialog_h
#define __LogisticMaintenanceSelectionDialog_h

#include "LogisticSelectionDialog_ABC.h"
#include "clients_kernel/SafePointer.h"
#include "tools/Observer_ABC.h"
#include "tools/ElementObserver_ABC.h"
#include <boost/optional.hpp>

namespace actions
{
    class ActionsModel;
}

namespace kernel
{
    class Agent_ABC;
    class Availability;
    class BreakdownType;
    class ComponentType;
    class Controller;
    class Controllers;
    class Entity_ABC;
    class MaintenanceStates_ABC;
    class ObjectTypes;
    class Profile_ABC;
}

namespace sword
{
    enum LogMaintenanceHandlingUpdate_EnumLogMaintenanceHandlingStatus;
}

namespace gui
{
    class DisplayExtractor;
}

class MaintenanceHaulersListView;
class MaintenanceRepairersListView;
class PartsView;
class DiagnosisUnitView;

// =============================================================================
/** @class  LogisticMaintenanceSelectionDialog
    @brief  LogisticMaintenanceSelectionDialog
*/
// Created: ABR 2014-01-23
// =============================================================================
class LogisticMaintenanceSelectionDialog : public LogisticSelectionDialog_ABC
                                         , public tools::Observer_ABC
                                         , public tools::ElementObserver_ABC< kernel::MaintenanceStates_ABC >
                                         , public tools::ElementObserver_ABC< kernel::Profile_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             LogisticMaintenanceSelectionDialog( const QString& objectName,
                                                 QWidget* parent,
                                                 kernel::Controllers& controllers,
                                                 actions::ActionsModel& actionsModel,
                                                 const kernel::ObjectTypes& breakdownTypes,
                                                 gui::DisplayExtractor& extractor );
    virtual ~LogisticMaintenanceSelectionDialog();
    //@}

private:
    //! @name LogisticSelectionDialog_ABC implementation
    //@{
    virtual void Show( const LogisticsConsign_ABC& consign );
    //@}

    virtual void NotifyUpdated( const kernel::MaintenanceStates_ABC& a );
    virtual void NotifyUpdated( const kernel::Profile_ABC& profile );
    //! @name Helpers
    //@{
    void Purge();
    void AddWidget( sword::LogMaintenanceHandlingUpdate_EnumLogMaintenanceHandlingStatus status, QWidget* widget );
    bool SetCurrentStatus( sword::LogMaintenanceHandlingUpdate_EnumLogMaintenanceHandlingStatus status );
    //@}

private slots:
    //! @name Slots
    //@{
    virtual void OnOkClicked();
    virtual void OnRadioButtonChanged();
    virtual void OnSelectionChanged( const QModelIndex&, const QModelIndex& );
    void UpdateDisplay();
    void OnTimeout();
    void OnDestinationSelected( unsigned int destination );
    void OnDestinationToggled( bool enabled );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    actions::ActionsModel& actionsModel_;
    const kernel::ObjectTypes& breakdownTypes_;
    unsigned int id_;
    int lastContext_;
    kernel::SafePointer< kernel::Entity_ABC > handler_;
    kernel::SafePointer< kernel::Agent_ABC > consumer_;
    const kernel::ComponentType* componentType_;
    const kernel::BreakdownType* breakdownType_;
    sword::LogMaintenanceHandlingUpdate_EnumLogMaintenanceHandlingStatus status_;
    std::map< sword::LogMaintenanceHandlingUpdate_EnumLogMaintenanceHandlingStatus, int > indexMap_;
    const kernel::Availability* availability_;

    QAbstractButton* automaticButton_;
    QAbstractButton* manualButton_;
    QAbstractButton* evacuateButton_;
    QPushButton* acceptButton_;
    QStackedWidget* stack_;
    MaintenanceHaulersListView* transporters_;
    QLabel* equipmentWeight_;

    QGroupBox* destinationBox_;
    boost::optional< unsigned int > selectedDestination_;
    MaintenanceRepairersListView* repairers_;
    MaintenanceRepairersListView* diagnosers_;
    QLabel* duration_;
    PartsView* parts_;
    DiagnosisUnitView* destinations_;
    QTimer timeout_;
    //@}
};

#endif // LogisticMaintenanceSelectionDialog_h
