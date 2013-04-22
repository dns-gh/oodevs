// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __GroupDialog_h_
#define __GroupDialog_h_

#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/SafePointer.h"
#include "clients_gui/CommonDelegate.h"
#include <boost/optional.hpp>

namespace kernel
{
    class Agent_ABC;
    class Entity_ABC;
    class Controllers;
}

class GroupsDelegate;

// =============================================================================
/** @class  GroupDialog
    @brief  GroupDialog
*/
// Created: NPT 2013-04-11
// =============================================================================
class GroupDialog : public QDialog
                  , public tools::Observer_ABC
                  , public kernel::ContextMenuObserver_ABC< kernel::Agent_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             GroupDialog( QWidget* parent, kernel::Controllers& controllers );
    virtual ~GroupDialog();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnValidate();
    void OnShow();
    void OnContextMenuRequested( const QPoint& );
    //@}

private:
    //! @name Operations
    //@{
    virtual void NotifyContextMenu( const kernel::Agent_ABC& entity, kernel::ContextMenu& menu );
    //@}

    //! @name Helpers
    //@{
    int AddGroup( const QString& name );
    int AddEquipment( const QString& name );
    void AddItem( int row, int column, const QString& text = "", Qt::ItemFlags flags = 0 );

    void Increase( int row, int column );
    bool Decrease( int row, int column );

    QString InputName( const QString& label, const QString& previous );

    boost::optional< QString > FindGroup( const QString& name );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    kernel::SafePointer< kernel::Agent_ABC > selected_;
    QTableView table_;
    QStandardItemModel dataModel_;
    QSortFilterProxyModel proxyModel_;
    std::auto_ptr< GroupsDelegate > delegate_;
    //@}
};

#endif // __GroupDialog_h_
