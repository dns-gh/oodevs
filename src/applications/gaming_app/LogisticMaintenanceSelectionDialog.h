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
    class Availability;
    class Controller;
    class Controllers;
    class Entity_ABC;
    class MaintenanceStates_ABC;
}

namespace sword
{
    enum LogMaintenanceHandlingUpdate_EnumLogMaintenanceHandlingStatus;
}

class MaintenanceHaulersListView;
class MaintenanceRepairersListView;

// =============================================================================
/** @class  LogisticMaintenanceSelectionDialog
    @brief  LogisticMaintenanceSelectionDialog
*/
// Created: ABR 2014-01-23
// =============================================================================
class LogisticMaintenanceSelectionDialog : public LogisticSelectionDialog_ABC
                                         , public tools::Observer_ABC
                                         , public tools::ElementObserver_ABC< kernel::MaintenanceStates_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             LogisticMaintenanceSelectionDialog( const QString& objectName,
                                                 QWidget* parent,
                                                 kernel::Controllers& controllers,
                                                 actions::ActionsModel& actionsModel );
    virtual ~LogisticMaintenanceSelectionDialog();
    //@}

private:
    //! @name LogisticSelectionDialog_ABC implementation
    //@{
    virtual void Show( const LogisticsConsign_ABC& consign );
    //@}

    //! @name ElementObserver_ABC< MaintenanceStates_ABC > implementation
    //@{
    virtual void NotifyUpdated( const kernel::MaintenanceStates_ABC& a );
    //@}

    //! @name Helpers
    //@{
    void Purge();
    void UpdateDisplay();
    void AddWidget( sword::LogMaintenanceHandlingUpdate_EnumLogMaintenanceHandlingStatus status, QWidget* widget );
    void SetCurrentWidget();
    //@}

private slots:
    //! @name Slots
    //@{
    virtual void accept();
    virtual void OnRadioButtonChanged();
    virtual void OnSelectionChanged( const QModelIndex&, const QModelIndex& );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    actions::ActionsModel& actionsModel_;
    unsigned int id_;
    kernel::SafePointer< kernel::Entity_ABC > handler_;
    sword::LogMaintenanceHandlingUpdate_EnumLogMaintenanceHandlingStatus status_;
    std::map< sword::LogMaintenanceHandlingUpdate_EnumLogMaintenanceHandlingStatus, int > indexMap_;
    const kernel::Availability* availability_;

    QAbstractButton* automaticButton_;
    QAbstractButton* manualButton_;
    QAbstractButton* evacuateButton_;
    QPushButton* acceptButton_;
    QStackedWidget* stack_;
    MaintenanceHaulersListView* transporters_;
    MaintenanceRepairersListView* repairers_;
    //@}
};

#endif // LogisticMaintenanceSelectionDialog_h
