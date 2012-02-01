// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

/* TRANSLATOR gui::ObjectPanel */

#include "clients_gui_pch.h"
#include "ObjectPanel.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/Units.h"
#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/StockAttribute_ABC.h"
#include "DisplayBuilder.h"
#include "Tools.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ObjectPanel constructor
// Created: APE 2004-06-11
// -----------------------------------------------------------------------------
ObjectPanel::ObjectPanel( QWidget* parent, PanelStack_ABC& panel, Controllers& controllers, ItemFactory_ABC& factory )
    : InfoPanel_ABC( parent, panel, tools::translate( "gui::ObjectPanel", "Object state" ) )
    , controllers_ ( controllers )
    , selected_    ( controllers )
{    
    pLayout_ = new QVBoxLayout();
    setLayout( pLayout_ );
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
            Clear();
            NotifyUpdated( *selected_ );
            UpdateExtension< MineAttribute_ABC >( *selected_ );
            UpdateExtension< ConstructionAttribute_ABC >( *selected_ );
            UpdateExtension< ObstacleAttribute_ABC >( *selected_ );
            UpdateExtension< BypassAttribute_ABC >( *selected_ );
            UpdateExtension< NBCAttribute_ABC >( *selected_ );
            UpdateExtension< FireAttribute_ABC >( *selected_ );
            UpdateExtension< MedicalTreatmentAttribute_ABC >( *selected_ );
            UpdateExtension< LogisticAttribute_ABC >( *selected_ );
            UpdateExtension< LodgingAttribute_ABC >( *selected_ );
            UpdateExtension< CrossingSiteAttribute_ABC >( *selected_ );
            UpdateExtension< SupplyRouteAttribute_ABC >( *selected_ );
            UpdateExtension< TimeLimitedAttribute_ABC >( *selected_ );
            UpdateExtension< StockAttribute_ABC >( *selected_ );
        }
        else
            Hide();
    }
}

// -----------------------------------------------------------------------------
// Name: ObjectPanel::Clear
// Created: LDC 2009-03-18
// -----------------------------------------------------------------------------
void ObjectPanel::Clear()
{
    display_->Clear();
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
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void ObjectPanel::NotifyUpdated( const MineAttribute_ABC& attributes )
{
    DisplayIfNeeded( attributes );
}

// -----------------------------------------------------------------------------
// Name: ObjectPanel::NotifyUpdated
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void ObjectPanel::NotifyUpdated( const ConstructionAttribute_ABC& attributes )
{
    DisplayIfNeeded( attributes );
}

// -----------------------------------------------------------------------------
// Name: ObjectPanel::NotifyUpdated
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void ObjectPanel::NotifyUpdated( const BypassAttribute_ABC& attributes )
{
    DisplayIfNeeded( attributes );
}

// -----------------------------------------------------------------------------
// Name: ObjectPanel::NotifyUpdated
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void ObjectPanel::NotifyUpdated( const ObstacleAttribute_ABC& attributes )
{
    DisplayIfNeeded( attributes );
}

// -----------------------------------------------------------------------------
// Name: ObjectPanel::NotifyUpdated
// Created: AGE 2006-02-17
// -----------------------------------------------------------------------------
void ObjectPanel::NotifyUpdated( const LogisticAttribute_ABC& attributes )
{
    DisplayIfNeeded( attributes );
}

// -----------------------------------------------------------------------------
// Name: ObjectPanel::NotifyUpdated
// Created: AGE 2006-02-17
// -----------------------------------------------------------------------------
void ObjectPanel::NotifyUpdated( const LodgingAttribute_ABC& attributes )
{
    DisplayIfNeeded( attributes );
}

// -----------------------------------------------------------------------------
// Name: ObjectPanel::NotifyUpdated
// Created: AGE 2006-02-17
// -----------------------------------------------------------------------------
void ObjectPanel::NotifyUpdated( const CrossingSiteAttribute_ABC& attributes )
{
    DisplayIfNeeded( attributes );
}

// -----------------------------------------------------------------------------
// Name: ObjectPanel::NotifyUpdated
// Created: AGE 2006-02-17
// -----------------------------------------------------------------------------
void ObjectPanel::NotifyUpdated( const SupplyRouteAttribute_ABC& attributes )
{
    DisplayIfNeeded( attributes );
}

// -----------------------------------------------------------------------------
// Name: ObjectPanel::NotifyUpdated
// Created: AGE 2006-02-17
// -----------------------------------------------------------------------------
void ObjectPanel::NotifyUpdated( const NBCAttribute_ABC& attributes )
{
    DisplayIfNeeded( attributes );
}

// -----------------------------------------------------------------------------
// Name: ObjectPanel::NotifyUpdated
// Created: RFT 2006-02-17
// -----------------------------------------------------------------------------
void ObjectPanel::NotifyUpdated( const FireAttribute_ABC& attributes )
{
    DisplayIfNeeded( attributes );
}

// -----------------------------------------------------------------------------
// Name: ObjectPanel::NotifyUpdated
// Created: RFT 2006-02-17
// -----------------------------------------------------------------------------
void ObjectPanel::NotifyUpdated( const MedicalTreatmentAttribute_ABC& attributes )
{
    DisplayIfNeeded( attributes );
}

// -----------------------------------------------------------------------------
// Name: ObjectPanel::NotifyUpdated
// Created: AGE 2006-02-17
// -----------------------------------------------------------------------------
void ObjectPanel::NotifyUpdated( const TimeLimitedAttribute_ABC& attributes )
{
    DisplayIfNeeded( attributes );
}

// -----------------------------------------------------------------------------
// Name: ObjectPanel::NotifyUpdated
// Created: BCI 2011-02-07
// -----------------------------------------------------------------------------
void ObjectPanel::NotifyUpdated( const kernel::StockAttribute_ABC& attributes )
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
