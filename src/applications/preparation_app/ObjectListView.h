// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectListView_h_
#define __ObjectListView_h_

#include "clients_gui/ObjectListView.h"
#include "clients_kernel/SafePointer.h"

class ModelBuilder;

// =============================================================================
/** @class  ObjectListView
    @brief  Object list view
*/
// Created: SBO 2006-10-16
// =============================================================================
class ObjectListView : public gui::ObjectListView
{
public:
    //! @name Constructors/Destructor
    //@{
             ObjectListView( QWidget* pParent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, ModelBuilder& modelBuilder );
    virtual ~ObjectListView();
    //@}

private:
    //! @name Helpers
    //@{
    virtual Q3DragObject* dragObject();
    virtual void NotifyCreated( const kernel::Object_ABC& object );
    virtual void NotifySelectionChanged( const std::vector< const kernel::Entity_ABC* >& elements );
    virtual void keyPressEvent( QKeyEvent* key );
    virtual void OnContextMenuRequested( Q3ListViewItem*, const QPoint&, int );
    //@}

private:
    //! @name Member data
    //@{
    ModelBuilder& modelBuilder_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    //@}
};

#endif // __ObjectListView_h_
