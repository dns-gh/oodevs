// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationListView_h_
#define __PopulationListView_h_

#include "clients_gui/PopulationListView.h"
#include "clients_kernel/SafePointer.h"

class ModelBuilder;

// =============================================================================
/** @class  PopulationListView
    @brief  Population list view
*/
// Created: SBO 2006-10-16
// =============================================================================
class PopulationListView : public gui::PopulationListView
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             PopulationListView( QWidget* pParent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, ModelBuilder& modelBuilder );
    virtual ~PopulationListView();
    //@}

private:
    //! @name Helpers
    //@{
    virtual Q3DragObject* dragObject();
    virtual void viewportDropEvent( QDropEvent* pEvent );
    virtual void viewportDragMoveEvent( QDragMoveEvent *pEvent );
    virtual void viewportDragEnterEvent( QDragEnterEvent* pEvent );
    virtual void NotifyCreated( const kernel::Population_ABC& population );
    virtual void NotifyCreated( const kernel::Team_ABC& team );
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

#endif // __PopulationListView_h_
