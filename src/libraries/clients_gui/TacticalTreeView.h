// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __gui_TacticalTreeView_h_
#define __gui_TacticalTreeView_h_

#include "HierarchyTreeView.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/SafePointer.h"
#include "clients_kernel/TacticalHierarchies.h"

namespace kernel
{
    class ContextMenu;
    class Controllers;
    class Entity_ABC;
    class Profile_ABC;
}

namespace gui
{
    class ChangeSuperiorDialog;
    class EntitySymbols;
    class ModelObserver_ABC;

// =============================================================================
/** @class  TacticalTreeView
    @brief  TacticalTreeView
*/
// Created: JSR 2012-08-31
// =============================================================================
class TacticalTreeView : public HierarchyTreeView< kernel::TacticalHierarchies >
                       , public kernel::ContextMenuObserver_ABC< kernel::Agent_ABC >
                       , public kernel::ContextMenuObserver_ABC< kernel::Automat_ABC >
                       , public kernel::ContextMenuObserver_ABC< kernel::Formation_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             TacticalTreeView( const QString& objectName,
                               kernel::Controllers& controllers,
                               const kernel::Profile_ABC& profile,
                               ModelObserver_ABC& modelObserver,
                               const EntitySymbols& symbols,
                               gui::ChangeSuperiorDialog& changeSuperiorDialog,
                               QWidget* parent = 0 );
    virtual ~TacticalTreeView();
    //@}

public:
    //! @name Operations
    //@{
    virtual std::vector< const QPixmap* > GetEntityPixmap( const kernel::Entity_ABC& entity );
    //@}

protected:
    //! @name ContextMenuObserver implementations
    //@{
    virtual void NotifyContextMenu( const kernel::Agent_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Automat_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Formation_ABC& entity, kernel::ContextMenu& menu );
    //@}

    //! @name Helpers
    //@{
    bool AddCommonMenu( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu );
    //@}

protected slots:
    //! @name Slots
    //@{
    void OnChangeSuperior();
    //@}

protected:
    //! @name Member data
    //@{
    kernel::SafePointer< kernel::Entity_ABC > contextMenuEntity_;
    gui::ChangeSuperiorDialog& changeSuperiorDialog_;
    //@}

private:
    //! @name Member data
    //@{
    QPixmap commandPost_;
    QPixmap lock_;
    //@}
};

}

#endif // __gui_TacticalTreeView_h_
