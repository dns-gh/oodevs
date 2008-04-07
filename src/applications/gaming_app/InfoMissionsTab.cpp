// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "InfoMissionsTab.h"
#include "GamingListItemDisplayer.h"
#include "clients_kernel/Controllers.h"
#include "gaming/Action_ABC.h"
#include "gaming/ActionParameter_ABC.h"
#include "gaming/MissionParameters.h"
#include "icons.h"

// -----------------------------------------------------------------------------
// Name: InfoMissionsTab constructor
// Created: SBO 2007-04-18
// -----------------------------------------------------------------------------
InfoMissionsTab::InfoMissionsTab( QTabWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
    : gui::ListDisplayer< InfoMissionsTab >( parent, *this, factory )
    , controllers_( controllers )
    , sub_ ( new GamingListItemDisplayer() )
    , selected_( controllers )
    , parent_( parent )
{
    setMargin( 2 );
    setFrameStyle( QFrame::Plain );
    AddColumn( tr( "Action" ) );
    AddColumn( tr( "Value" ) );
    header()->hide();
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: InfoMissionsTab destructor
// Created: SBO 2007-04-18
// -----------------------------------------------------------------------------
InfoMissionsTab::~InfoMissionsTab()
{
    controllers_.Unregister( *this );
    delete sub_;
}

// -----------------------------------------------------------------------------
// Name: InfoMissionsTab::Display
// Created: SBO 2007-04-18
// -----------------------------------------------------------------------------
void InfoMissionsTab::Display( const Action_ABC& action, kernel::Displayer_ABC& displayer, gui::ValuedListItem* item )
{
    item->setPixmap( 0, MAKE_PIXMAP( mission ) );
    action.Display( displayer );
    DeleteTail( DisplayList( action.CreateIterator(), item ) );
}

// -----------------------------------------------------------------------------
// Name: InfoMissionsTab::Display
// Created: SBO 2007-04-18
// -----------------------------------------------------------------------------
void InfoMissionsTab::Display( const ActionParameter_ABC& param, kernel::Displayer_ABC& /*displayer*/, gui::ValuedListItem* item )
{
    item->setPixmap( 0, MAKE_PIXMAP( parameter2 ) );
    param.Display( (*sub_)( item ) );
    DeleteTail( DisplayList( param.CreateIterator(), item ) );
}

// -----------------------------------------------------------------------------
// Name: InfoMissionsTab::AddColumn
// Created: SBO 2007-04-18
// -----------------------------------------------------------------------------
void InfoMissionsTab::AddColumn( const QString& column )
{
    gui::ListDisplayer< InfoMissionsTab >::AddColumn( column );
    sub_->AddColumn( column );
}

// -----------------------------------------------------------------------------
// Name: InfoMissionsTab::NotifySelected
// Created: SBO 2007-04-18
// -----------------------------------------------------------------------------
void InfoMissionsTab::NotifySelected( const kernel::Entity_ABC* entity )
{
    if( selected_ != entity )
    {
        selected_ = entity;
        const MissionParameters* extension = selected_ ? selected_->Retrieve< MissionParameters >() : 0;
        if( extension )
            NotifyUpdated( *extension );
        else
            clear();
        parent_->setTabEnabled( this, extension );
    }
}

// -----------------------------------------------------------------------------
// Name: InfoMissionsTab::NotifyUpdated
// Created: SBO 2007-04-18
// -----------------------------------------------------------------------------
void InfoMissionsTab::NotifyUpdated( const MissionParameters& extension )
{
    if( ShouldUpdate( extension ) )
        DeleteTail( DisplayList( extension.CreateIterator() ) );
}

// -----------------------------------------------------------------------------
// Name: InfoMissionsTab::ShouldUpdate
// Created: SBO 2007-04-18
// -----------------------------------------------------------------------------
bool InfoMissionsTab::ShouldUpdate( const MissionParameters& extension ) const
{
    return isVisible() && selected_ && selected_->Retrieve< MissionParameters >() == &extension;
}

// -----------------------------------------------------------------------------
// Name: InfoMissionsTab::showEvent
// Created: SBO 2007-04-18
// -----------------------------------------------------------------------------
void InfoMissionsTab::showEvent( QShowEvent* event )
{
    const kernel::Entity_ABC* selected = selected_;
    selected_ = 0;
    NotifySelected( selected );
    gui::ListDisplayer< InfoMissionsTab >::showEvent( event );
}
