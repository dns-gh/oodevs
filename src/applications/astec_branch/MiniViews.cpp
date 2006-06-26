// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "MiniViews.h"
#include "moc_MiniViews.cpp"
#include "Controllers.h"
#include "MiniView.h"
#include "GlWidget.h"
#include "SmartGridWidget.h"

// -----------------------------------------------------------------------------
// Name: MiniViews constructor
// Created: AGE 2006-06-23
// -----------------------------------------------------------------------------
MiniViews::MiniViews( QMainWindow* parent, Controllers& controllers, GlWidget* const& widget )
    : QDockWindow( parent )
    , controllers_( controllers )
    , widget_( widget )
    , selected_( controllers_ )
{
    setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );
    setResizeEnabled( false );
    setCloseMode( Always );
    parent->setDockEnabled( this, Qt::DockTop, false );
    setCaption( tr( "Mini vues" ) );
    grid_ = new SmartGridWidget( this, 2, Qt::Vertical );
    setWidget( grid_ );
   
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: MiniViews destructor
// Created: AGE 2006-06-23
// -----------------------------------------------------------------------------
MiniViews::~MiniViews()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: MiniViews::NotifyContextMenu
// Created: AGE 2006-06-23
// -----------------------------------------------------------------------------
void MiniViews::NotifyContextMenu( const Agent& agent, QPopupMenu& menu )
{
    selected_ = &agent;
    if( menu.count() ) 
        menu.insertSeparator();
    bool remove = miniViews_.find( &agent ) != miniViews_.end();
    menu.insertItem( remove ? "Supprimer la minivue" : "Ajouter une minivue" , this, SLOT( OnMiniView() ) );
}

// -----------------------------------------------------------------------------
// Name: MiniViews::OnMiniView
// Created: AGE 2006-06-23
// -----------------------------------------------------------------------------
void MiniViews::OnMiniView()
{
    if( selected_ && widget_ )
    {
        MiniView*& view = miniViews_[ selected_ ];
        if( view )
        {
            delete view;
            miniViews_.erase( selected_ );
            if( miniViews_.empty() )
                hide();
        }
        else
        {
            view = new MiniView( grid_, controllers_, *selected_, widget_ );
            show();
        }
        selected_ = 0;
    }
}
