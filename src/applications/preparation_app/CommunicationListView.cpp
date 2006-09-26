// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "CommunicationListView.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "ModelBuilder.h"
#include "clients_gui/Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: CommunicationListView constructor
// Created: SBO 2006-09-25
// -----------------------------------------------------------------------------
CommunicationListView::CommunicationListView( QWidget* parent, Controllers& controllers, gui::ItemFactory_ABC& factory, ModelBuilder& modelBuilder )
    : gui::CommunicationListView( parent, controllers, factory )
    , factory_( factory )
    , modelBuilder_( modelBuilder )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CommunicationListView destructor
// Created: SBO 2006-09-25
// -----------------------------------------------------------------------------
CommunicationListView::~CommunicationListView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CommunicationListView::Display
// Created: SBO 2006-09-25
// -----------------------------------------------------------------------------
void CommunicationListView::Display( const CommunicationHierarchies& hierarchy, gui::ValuedListItem* item )
{
    if( ! hierarchy.GetSuperior() )
        item->setRenameEnabled( 0, true );
}

// -----------------------------------------------------------------------------
// Name: CommunicationListView::NotifyUpdated
// Created: SBO 2006-09-26
// -----------------------------------------------------------------------------
void CommunicationListView::NotifyUpdated( const ModelLoaded& )
{
    clear();
}
    
// -----------------------------------------------------------------------------
// Name: CommunicationListView::NotifyContextMenu
// Created: SBO 2006-09-26
// -----------------------------------------------------------------------------
void CommunicationListView::NotifyContextMenu( const Team_ABC& agent, ContextMenu& menu )
{
    if( !isVisible() )
        return;
    menu.InsertItem( "Commande", tools::translate( "Preparation", "Créer un groupe de connaissance" ), &modelBuilder_, SLOT( OnCreateCommunication() ) );
}
