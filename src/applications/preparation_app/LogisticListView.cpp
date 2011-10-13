// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "LogisticListView.h"
#include "ModelBuilder.h"
#include "clients_gui/LongNameHelper.h"
#include "clients_gui/ValuedListItem.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Hierarchies.h"
#include "clients_kernel/LogisticLevel.h"
#include "preparation/LogisticBaseStates.h"
#include "preparation/LogisticLevelAttritube.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: LogisticListView constructor
// Created: ABR 2011-09-14
// -----------------------------------------------------------------------------
LogisticListView::LogisticListView( QWidget* pParent, Controllers& controllers, gui::ItemFactory_ABC& factory, const Profile_ABC& profile,
                                    const gui::EntitySymbols& symbols, ModelBuilder& modelBuilder )
    : gui::LogisticListView( pParent, controllers, factory, profile, symbols, true )
{
    connect( this, SIGNAL( itemRenamed( Q3ListViewItem*, int, const QString& ) ), &modelBuilder, SLOT( OnRename( Q3ListViewItem*, int, const QString& ) ) );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticListView destructor
// Created: ABR 2011-09-14
// -----------------------------------------------------------------------------
LogisticListView::~LogisticListView()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticListView::NotifyUpdated
// Created: ABR 2011-09-19
// -----------------------------------------------------------------------------
void LogisticListView::NotifyUpdated( const kernel::Entity_ABC& entity )
{
    gui::ValuedListItem* item = gui::FindItem( &entity, firstChild() );
    if( item )
        item->setRenameEnabled( 0, !gui::LongNameHelper::SetItemLongName( entity, *item ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticListView::NotifyUpdated
// Created: ABR 2011-09-14
// -----------------------------------------------------------------------------
void LogisticListView::NotifyUpdated( const LogisticBaseStates& hierarchy )
{
    const Entity_ABC& entity = hierarchy.GetEntity();
    FindOrCreateOrReplace( &entity );
}

// -----------------------------------------------------------------------------
// Name: LogisticListView::NotifyUpdated
// Created: ABR 2011-09-15
// -----------------------------------------------------------------------------
void LogisticListView::NotifyUpdated( const LogisticLevelAttritube& logisticLevel )
{
    const Entity_ABC& entity = logisticLevel.GetEntity();
    FindOrCreateOrReplace( &entity );
}

// -----------------------------------------------------------------------------
// Name: LogisticListView::RetrieveSuperior
// Created: ABR 2011-09-16
// -----------------------------------------------------------------------------
const Entity_ABC* LogisticListView::RetrieveSuperior( const Entity_ABC& entity ) const
{
    const LogisticBaseStates* hierarchy = static_cast< const LogisticBaseStates* >( entity.Retrieve< LogisticHierarchiesBase >() );
    return ( hierarchy ) ? hierarchy->GetSuperior() : 0;
}

// -----------------------------------------------------------------------------
// Name: LogisticListView::SetSuperior
// Created: ABR 2011-09-16
// -----------------------------------------------------------------------------
void LogisticListView::SetSuperior( const Entity_ABC& entity, const Entity_ABC* superior )
{
    LogisticHierarchiesBase* hierarchy = const_cast< Entity_ABC& >( entity ).Retrieve< LogisticHierarchiesBase >();
    assert( hierarchy );
    hierarchy->SetLogisticSuperior( ( superior ) ? superior : LogisticBaseSuperior() );
}
