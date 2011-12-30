// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __InhabitantPanel_h_
#define __InhabitantPanel_h_

#include "tools/SelectionObserver_ABC.h"
#include "tools/ElementObserver_ABC.h"
#include "clients_kernel/LivingAreaVisitor_ABC.h"

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class LivingArea_ABC;
}

class Model;

// =============================================================================
/** @class  InhabitantPanel
    @brief  Inhabitant panel
*/
// Created: LGY 2011-12-29
// =============================================================================
class InhabitantPanel : public QDockWidget
                      , public tools::Observer_ABC
                      , public tools::SelectionObserver< kernel::Entity_ABC >
                      , public tools::ElementObserver_ABC< kernel::LivingArea_ABC >
                      , private kernel::LivingAreaVisitor_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             InhabitantPanel( QMainWindow* mainWindow, kernel::Controllers& controllers, Model& model );
    virtual ~InhabitantPanel();
    //@}

private:
    //! @name Operations
    //@{
    virtual void NotifySelected( const kernel::Entity_ABC* entity );
    virtual void Visit( unsigned int id, const std::string& name, unsigned int resident,
                        bool alerted, float angriness, bool confined, bool evacuated );
     virtual void NotifyUpdated( const kernel::LivingArea_ABC& area );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnSelectionChanged( const QModelIndex& );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    Model& model_;
    const kernel::Entity_ABC* current_;
    QStringList horizontalHeaders_;
    QStandardItemModel* dataModel_;
    QSortFilterProxyModel* proxyModel_;
    QTableView* table_;
    //@}
};

#endif // __InhabitantPanel_h_
