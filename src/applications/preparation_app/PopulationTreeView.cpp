// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "PopulationTreeView.h"
#include "clients_gui/DragAndDropHelpers.h"
#include "clients_gui/ModelObserver_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "preparation/PopulationHierarchies.h"

// -----------------------------------------------------------------------------
// Name: PopulationTreeView constructor
// Created: JSR 2012-09-21
// -----------------------------------------------------------------------------
PopulationTreeView::PopulationTreeView( const QString& objectName, kernel::Controllers& controllers, const kernel::Profile_ABC& profile, gui::ModelObserver_ABC& modelObserver, QWidget* parent /*= 0*/ )
    : gui::PopulationTreeView( objectName, controllers, profile, modelObserver, true, parent )
{
    EnableDragAndDrop( true );
}

// -----------------------------------------------------------------------------
// Name: PopulationTreeView destructor
// Created: JSR 2012-09-21
// -----------------------------------------------------------------------------
PopulationTreeView::~PopulationTreeView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationTreeView::dragMoveEvent
// Created: JSR 2012-09-21
// -----------------------------------------------------------------------------
void PopulationTreeView::dragMoveEvent( QDragMoveEvent* pEvent )
{
    if( IsReadOnly() )
    {
        pEvent->ignore();
        return;
    }
    kernel::Entity_ABC* target = dataModel_.GetDataFromIndex< kernel::Entity_ABC >( indexAt( pEvent->pos() ) );
    if( !target || target->GetTypeName() != kernel::Team_ABC::typeName_ || !profile_.CanDoMagic( *target ) )
    {
        pEvent->ignore();
        return;
    }
    if( const kernel::Population_ABC* pop = dnd::FindSafeData< kernel::Population_ABC >( pEvent ) )
    {
        const kernel::TacticalHierarchies* hierachies = pop->Retrieve< kernel::TacticalHierarchies >();
        if( hierachies && profile_.CanDoMagic( *pop ) && &hierachies->GetTop() != target )
        {
            pEvent->accept();
            return;
        }
    }
    pEvent->ignore();
}

// -----------------------------------------------------------------------------
// Name: PopulationTreeView::ItemSpecificFlags
// Created: JSR 2012-09-21
// -----------------------------------------------------------------------------
Qt::ItemFlags PopulationTreeView::ItemSpecificFlags( const kernel::Entity_ABC& entity ) const
{
    return gui::PopulationTreeView::ItemSpecificFlags( entity ) | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
}

// -----------------------------------------------------------------------------
// Name: PopulationTreeView::MimeTypes
// Created: JSR 2012-09-21
// -----------------------------------------------------------------------------
QStringList PopulationTreeView::MimeTypes() const
{
    QStringList l;
    l << typeid( kernel::Population_ABC ).name() << typeid( kernel::Team_ABC ).name();
    return l;
}

// -----------------------------------------------------------------------------
// Name: PopulationTreeView::Drop
// Created: JSR 2012-09-21
// -----------------------------------------------------------------------------
void PopulationTreeView::Drop( const QString& mimeType, void* data, QStandardItem& target )
{
    if( IsReadOnly() )
        return;
    kernel::Entity_ABC* entityTarget = dataModel_.GetDataFromItem< kernel::Entity_ABC >( target );
    if( !entityTarget || entityTarget->GetTypeName() != kernel::Team_ABC::typeName_ )
        return;

    if( mimeType == typeid( kernel::Population_ABC ).name() )
    {
        kernel::SafePointer< kernel::Population_ABC >* safePtr = reinterpret_cast< kernel::SafePointer< kernel::Population_ABC >* >( data );
        if( !safePtr || !*safePtr )
            return;
        kernel::TacticalHierarchies* hierarchies = safePtr->ConstCast()->Retrieve< kernel::TacticalHierarchies >();
        kernel::TacticalHierarchies* targetHierarchies = entityTarget->Retrieve< kernel::TacticalHierarchies >();
        const kernel::Entity_ABC& superior = targetHierarchies ? targetHierarchies->GetTop() : *entityTarget;
        static_cast< PopulationHierarchies* >( hierarchies )->ChangeSuperior( const_cast< kernel::Entity_ABC& >( superior ) );
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationTreeView::keyPressEvent
// Created: JSR 2012-09-24
// -----------------------------------------------------------------------------
void PopulationTreeView::keyPressEvent( QKeyEvent* event )
{
    const QModelIndex index = selectionModel()->currentIndex();
    if( event && event->key() == Qt::Key_Delete && index.isValid() )
    {
        if( kernel::Entity_ABC* entity = dataModel_.GetDataFromIndex< kernel::Entity_ABC >( index ) )
            modelObserver_.DeleteEntity( *entity );
    }
    else
        gui::PopulationTreeView::keyPressEvent( event );
}
