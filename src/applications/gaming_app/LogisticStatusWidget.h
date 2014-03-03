// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __LogisticStatusWidget_h_
#define __LogisticStatusWidget_h_

#include "clients_gui/RichWidget.h"
#include "clients_kernel/SafePointer.h"
#include <tools/ElementObserver_ABC.h>
#include <tools/SelectionObserver_ABC.h>

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class MaintenanceStates_ABC;
}

namespace gui
{
    class CommonDelegate;
    class LogisticBase;
}

class SupplyStates;
class MedicalStates;

// =============================================================================
/** @class  LogisticStatusWidget
    @brief  LogisticStatusWidget
*/
// Created: SBO 2007-02-19
// =============================================================================
template< typename Extension >
class LogisticStatusWidget : public gui::RichWidget< QTableView >
                           , public tools::Observer_ABC
                           , public tools::ElementObserver_ABC< Extension >
                           , public tools::ElementObserver_ABC< gui::LogisticBase >
                           , public tools::SelectionObserver< kernel::Entity_ABC >
{
public:
    //! @name Types
    //@{
    typedef std::function< bool( const gui::LogisticBase& ) > T_LogisticBaseModeChecker;
    //@}

public:
    //! @name Constructor/Destructor
    //@{
             LogisticStatusWidget( const QString& objectName,
                                   QWidget* parent,
                                   kernel::Controllers& controllers,
                                   const T_LogisticBaseModeChecker& modeChecker = T_LogisticBaseModeChecker() );
    virtual ~LogisticStatusWidget();
    //@}

    //! @name Operations
    //@{
    void Add( const QString& title, const QString& value );
    //@}

private:
    //! @name Observers implementation
    //@{
    virtual void NotifySelected( const kernel::Entity_ABC* element );
    virtual void NotifyUpdated( const Extension& extension );
    virtual void NotifyUpdated( const gui::LogisticBase& base );
    //@}

    //! @name Helpers
    //@{
    void showEvent( QShowEvent* );
    void Update();
    //@}

private:
    //! @name Member data
    //@{
    QStandardItemModel* dataModel_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    kernel::Controllers& controllers_;
    QSortFilterProxyModel* proxyModel_;
    gui::CommonDelegate* delegate_;
    const T_LogisticBaseModeChecker modeChecker_;
    //@}
};

// =============================================================================
/** @class  MaintenanceStatusWidget
    @brief  MaintenanceStatusWidget
*/
// Created: SBO 2007-02-19
// =============================================================================
class MaintenanceStatusWidget : public LogisticStatusWidget< kernel::MaintenanceStates_ABC >
{
public:
             MaintenanceStatusWidget( QWidget* parent, kernel::Controllers& controllers );
    virtual ~MaintenanceStatusWidget() {}
};

// =============================================================================
/** @class  MedicalStatusWidget
    @brief  MedicalStatusWidget
*/
// Created: SBO 2007-02-19
// =============================================================================
class MedicalStatusWidget : public LogisticStatusWidget< MedicalStates >
{
public:
             MedicalStatusWidget( QWidget* parent, kernel::Controllers& controllers );
    virtual ~MedicalStatusWidget() {}
};

// =============================================================================
/** @class  SupplyStatusWidget
    @brief  SupplyStatusWidget
*/
// Created: SBO 2007-02-19
// =============================================================================
class SupplyStatusWidget : public LogisticStatusWidget< SupplyStates >
{
public:
             SupplyStatusWidget( QWidget* parent, kernel::Controllers& controllers );
    virtual ~SupplyStatusWidget() {}
};

#endif // __LogisticStatusWidget_h_
