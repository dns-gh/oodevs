// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "Bookmarks.h"
#include "moc_Bookmarks.cpp"
#include "Controllers.h"
#include "Agent.h"
#include "AttributeView.h"
#include "GlWidget.h"
#include "Positions.h"
#include "MiniView.h"

#include <qtoolbox.h>
#include <qsizepolicy.h>

// -----------------------------------------------------------------------------
// Name: Bookmarks constructor
// Created: SBO 2006-06-21
// -----------------------------------------------------------------------------
Bookmarks::Bookmarks( QWidget* parent, Controllers& controllers )
    : QVBox( parent, "bookmarks" )
    , controllers_( controllers )
    , selected_( controllers )
{
    QVBox* box = new QVBox( this );
    QPushButton* button = new QPushButton( MAKE_ICON( cross ), "Vider", box );
    button->setSizePolicy( QSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding ) );
    toolBox_ = new QToolBox( box );
    box->setStretchFactor( toolBox_, 100 );
    connect( button, SIGNAL( clicked() ), this, SLOT( RemoveAll() ) );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: Bookmarks destructor
// Created: SBO 2006-06-21
// -----------------------------------------------------------------------------
Bookmarks::~Bookmarks()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: Bookmarks::NotifyContextMenu
// Created: SBO 2006-06-21
// -----------------------------------------------------------------------------
void Bookmarks::NotifyContextMenu( const Agent& agent, QPopupMenu& menu )
{
    CIT_Agents it = std::find( bookmarks_.begin(), bookmarks_.end(), &agent );
    if( it == bookmarks_.end() )
    {
        if( menu.count() > 0 )
            menu.insertSeparator();
        selected_ = &agent;
        menu.insertItem( "Bookmark", this, SLOT( Bookmark() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: Bookmarks::Bookmark
// Created: SBO 2006-06-21
// -----------------------------------------------------------------------------
void Bookmarks::Bookmark()
{
    if( !selected_ )
        return;
    CIT_Agents it = std::find( bookmarks_.begin(), bookmarks_.end(), selected_ );
    if( it != bookmarks_.end() )
        return;
    bookmarks_.push_back( selected_ );
    toolBox_->addItem( CreateView( *selected_ ), selected_->GetName().c_str() );
    
    selected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: Bookmarks::RemoveAll
// Created: SBO 2006-06-21
// -----------------------------------------------------------------------------
void Bookmarks::RemoveAll()
{
    while( toolBox_->currentIndex() != -1 )
    {
        QWidget* item = toolBox_->currentItem();
        toolBox_->removeItem( item );
        delete item;
    }
    bookmarks_.clear();
}

// -----------------------------------------------------------------------------
// Name: Bookmarks::CreateView
// Created: AGE 2006-06-22
// -----------------------------------------------------------------------------
QWidget* Bookmarks::CreateView( const Agent& agent )
{
    return new AttributeView( this, controllers_, agent );
}
