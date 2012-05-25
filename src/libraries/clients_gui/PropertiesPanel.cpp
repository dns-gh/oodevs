// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

/* TRANSLATOR gui::PropertiesPanel */

#include "clients_gui_pch.h"
#include "PropertiesPanel.h"
#include "clients_kernel/EditorFactory_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "GlProxy.h"
#include "PropertiesWidget.h"
#include "TableItemDisplayer.h"
#include "Tools.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: PropertiesPanel constructor
// Created: SBO 2008-04-08
// -----------------------------------------------------------------------------
PropertiesPanel::PropertiesPanel( QWidget* parent, kernel::Controllers& controllers, kernel::EditorFactory_ABC& factory, gui::TableItemDisplayer& displayer, const GlProxy& glProxy )
    : QScrollArea( parent )
    , controllers_( controllers )
    , glProxy_    ( glProxy )
    , selected_   ( 0 )
    , displayer_  ( displayer )
{
    table_ = new gui::PropertiesWidget( controllers_, this, tools::translate( "gui::PropertiesPanel", "Properties" ), factory, displayer_ );
    setWidget( table_ );
    setWidgetResizable( true );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: PropertiesPanel destructor
// Created: SBO 2008-04-08
// -----------------------------------------------------------------------------
PropertiesPanel::~PropertiesPanel()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: PropertiesPanel::showEvent
// Created: SBO 2008-04-08
// -----------------------------------------------------------------------------
void PropertiesPanel::showEvent( QShowEvent* )
{
    const kernel::Entity_ABC* selected = selected_;
    selected_ = 0;
    NotifySelected( selected );
}

// -----------------------------------------------------------------------------
// Name: PropertiesPanel::NotifySelected
// Created: SBO 2008-04-08
// -----------------------------------------------------------------------------
void PropertiesPanel::NotifySelected( const kernel::Entity_ABC* element )
{
    if( !element || selected_ != element )
    {
        table_->Clear();
        selected_ = element;
        if( selected_ && isVisible() )
            if( kernel::PropertiesDictionary* dico = const_cast< kernel::Entity_ABC* >( element )->Retrieve< kernel::PropertiesDictionary >() )
                dico->Display( *table_ );
    }
    setWidget( table_ );
    if( element )
        table_->setEnabled( glProxy_.ShouldEdit( *element ) );
}

// -----------------------------------------------------------------------------
// Name: PropertiesPanel::NotifyDeleted
// Created: SBO 2008-04-08
// -----------------------------------------------------------------------------
void PropertiesPanel::NotifyDeleted( const kernel::Entity_ABC& element )
{
    if( selected_ && selected_->GetId() == element.GetId() )
    {
        NotifySelected( 0 );
        setWidget( table_ );
    }
}
