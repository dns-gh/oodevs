// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef _LogisticEditor_h
#define _LogisticEditor_h

#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/SafePointer.h"

class StaticModel;

namespace gui
{
    class CommonDelegate;
    class LogisticHierarchiesBase;
    class RichPushButton;
    template< typename T > class RichWidget;
}

namespace kernel
{
    class Agent_ABC;
    class Automat_ABC;
    class Controllers;
    class Entity_ABC;
    class Formation_ABC;
    class DotationType;
    class LogisticSupplyClass;
}

// =============================================================================
/** @class  LogisticEditor
@brief  Color editor
*/
// Created: MMC 2011-06-23
// =============================================================================
class LogisticEditor : public QDialog
                     , public tools::Observer_ABC
                     , public kernel::ContextMenuObserver_ABC< kernel::Automat_ABC >
                     , public kernel::ContextMenuObserver_ABC< kernel::Formation_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             LogisticEditor( QWidget* parent, const QString& objectName, kernel::Controllers& controllers, const ::StaticModel& staticModel );
    virtual ~LogisticEditor();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyContextMenu( const kernel::Automat_ABC& automat, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Formation_ABC& formation, kernel::ContextMenu& menu );
    //@}

protected:
    //! @name Types
    //@{
    typedef std::map< const kernel::DotationType*, double > T_Requirements;
    enum
    {
        eCategory,
        eDays
    };
    //@}

protected:
    //! @name Operations
    //@{
    virtual void Update( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu ) = 0;
    virtual void SupplyHierarchy( const kernel::Entity_ABC& entity, const gui::LogisticHierarchiesBase& logHierarchy ) = 0;
    //@}

    //! @name Helpers
    //@{
    void SupplyLogisticBaseStocks( const kernel::Entity_ABC& logBase, const kernel::LogisticSupplyClass& logType, T_Requirements& requirements );
    unsigned int GetQuantity( const QStandardItemModel& dataModel, int row, double requirement );
    //@}

private:
    //! @name Helpers
    //@{
    bool IsLogisticBase( const kernel::Entity_ABC& rootEntity );
    void SupplyHierarchy( kernel::SafePointer< kernel::Entity_ABC > entity );
    void ComputeRequirements( const kernel::Agent_ABC& agent, const kernel::LogisticSupplyClass& logType, T_Requirements& requirements );
    void FillSupplyRequirements( const kernel::Entity_ABC& entity,const kernel::LogisticSupplyClass& logType, T_Requirements& requirements );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnValueChanged( QStandardItem* item );
    void Validate();
    void Accept();
    void Reject();
    void closeEvent( QCloseEvent* pEvent );
    void Show();
    //@}

protected:
    //! @name Member data
    //@{
    const ::StaticModel& staticModel_;
    QStandardItemModel* dataModel_;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    gui::CommonDelegate* delegate_;
    gui::RichWidget< QTableView >* tableView_;
    gui::RichPushButton* validateButton_;
    gui::RichPushButton* cancelButton_;
    //@}
};

#endif // _LogisticEditor_h
