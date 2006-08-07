// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_gui_pch.h"
#include "BigBrother.h"
#include "moc_BigBrother.cpp"
#include "astec_gaming/Controllers.h"
#include "astec_gaming/ActionController.h"
#include "astec_gaming/Agent_ABC.h"
#include "astec_gaming/Population.h"
#include "AttributeView.h"
#include "GlWidget.h"
#include "astec_gaming/Positions.h"
#include "astec_gaming/Report_ABC.h"

#include <qtoolbox.h>
#include <qsizepolicy.h>

// -----------------------------------------------------------------------------
// Name: BigBrother constructor
// Created: SBO 2006-06-21
// -----------------------------------------------------------------------------
BigBrother::BigBrother( QWidget* parent, Controllers& controllers )
    : QVBox( parent, "Espion" )
    , controllers_( controllers )
    , selected_( 0 )
{
    QVBox* box = new QVBox( this );
    QPushButton* button = new QPushButton( MAKE_ICON( cross ), "Vider", box );
    button->setSizePolicy( QSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding ) );
    toolBox_ = new QToolBox( box );
    box->setStretchFactor( toolBox_, 100 );
    connect( button, SIGNAL( clicked() ), this, SLOT( RemoveAll() ) );
    connect( toolBox_, SIGNAL( currentChanged( int ) ), this, SLOT( OnSelect( int ) ) );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: BigBrother destructor
// Created: SBO 2006-06-21
// -----------------------------------------------------------------------------
BigBrother::~BigBrother()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: BigBrother::NotifyContextMenu
// Created: SBO 2006-06-21
// -----------------------------------------------------------------------------
void BigBrother::NotifyContextMenu( const Agent_ABC& agent, ContextMenu& menu )
{
    NotifyContextMenu( (Entity_ABC&)agent, menu );
}

// -----------------------------------------------------------------------------
// Name: BigBrother::NotifyContextMenu
// Created: AGE 2006-06-30
// -----------------------------------------------------------------------------
void BigBrother::NotifyContextMenu( const Population& popu, ContextMenu& menu )
{
    NotifyContextMenu( (Entity_ABC&)popu, menu );
}

// -----------------------------------------------------------------------------
// Name: BigBrother::NotifyDeleted
// Created: AGE 2006-06-30
// -----------------------------------------------------------------------------
void BigBrother::NotifyDeleted( const Agent_ABC& agent )
{
    RemoveAgent( agent );
}

// -----------------------------------------------------------------------------
// Name: BigBrother::NotifyDeleted
// Created: AGE 2006-06-30
// -----------------------------------------------------------------------------
void BigBrother::NotifyDeleted( const Population& agent )
{
    RemoveAgent( agent );
}

// -----------------------------------------------------------------------------
// Name: BigBrother::RemoveAgent
// Created: AGE 2006-06-30
// -----------------------------------------------------------------------------
void BigBrother::RemoveAgent( const Entity_ABC& agent )
{
    if( selected_ == &agent )
        selected_ = 0;
    spied_.erase( &agent );
}

// -----------------------------------------------------------------------------
// Name: BigBrother::NotifyContextMenu
// Created: AGE 2006-06-30
// -----------------------------------------------------------------------------
void BigBrother::NotifyContextMenu( const Entity_ABC& agent, ContextMenu& menu )
{
    CIT_Agents it = spied_.find( &agent );
    if( it == spied_.end() )
    {
        selected_ = &agent;
        menu.InsertItem( "Interface", "Espionner", this, SLOT( Bookmark() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: BigBrother::Bookmark
// Created: SBO 2006-06-21
// -----------------------------------------------------------------------------
void BigBrother::Bookmark()
{
    if( !selected_ )
        return;
    CIT_Agents it = spied_.find( selected_ );
    if( it != spied_.end() )
        return;
    int id = toolBox_->addItem( CreateView( *selected_ ), selected_->GetName().c_str() );
    spied_[ selected_ ] = id;
    
    selected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: BigBrother::RemoveAll
// Created: SBO 2006-06-21
// -----------------------------------------------------------------------------
void BigBrother::RemoveAll()
{
    while( toolBox_->currentIndex() != -1 )
    {
        QWidget* item = toolBox_->currentItem();
        toolBox_->removeItem( item );
        delete item;
    }
    spied_.clear();
}

// -----------------------------------------------------------------------------
// Name: BigBrother::CreateView
// Created: AGE 2006-06-22
// -----------------------------------------------------------------------------
QWidget* BigBrother::CreateView( const Entity_ABC& agent )
{
    return new AttributeView( this, controllers_, agent );
}

// -----------------------------------------------------------------------------
// Name: BigBrother::NotifyCreated
// Created: SBO 2006-06-26
// -----------------------------------------------------------------------------
void BigBrother::NotifyCreated( const Report_ABC& report )
{
    const Entity_ABC* agent = &report.GetAgent();
    CIT_Agents it = spied_.find( agent );
    if( it == spied_.end() )
        return;
    toolBox_->setItemIconSet( it->second, MAKE_PIXMAP( msg ) );
}

// -----------------------------------------------------------------------------
// Name: BigBrother::OnSelect
// Created: SBO 2006-06-26
// -----------------------------------------------------------------------------
void BigBrother::OnSelect( int index )
{
    for( CIT_Agents it = spied_.begin(); it != spied_.end(); ++it )
        if( it->second == index )
        {
            controllers_.actions_.Select( *it->first );
            toolBox_->setItemIconSet( index, QPixmap() );
            return;
        }
}
