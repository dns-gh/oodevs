// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "InhabitantTreeView.h"
#include "clients_gui/ModelObserver_ABC.h"
#include "clients_kernel/Inhabitant_ABC.h"

// -----------------------------------------------------------------------------
// Name: InhabitantTreeView constructor
// Created: JSR 2012-09-24
// -----------------------------------------------------------------------------
InhabitantTreeView::InhabitantTreeView( const QString& objectName, kernel::Controllers& controllers, const kernel::Profile_ABC& profile, gui::ModelObserver_ABC& modelObserver, QWidget* parent /*= 0*/ )
    : gui::InhabitantTreeView( objectName, controllers, profile, modelObserver, true, parent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InhabitantTreeView destructor
// Created: JSR 2012-09-24
// -----------------------------------------------------------------------------
InhabitantTreeView::~InhabitantTreeView()
{
    // NOTHING
}

void InhabitantTreeView::keyPressEvent( QKeyEvent* event )
{
    const QModelIndex index = selectionModel()->currentIndex();
    if( event && event->key() == Qt::Key_Delete && index.isValid() )
    {
        if( kernel::Entity_ABC* entity = dataModel_.GetDataFromIndex< kernel::Entity_ABC >( index ) )
            modelObserver_.DeleteEntity( *entity );
    }
    else
        gui::InhabitantTreeView::keyPressEvent( event );
}
