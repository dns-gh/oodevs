// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "Spy.h"
#include "moc_Spy.cpp"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "AttributeView.h"
#include "clients_gui/GlWidget.h"
#include "clients_kernel/Positions.h"
#include "gaming/Report_ABC.h"

#include <qtoolbox.h>
#include <qsizepolicy.h>

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: Spy constructor
// Created: SBO 2006-06-21
// -----------------------------------------------------------------------------
Spy::Spy( QWidget* parent, Controllers& controllers )
    : QVBox( parent, tr( "Spy" ) )
    , controllers_( controllers )
    , selected_( controllers )
{
    QVBox* box = new QVBox( this );
    QPushButton* button = new QPushButton( MAKE_ICON( cross ), tr( "Clear" ), box );
    button->setSizePolicy( QSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding ) );
    toolBox_ = new QToolBox( box );
    box->setStretchFactor( toolBox_, 100 );
    connect( button, SIGNAL( clicked() ), this, SLOT( RemoveAll() ) );
    connect( toolBox_, SIGNAL( currentChanged( int ) ), this, SLOT( OnSelect( int ) ) );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: Spy destructor
// Created: SBO 2006-06-21
// -----------------------------------------------------------------------------
Spy::~Spy()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: Spy::NotifyContextMenu
// Created: SBO 2006-06-21
// -----------------------------------------------------------------------------
void Spy::NotifyContextMenu( const Agent_ABC& agent, ContextMenu& menu )
{
    NotifyContextMenu( (Entity_ABC&)agent, menu );
}

// -----------------------------------------------------------------------------
// Name: Spy::NotifyContextMenu
// Created: AGE 2006-06-30
// -----------------------------------------------------------------------------
void Spy::NotifyContextMenu( const Population_ABC& popu, ContextMenu& menu )
{
    NotifyContextMenu( (Entity_ABC&)popu, menu );
}

// -----------------------------------------------------------------------------
// Name: Spy::NotifyDeleted
// Created: AGE 2006-06-30
// -----------------------------------------------------------------------------
void Spy::NotifyDeleted( const Agent_ABC& agent )
{
    RemoveAgent( agent );
}

// -----------------------------------------------------------------------------
// Name: Spy::NotifyDeleted
// Created: AGE 2006-06-30
// -----------------------------------------------------------------------------
void Spy::NotifyDeleted( const Population_ABC& agent )
{
    RemoveAgent( agent );
}

// -----------------------------------------------------------------------------
// Name: Spy::RemoveAgent
// Created: AGE 2006-06-30
// -----------------------------------------------------------------------------
void Spy::RemoveAgent( const Entity_ABC& agent )
{
    if( selected_ == &agent )
        selected_ = 0;
    spied_.erase( &agent );
}

// -----------------------------------------------------------------------------
// Name: Spy::NotifyContextMenu
// Created: AGE 2006-06-30
// -----------------------------------------------------------------------------
void Spy::NotifyContextMenu( const Entity_ABC& agent, ContextMenu& menu )
{
    CIT_Agents it = spied_.find( &agent );
    if( it == spied_.end() )
    {
        selected_ = &agent;
        menu.InsertItem( "Interface", tr( "Spy" ), this, SLOT( Bookmark() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: Spy::Bookmark
// Created: SBO 2006-06-21
// -----------------------------------------------------------------------------
void Spy::Bookmark()
{
    if( !selected_ )
        return;
    CIT_Agents it = spied_.find( selected_ );
    if( it != spied_.end() )
        return;
    int id = toolBox_->addItem( CreateView( *selected_ ), selected_->GetName() );
    spied_[ selected_ ] = id;
    
    selected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: Spy::RemoveAll
// Created: SBO 2006-06-21
// -----------------------------------------------------------------------------
void Spy::RemoveAll()
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
// Name: Spy::CreateView
// Created: AGE 2006-06-22
// -----------------------------------------------------------------------------
QWidget* Spy::CreateView( const Entity_ABC& agent )
{
    return new AttributeView( this, controllers_, agent );
}

// -----------------------------------------------------------------------------
// Name: Spy::NotifyCreated
// Created: SBO 2006-06-26
// -----------------------------------------------------------------------------
void Spy::NotifyCreated( const Report_ABC& report )
{
    const Entity_ABC* agent = &report.GetAgent();
    CIT_Agents it = spied_.find( agent );
    if( it == spied_.end() )
        return;
    toolBox_->setItemIconSet( it->second, MAKE_PIXMAP( msg ) );
}

// -----------------------------------------------------------------------------
// Name: Spy::OnSelect
// Created: SBO 2006-06-26
// -----------------------------------------------------------------------------
void Spy::OnSelect( int index )
{
    for( CIT_Agents it = spied_.begin(); it != spied_.end(); ++it )
        if( it->second == index )
        {
            controllers_.actions_.Select( *it->first );
            toolBox_->setItemIconSet( index, QPixmap() );
            return;
        }
}
