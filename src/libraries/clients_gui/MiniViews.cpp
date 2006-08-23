// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "MiniViews.h"
#include "moc_MiniViews.cpp"
#include "clients_kernel/Controllers.h"
#include "MiniView.h"
#include "GlWidget.h"
#include "SmartGridWidget.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Population_ABC.h"

using namespace kernel;
using namespace gui;

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
// Name: MiniViews::BuildContextMenu
// Created: SBO 2006-07-05
// -----------------------------------------------------------------------------
void MiniViews::BuildContextMenu( const Entity_ABC& agent, ContextMenu& menu )
{
    selected_ = &agent;
    bool remove = miniViews_.find( &agent ) != miniViews_.end();
    static const QString supprimer = tr( "Supprimer la minivue" );
    static const QString ajouter   = tr( "Ajouter une minivue" );
    menu.InsertItem( "Interface", remove ? supprimer : ajouter , this, SLOT( OnMiniView() ) );
}

// -----------------------------------------------------------------------------
// Name: MiniViews::NotifyContextMenu
// Created: AGE 2006-06-23
// -----------------------------------------------------------------------------
void MiniViews::NotifyContextMenu( const Agent_ABC& agent, ContextMenu& menu )
{
    BuildContextMenu( agent, menu );
}

// -----------------------------------------------------------------------------
// Name: MiniViews::NotifyContextMenu
// Created: SBO 2006-07-05
// -----------------------------------------------------------------------------
void MiniViews::NotifyContextMenu( const Population_ABC& popu, ContextMenu& menu )
{
    BuildContextMenu( popu, menu );
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
