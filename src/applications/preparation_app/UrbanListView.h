// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __UrbanListView_h_
#define __UrbanListView_h_

#include "clients_gui/EntityListView.h"
#include "clients_kernel/ModesObserver_ABC.h"
#include "tools/ElementObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class UrbanObject_ABC;
}

namespace gui
{
    class ItemFactory_ABC;
}

class ModelBuilder;

// =============================================================================
/** @class  UrbanListView
    @brief  UrbanListView
*/
// Created: JSR 2012-05-15
// =============================================================================
class UrbanListView : public gui::EntityListView
                    , public tools::ElementObserver_ABC< kernel::UrbanObject_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             UrbanListView( QWidget* pParent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, ModelBuilder& modelBuilder );
    virtual ~UrbanListView();
    //@}

protected:
    //! @name Helpers
    //@{
    virtual void NotifyCreated( const kernel::Team_ABC& team );
    virtual void NotifyDeleted( const kernel::Team_ABC& team );
    virtual void NotifyCreated( const kernel::UrbanObject_ABC& object );
    virtual void NotifyDeleted( const kernel::UrbanObject_ABC& object );
    virtual bool IsTypeRejected( const kernel::Entity_ABC& entity ) const;
    virtual void NotifyModeChanged( int newMode );
    virtual void OnContextMenuRequested( Q3ListViewItem* i, const QPoint& pos, int );
    virtual void keyPressEvent( QKeyEvent* key );
    virtual Q3DragObject* dragObject();
    virtual void viewportDropEvent( QDropEvent* pEvent );
    virtual void viewportDragMoveEvent( QDragMoveEvent *pEvent );
    virtual void viewportDragEnterEvent( QDragEnterEvent* pEvent );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnCreateCity();
    //@}

private:
    //! @name Helpers
    //@{
    kernel::Entity_ABC* Drop( QDropEvent* pEvent ) const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    ModelBuilder& modelBuilder_;
    std::vector< kernel::Entity_ABC* > dragged_;
    //@}
};

#endif // __UrbanListView_h_
