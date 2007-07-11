// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "ObjectPanel.h"

#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/Units.h"
#include "clients_kernel/ObjectExtensions.h"
#include "DisplayBuilder.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ObjectPanel constructor
// Created: APE 2004-06-11
// -----------------------------------------------------------------------------
ObjectPanel::ObjectPanel( QWidget* parent, PanelStack_ABC& panel, Controllers& controllers, ItemFactory_ABC& factory )
    : InfoPanel_ABC( parent, panel, tr( "Object state" ) )
    , controllers_ ( controllers )
    , selected_    ( controllers )
{
    display_ = new DisplayBuilder( this, factory );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ObjectPanel destructor
// Created: APE 2004-06-11
// -----------------------------------------------------------------------------
ObjectPanel::~ObjectPanel()
{
    controllers_.Unregister( *this );
    delete display_;
}

// -----------------------------------------------------------------------------
// Name: ObjectPanel::showEvent
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void ObjectPanel::showEvent( QShowEvent* )
{
    Update();
}

// -----------------------------------------------------------------------------
// Name: ObjectPanel::Update
// Created: AGE 2006-09-08
// -----------------------------------------------------------------------------
void ObjectPanel::Update()
{
    const Object_ABC* selected = selected_;
    selected_ = 0;
    NotifySelected( selected );
}

// -----------------------------------------------------------------------------
// Name: ObjectPanel::UpdateExtension
// Created: AGE 2006-08-03
// -----------------------------------------------------------------------------
template< typename T >
void ObjectPanel::UpdateExtension( const Object_ABC& object )
{
    const T* extension = object.Retrieve< T >();
    if( extension )
        NotifyUpdated( *extension );
}

// -----------------------------------------------------------------------------
// Name: ObjectPanel::NotifySelected
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void ObjectPanel::NotifySelected( const Object_ABC* object )
{
    if( selected_ != object || ! object )
    {
        selected_ = object;
        if( selected_ )
        {
            // $$$$ AGE 2006-02-17: refactor that
            Show();
            display_->Hide();
            NotifyUpdated( *selected_ );
            UpdateExtension< CampAttributes_ABC >( *selected_ );
            UpdateExtension< CrossingSiteAttributes_ABC >( *selected_ );
            UpdateExtension< LogisticRouteAttributes_ABC >( *selected_ );
            UpdateExtension< NBCAttributes_ABC >( *selected_ );
            UpdateExtension< RotaAttributes_ABC >( *selected_ );
        }
        else
            Hide();
    }
}

// -----------------------------------------------------------------------------
// Name: ObjectPanel::NotifyUpdated
// Created: AGE 2006-02-17
// -----------------------------------------------------------------------------
void ObjectPanel::NotifyUpdated( const Object_ABC& object )
{
    if( selected_  != &object || ! IsVisible() )
        return;

    object.Display( *display_ );
}

// $$$$ AGE 2006-02-17: Factor
// -----------------------------------------------------------------------------
// Name: ObjectPanel::NotifyDeleted
// Created: AGE 2006-02-17
// -----------------------------------------------------------------------------
void ObjectPanel::NotifyDeleted( const Object_ABC& object )
{
    if( selected_ == &object )
        NotifySelected( 0 );
}

// -----------------------------------------------------------------------------
// Name: ObjectPanel::ShouldUpdate
// Created: AGE 2006-02-17
// -----------------------------------------------------------------------------
// $$$$ AGE 2006-02-17: Factor
template< typename Extension >
bool ObjectPanel::ShouldUpdate( const Extension& extension )
{
    return IsVisible()
        && selected_ 
        && ( selected_->Retrieve< Extension >() == & extension );
}

// -----------------------------------------------------------------------------
// Name: ObjectPanel::DisplayIfNeeded
// Created: AGE 2006-08-24
// -----------------------------------------------------------------------------
template< typename Extension >
void ObjectPanel::DisplayIfNeeded( const Extension& extension )
{
    if( ShouldUpdate( extension ) )
        extension.Display( *display_ );
}

// -----------------------------------------------------------------------------
// Name: ObjectPanel::NotifyUpdated
// Created: AGE 2006-02-17
// -----------------------------------------------------------------------------
void ObjectPanel::NotifyUpdated( const CampAttributes_ABC& attributes )
{
    DisplayIfNeeded( attributes );
}

// -----------------------------------------------------------------------------
// Name: ObjectPanel::NotifyUpdated
// Created: AGE 2006-02-17
// -----------------------------------------------------------------------------
void ObjectPanel::NotifyUpdated( const CrossingSiteAttributes_ABC& attributes )
{
    DisplayIfNeeded( attributes );
}

// -----------------------------------------------------------------------------
// Name: ObjectPanel::NotifyUpdated
// Created: AGE 2006-02-17
// -----------------------------------------------------------------------------
void ObjectPanel::NotifyUpdated( const LogisticRouteAttributes_ABC& attributes )
{
    DisplayIfNeeded( attributes );
}

// -----------------------------------------------------------------------------
// Name: ObjectPanel::NotifyUpdated
// Created: AGE 2006-02-17
// -----------------------------------------------------------------------------
void ObjectPanel::NotifyUpdated( const NBCAttributes_ABC& attributes )
{
    DisplayIfNeeded( attributes );
}

// -----------------------------------------------------------------------------
// Name: ObjectPanel::NotifyUpdated
// Created: AGE 2006-02-17
// -----------------------------------------------------------------------------
void ObjectPanel::NotifyUpdated( const RotaAttributes_ABC& attributes )
{
    DisplayIfNeeded( attributes );
}

// -----------------------------------------------------------------------------
// Name: ObjectPanel::GetSelected
// Created: AGE 2006-09-08
// -----------------------------------------------------------------------------
const kernel::Object_ABC* ObjectPanel::GetSelected()
{
    return selected_;
}

// -----------------------------------------------------------------------------
// Name: ObjectPanel::GetBuilder
// Created: AGE 2006-09-08
// -----------------------------------------------------------------------------
DisplayBuilder& ObjectPanel::GetBuilder()
{
    return *display_;
}
