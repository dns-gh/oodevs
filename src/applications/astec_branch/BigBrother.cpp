// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "BigBrother.h"
#include "moc_BigBrother.cpp"
#include "Controllers.h"
#include "Agent.h"
#include "AttributeView.h"
#include "GlWidget.h"
#include "Positions.h"

#include <qtoolbox.h>
#include <qsizepolicy.h>

// -----------------------------------------------------------------------------
// Name: BigBrother constructor
// Created: SBO 2006-06-21
// -----------------------------------------------------------------------------
BigBrother::BigBrother( QWidget* parent, Controllers& controllers )
    : QVBox( parent, "Espion" )
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
void BigBrother::NotifyContextMenu( const Agent& agent, QPopupMenu& menu )
{
    CIT_Agents it = std::find( spied_.begin(), spied_.end(), &agent );
    if( it == spied_.end() )
    {
        if( menu.count() > 0 )
            menu.insertSeparator();
        selected_ = &agent;
        menu.insertItem( "Bookmark", this, SLOT( Bookmark() ) );
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
    CIT_Agents it = std::find( spied_.begin(), spied_.end(), selected_ );
    if( it != spied_.end() )
        return;
    spied_.push_back( selected_ );
    toolBox_->addItem( CreateView( *selected_ ), selected_->GetName().c_str() );
    
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
QWidget* BigBrother::CreateView( const Agent& agent )
{
    return new AttributeView( this, controllers_, agent );
}
