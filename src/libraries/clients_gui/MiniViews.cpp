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
#include "GlWidget.h"
#include "MiniView.h"
#include "MiniViewsRenderPass.h"
#include "SmartGridWidget.h"
#include "SubObjectName.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/AgentKnowledge_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/ObjectKnowledge_ABC.h"
#include "clients_kernel/UrbanObject_ABC.h"
#include "MiniView.h"
#include "GlWidget.h"
#include "SmartGridWidget.h"
#include "MiniViewsRenderPass.h"
#include "clients_kernel/Profile_ABC.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: MiniViews constructor
// Created: AGE 2006-06-23
// -----------------------------------------------------------------------------
MiniViews::MiniViews( QWidget* parent, kernel::Controllers& controllers, const kernel::Profile_ABC& profile )
    : RichDockWidget( controllers, parent, "miniviews" )
    , profile_( profile )
    , widget_( 0 )
    , selected_( controllers )
{
    SubObjectName subObject( this->objectName() );
    setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );
    setWindowTitle( tr( "Miniviews" ) );
    grid_ = new SmartGridWidget( this, 2, Qt::Vertical, "miniviews" );
    setWidget( grid_ );

    controllers_.Update( *this );
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
// Name: MiniViews::UpdateContextMenu
// Created: AGE 2006-10-26
// -----------------------------------------------------------------------------
void MiniViews::UpdateContextMenu( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu )
{
    if( widget_ && entity.Retrieve< kernel::Positions >() && profile_.IsVisible( entity ) )
    {
        selected_ = &entity;
        bool remove = miniViews_.find( &entity ) != miniViews_.end();
        static const QString supprimer = tr( "Remove miniview" );
        static const QString ajouter   = tr( "Add miniview" );
        menu.InsertItem( "Interface", remove ? supprimer : ajouter , this, SLOT( OnMiniView() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: MiniViews::NotifyContextMenu
// Created: MMC 2013-09-05
// -----------------------------------------------------------------------------
void MiniViews::NotifyContextMenu( const kernel::Agent_ABC& entity, kernel::ContextMenu& menu )
{
    UpdateContextMenu( entity, menu );
}

// -----------------------------------------------------------------------------
// Name: MiniViews::NotifyContextMenu
// Created: MMC 2013-09-05
// -----------------------------------------------------------------------------
void MiniViews::NotifyContextMenu( const kernel::Automat_ABC& entity, kernel::ContextMenu& menu )
{
    UpdateContextMenu( entity, menu );
}

// -----------------------------------------------------------------------------
// Name: MiniViews::NotifyContextMenu
// Created: MMC 2013-09-05
// -----------------------------------------------------------------------------
void MiniViews::NotifyContextMenu( const kernel::Formation_ABC& entity, kernel::ContextMenu& menu )
{
    UpdateContextMenu( entity, menu );
}

// -----------------------------------------------------------------------------
// Name: MiniViews::NotifyContextMenu
// Created: MMC 2013-09-05
// -----------------------------------------------------------------------------
void MiniViews::NotifyContextMenu( const kernel::Population_ABC& entity, kernel::ContextMenu& menu )
{
    UpdateContextMenu( entity, menu );
}

// -----------------------------------------------------------------------------
// Name: MiniViews::NotifyContextMenu
// Created: MMC 2013-09-05
// -----------------------------------------------------------------------------
void MiniViews::NotifyContextMenu( const kernel::Object_ABC& entity, kernel::ContextMenu& menu )
{
    UpdateContextMenu( entity, menu );
}

// -----------------------------------------------------------------------------
// Name: MiniViews::NotifyContextMenu
// Created: MMC 2013-09-05
// -----------------------------------------------------------------------------
void MiniViews::NotifyContextMenu( const kernel::UrbanObject_ABC& entity, kernel::ContextMenu& menu )
{
    UpdateContextMenu( entity, menu );
}

// -----------------------------------------------------------------------------
// Name: MiniViews::NotifyContextMenu
// Created: MMC 2013-09-05
// -----------------------------------------------------------------------------
void MiniViews::NotifyContextMenu( const kernel::AgentKnowledge_ABC& entity, kernel::ContextMenu& menu )
{
    UpdateContextMenu( entity, menu );
}

// -----------------------------------------------------------------------------
// Name: MiniViews::NotifyContextMenu
// Created: MMC 2013-09-05
// -----------------------------------------------------------------------------
void MiniViews::NotifyContextMenu( const kernel::ObjectKnowledge_ABC& entity, kernel::ContextMenu& menu )
{
    UpdateContextMenu( entity, menu );
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
            DeleteView( view, selected_ );
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

// -----------------------------------------------------------------------------
// Name: MiniViews::NotifyDeleted
// Created: JSR 2013-02-04
// -----------------------------------------------------------------------------
void MiniViews::NotifyDeleted( const kernel::Entity_ABC& entity )
{
    auto it = miniViews_.find( &entity );
    if( it != miniViews_.end() )
        DeleteView( it->second, &entity );
}

// -----------------------------------------------------------------------------
// Name: MiniViews::DeleteView
// Created: JSR 2013-02-04
// -----------------------------------------------------------------------------
void MiniViews::DeleteView( MiniView*& view, const kernel::Entity_ABC* entity )
{
    if( GlRenderPass_ABC* pass = renderPasses_[ view ] )
    {
        widget_->RemovePass( *pass );
        delete pass;
        renderPasses_.erase( view );
    }
    delete view;
    miniViews_.erase( entity );
    if( miniViews_.empty() )
        hide();
}
