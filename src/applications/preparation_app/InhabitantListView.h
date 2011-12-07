// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __InhabitantListView_h_
#define __InhabitantListView_h_

#include "clients_gui/InhabitantListView.h"
#include "clients_kernel/SafePointer.h"

class ModelBuilder;

// =============================================================================
/** @class  InhabitantListView
    @brief  Inhabitant list view
*/
// Created: SLG 2010-11-23
// =============================================================================
class InhabitantListView : public gui::InhabitantListView
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             InhabitantListView( QWidget* pParent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, ModelBuilder& modelBuilder );
    virtual ~InhabitantListView();
    //@}

private:
    //! @name Helpers
    //@{
    virtual Q3DragObject* dragObject();
    virtual void NotifyCreated( const kernel::Team_ABC& team );
    virtual void NotifySelected( const kernel::Entity_ABC* element );
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

#endif // __InhabitantListView_h_
