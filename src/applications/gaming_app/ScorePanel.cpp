// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ScorePanel.h"
#include "clients_gui/ListDisplayer.h"
#include "clients_kernel/Controllers.h"
#include "gaming/Score.h"
#include "gaming/Tools.h"

namespace
{
    class MyList : public gui::ListDisplayer< ScorePanel >
    {
    public:
        MyList( ScorePanel* parent, gui::ItemFactory_ABC& factory )
            : gui::ListDisplayer< ScorePanel >( parent, *parent, factory ) {}
    };
}

// -----------------------------------------------------------------------------
// Name: ScorePanel constructor
// Created: SBO 2009-03-12
// -----------------------------------------------------------------------------
ScorePanel::ScorePanel( QMainWindow* mainWindow, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
    : QDockWindow( mainWindow, "score" )
    , controllers_( controllers )
    , factory_( factory )
{
    setResizeEnabled( true );
    setCloseMode( QDockWindow::Always );
    setCaption( tools::translate( "ScorePanel", "Scores" ) );

    scores_ = new MyList( this, factory_ );
    scores_->AddColumn( tools::translate( "Score", "Name" ) );
    scores_->AddColumn( tools::translate( "Score", "Value" ) );
    setWidget( scores_ );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ScorePanel destructor
// Created: SBO 2009-03-12
// -----------------------------------------------------------------------------
ScorePanel::~ScorePanel()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ScorePanel::NotifyCreated
// Created: SBO 2009-03-12
// -----------------------------------------------------------------------------
void ScorePanel::NotifyCreated( const Score& element )
{
    gui::ValuedListItem* item = factory_.CreateItem( scores_ );
    Display( element, item );
}

// -----------------------------------------------------------------------------
// Name: ScorePanel::NotifyUpdated
// Created: SBO 2009-03-12
// -----------------------------------------------------------------------------
void ScorePanel::NotifyUpdated( const Score& element )
{
    if( gui::ValuedListItem* item = gui::FindItem( &element, scores_->firstChild() ) )
        Display( element, item );
}

// -----------------------------------------------------------------------------
// Name: ScorePanel::NotifyDeleted
// Created: SBO 2009-03-12
// -----------------------------------------------------------------------------
void ScorePanel::NotifyDeleted( const Score& element )
{
    // $$$$ SBO 2009-03-12: TODO !
}

// -----------------------------------------------------------------------------
// Name: ScorePanel::Display
// Created: SBO 2009-03-12
// -----------------------------------------------------------------------------
void ScorePanel::Display( const Score& score, gui::ValuedListItem* item )
{
    item->SetValue( &score );
    score.Display( scores_->GetItemDisplayer( item ) );
}
