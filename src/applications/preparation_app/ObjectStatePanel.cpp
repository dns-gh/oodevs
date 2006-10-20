// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "ObjectStatePanel.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_gui/Tools.h"
#include "clients_gui/PropertiesWidget.h"
#include "clients_kernel/PropertiesDictionary.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ObjectStatePanel constructor
// Created: SBO 2006-10-20
// -----------------------------------------------------------------------------
ObjectStatePanel::ObjectStatePanel( QWidget* parent, PanelStack_ABC& panel, Controllers& controllers )
    : InfoPanel_ABC( parent, panel, tools::translate( "Preparation", "Object State" ) )
    , controllers_( controllers )
    , selected_( controllers )
{
    properties_ = new PropertiesWidget( this, "Properties" );
    controllers_.Register( *this );
}
    
// -----------------------------------------------------------------------------
// Name: ObjectStatePanel destructor
// Created: SBO 2006-10-20
// -----------------------------------------------------------------------------
ObjectStatePanel::~ObjectStatePanel()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: ObjectStatePanel::NotifySelected
// Created: SBO 2006-10-20
// -----------------------------------------------------------------------------
void ObjectStatePanel::NotifySelected( const kernel::Object_ABC* element )
{
    if( selected_ != element || ! element )
    {
        selected_ = element;
        if( selected_ )
        {
            Show();
            PropertiesDictionary& dico = const_cast< kernel::Object_ABC* >( element )->Get< PropertiesDictionary >();
            dico.Display( *properties_ );
        }
        else
            Hide();
    }
}

// -----------------------------------------------------------------------------
// Name: ObjectStatePanel::showEvent
// Created: SBO 2006-10-20
// -----------------------------------------------------------------------------
void ObjectStatePanel::showEvent( QShowEvent* )
{
    const Object_ABC* selected = selected_;
    selected_ = 0;
    NotifySelected( selected );    
}

// -----------------------------------------------------------------------------
// Name: ObjectStatePanel::NotifyDeleted
// Created: SBO 2006-10-20
// -----------------------------------------------------------------------------
void ObjectStatePanel::NotifyDeleted( const kernel::Object_ABC& object )
{
    if( selected_ = & object )
        NotifySelected( 0 );
}
