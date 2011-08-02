// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "IntelligenceListView.h"
#include "moc_IntelligenceListView.cpp"
#include "EntitySymbols.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: IntelligenceListView constructor
// Created: SBO 2007-10-18
// -----------------------------------------------------------------------------
IntelligenceListView::IntelligenceListView( QWidget* parent, Controllers& controllers, ItemFactory_ABC& factory, EntitySymbols& icons, const Profile_ABC& profile )
    : HierarchyListView< IntelligenceHierarchies >( parent, controllers, factory, profile, icons )
    , icons_( icons )
{
    connect( this, SIGNAL( itemRenamed( Q3ListViewItem*, int, const QString& ) ), SLOT( OnRename( Q3ListViewItem*, int, const QString& ) ) );
}

// -----------------------------------------------------------------------------
// Name: IntelligenceListView destructor
// Created: SBO 2007-10-18
// -----------------------------------------------------------------------------
IntelligenceListView::~IntelligenceListView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IntelligenceListView::NotifyUpdated
// Created: SBO 2007-10-18
// -----------------------------------------------------------------------------
void IntelligenceListView::NotifyUpdated( const ModelLoaded& )
{
    clear();
}

// -----------------------------------------------------------------------------
// Name: IntelligenceListView::keyPressEvent
// Created: SBO 2007-10-18
// -----------------------------------------------------------------------------
void IntelligenceListView::keyPressEvent( QKeyEvent* event )
{
    // $$$$ SBO 2007-10-19: TODO!
//    if( event->key() == Qt::Key_Delete )
//        if( const Entity_ABC* entity = selectedItem())->GetValue< const Entity_ABC >() )
//            // $$$$ SBO 2007-10-19: could delete any team/formation/intelligence, maybe add Entity_ABC::Delete()
//            if( const Intelligence_ABC* intelligence = dynamic_cast< const Intelligence_ABC* >( entity ) )
//            {
//                Delete( *entity ); // $$$$ SBO 2007-10-19: virtual or something...
//                event->accept();
//                return;
//            }
    Q3ListView::keyPressEvent( event );
}

// -----------------------------------------------------------------------------
// Name: IntelligenceListView::OnRename
// Created: SBO 2007-10-19
// -----------------------------------------------------------------------------
void IntelligenceListView::OnRename( Q3ListViewItem* item, int /*col*/, const QString& name )
{
    if( const Entity_ABC* entity = static_cast< ValuedListItem* >( item )->GetValue< const Entity_ABC >() )
        // $$$$ SBO 2007-10-19: could rename any team/formation/intelligence, maybe add Entity_ABC::Rename( const QString& )
        if( const Intelligence_ABC* intelligence = dynamic_cast< const Intelligence_ABC* >( entity ) )
            const_cast< Intelligence_ABC* >( intelligence )->Rename( name );
}

// -----------------------------------------------------------------------------
// Name: IntelligenceListView::DisplayIcon
// Created: SBO 2007-11-02
// -----------------------------------------------------------------------------
void IntelligenceListView::DisplayIcon( const kernel::Entity_ABC& entity, gui::ValuedListItem* item )
{
    if( const Intelligence_ABC* intelligence = dynamic_cast< const Intelligence_ABC* >( &entity ) )
    {
        QPixmap pixmap = icons_.GetSymbol( *intelligence );
        if( pixmap.isNull() )
            QTimer::singleShot( 500, this, SLOT( Update() ) );
        item->setPixmap( 0, pixmap );
        item->setRenameEnabled( 0, true );
    }
    else
        HierarchyListView< kernel::IntelligenceHierarchies >::DisplayIcon( entity, item );
}
