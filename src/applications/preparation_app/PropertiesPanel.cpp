// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "PropertiesPanel.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_gui/PropertiesWidget.h"
#include "PropertiesTableDisplayer.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: PropertiesPanel constructor
// Created: SBO 2006-10-27
// -----------------------------------------------------------------------------
PropertiesPanel::PropertiesPanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers, kernel::EditorFactory_ABC& editorFactory )
    : InfoPanel_ABC( parent, panel, tr( "Properties" ) )
    , controllers_( controllers )
    , selected_( controllers )
    , tableItemDisplayer_( *new PropertiesTableDisplayer() )
{
    properties_ = new PropertiesWidget( controllers_.controller_, this, "Properties", editorFactory, tableItemDisplayer_ );
    controllers_.Register( *this );
}
    
// -----------------------------------------------------------------------------
// Name: PropertiesPanel destructor
// Created: SBO 2006-10-27
// -----------------------------------------------------------------------------
PropertiesPanel::~PropertiesPanel()
{
    controllers_.Remove( *this );
    delete &tableItemDisplayer_;
}
    
// -----------------------------------------------------------------------------
// Name: PropertiesPanel::showEvent
// Created: SBO 2006-10-27
// -----------------------------------------------------------------------------
void PropertiesPanel::showEvent( QShowEvent* )
{
    const Entity_ABC* selected = selected_;
    selected_ = 0;
    NotifySelected( selected );
}
    
// -----------------------------------------------------------------------------
// Name: PropertiesPanel::NotifySelected
// Created: SBO 2006-10-27
// -----------------------------------------------------------------------------
void PropertiesPanel::NotifySelected( const kernel::Entity_ABC* element )
{
    if( !element || selected_ != element )
    {
        properties_->Clear();
        selected_ = element;
        if( selected_ )
            if( PropertiesDictionary* dico = const_cast< kernel::Entity_ABC* >( element )->Retrieve< PropertiesDictionary >() )
            {
                Show();
                dico->Display( *properties_ );
                return;
            }
        Hide();
    }
}

// -----------------------------------------------------------------------------
// Name: PropertiesPanel::NotifyDeleted
// Created: SBO 2006-10-27
// -----------------------------------------------------------------------------
void PropertiesPanel::NotifyDeleted( const kernel::Entity_ABC& element )
{
    if( selected_ = &element )
        NotifySelected( 0 );
}
