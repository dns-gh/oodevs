// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

/* TRANSLATOR gui::MiniViews */

#include "clients_gui_pch.h"
#include "MiniViews.h"
#include "moc_MiniViews.cpp"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Positions.h"
#include "MiniView.h"
#include "GlWidget.h"
#include "SmartGridWidget.h"
#include "MiniViewsRenderPass.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: MiniViews constructor
// Created: AGE 2006-06-23
// -----------------------------------------------------------------------------
MiniViews::MiniViews( QMainWindow* parent, Controllers& controllers )
    : QDockWindow( parent, "miniviews" )
    , controllers_( controllers )
    , widget_     ( 0 )
    , selected_   ( controllers_ )
{
    setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );
    setResizeEnabled( true );
    setCloseMode( Always );
    parent->setDockEnabled( this, Qt::DockTop, false );
    setCaption( tr( "Miniviews" ) );
    grid_ = new SmartGridWidget( this, 2, Qt::Vertical, "miniviews" );
    setWidget( grid_ );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: MiniViews destructor
// Created: AGE 2006-06-23
// -----------------------------------------------------------------------------
MiniViews::~MiniViews()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: MiniViews::OnWidget2dChanged
// Created: AGE 2007-03-09
// -----------------------------------------------------------------------------
void MiniViews::OnWidget2dChanged( gui::GlWidget* widget )
{
    if( widget_ && !widget )
        while( ! renderPasses_.empty() )
        {
            widget_->RemovePass( *renderPasses_.begin()->second );
            delete renderPasses_.begin()->second;
            renderPasses_.erase( renderPasses_.begin() );
        }
    widget_ = widget;
    if( ! widget_ )
        while( ! miniViews_.empty() )
        {
            delete miniViews_.begin()->second;
            miniViews_.erase( miniViews_.begin() );
        }
}

// -----------------------------------------------------------------------------
// Name: MiniViews::NotifyContextMenu
// Created: AGE 2006-10-26
// -----------------------------------------------------------------------------
void MiniViews::NotifyContextMenu( const kernel::Entity_ABC& agent, kernel::ContextMenu& menu )
{
    if( widget_ && agent.Retrieve< Positions >() )
    {
        selected_ = &agent;
        bool remove = miniViews_.find( &agent ) != miniViews_.end();
        static const QString supprimer = tr( "Remove miniview" );
        static const QString ajouter   = tr( "Add miniview" );
        menu.InsertItem( "Interface", remove ? supprimer : ajouter , this, SLOT( OnMiniView() ) );
    }
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
            if( GlRenderPass_ABC* pass = renderPasses_[ view ] )
            {
                widget_->RemovePass( *pass );
                delete pass;
                renderPasses_.erase( view );
            }
            delete view;
            miniViews_.erase( selected_ );
            if( miniViews_.empty() )
                hide();
        }
        else
        {
            view = new MiniView( grid_, controllers_, *selected_ );
            GlRenderPass_ABC* pass = new MiniViewsRenderPass( *view );
            renderPasses_[ view ] = pass;
            widget_->AddPass( *pass );
            show();
        }
        selected_ = 0;
    }
}
