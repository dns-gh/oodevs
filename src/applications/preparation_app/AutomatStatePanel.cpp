// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "AutomatStatePanel.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_gui/Tools.h"
#include "clients_gui/PropertiesWidget.h"
#include "clients_kernel/PropertiesDictionary.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: AutomatStatePanel constructor
// Created: SBO 2006-10-11
// -----------------------------------------------------------------------------
AutomatStatePanel::AutomatStatePanel( QWidget* parent, PanelStack_ABC& panel, Controllers& controllers, kernel::EditorFactory_ABC& editorFactory )
    : InfoPanel_ABC( parent, panel, tools::translate( "Preparation", "Automat State" ) )
    , controllers_( controllers )
    , selected_( controllers )
{
    properties_ = new PropertiesWidget( this, "Properties", editorFactory );
    controllers_.Register( *this );
}
    
// -----------------------------------------------------------------------------
// Name: AutomatStatePanel destructor
// Created: SBO 2006-10-11
// -----------------------------------------------------------------------------
AutomatStatePanel::~AutomatStatePanel()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: AutomatStatePanel::showEvent
// Created: SBO 2006-10-11
// -----------------------------------------------------------------------------
void AutomatStatePanel::showEvent( QShowEvent* )
{
    const Automat_ABC* selected = selected_;
    selected_ = 0;
    NotifySelected( selected );
}

// -----------------------------------------------------------------------------
// Name: AutomatStatePanel::NotifySelected
// Created: SBO 2006-10-11
// -----------------------------------------------------------------------------
void AutomatStatePanel::NotifySelected( const kernel::Automat_ABC* element )
{
    if( selected_ != element || ! element )
    {
        selected_ = element;
        if( selected_ )
        {
            Show();
            PropertiesDictionary& dico = const_cast< kernel::Automat_ABC* >( element )->Get< PropertiesDictionary >();
            dico.Display( *properties_ );
        }
        else
        {
            properties_->Clear();
            Hide();
        }
    }
}

// $$$$ AGE 2006-02-16: Centraliser tout ca : 
// -----------------------------------------------------------------------------
// Name: AutomatStatePanel::NotifyDeleted
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void AutomatStatePanel::NotifyDeleted( const Automat_ABC& agent )
{
    if( selected_ = & agent )
        NotifySelected( 0 );
}