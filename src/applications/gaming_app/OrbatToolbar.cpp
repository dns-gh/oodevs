// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "OrbatToolbar.h"
#include "moc_OrbatToolbar.cpp"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/AgentKnowledge_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/ObjectKnowledge_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_gui/resources.h"
#include "clients_gui/AggregateToolbar.h"
#include "gaming/ProfileFilter.h"
#include "gaming/Profile.h"
#include "gaming/UnitFilter.h"
#include "gaming/Simulation.h"

// -----------------------------------------------------------------------------
// Name: OrbatToolbar constructor
// Created: SBO 2007-03-05
// -----------------------------------------------------------------------------
OrbatToolbar::OrbatToolbar( QWidget* parent, kernel::Controllers& controllers, ProfileFilter& filter,
                            gui::AutomatsLayer& automats, gui::FormationLayer& formations )
    : QFrame( parent )
    , controllers_( controllers )
    , filter_     ( filter )
    , entity_     ( controllers_ )
{
    setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Maximum );
    setBackgroundMode( Qt::PaletteButton );
    setFrameStyle( QFrame::StyledPanel | QFrame::Raised );
    setMinimumWidth( 150 );
    setMaximumWidth( 440 );

    QGridLayout* toolbarBox = new QGridLayout( this );
    pAggregateToolbar_ = new gui::AggregateToolbar( controllers.controller_, automats, formations, true );
    toolbarBox->addLayout( pAggregateToolbar_, 0, 0, 1, 1, Qt::AlignLeft );

    filterBtn_ = new QToolButton();
    filterBtn_->setAutoRaise( true );
    QFont font;
    font.setPixelSize( 14 );
    font.setBold( true );
    font.setItalic( true );
    filterBtn_->setFont( font );
    filterBtn_->setIconSet( MAKE_ICON( filter ) );
    filterBtn_->setTextPosition( QToolButton::BesideIcon );
    filterBtn_->setUsesTextLabel( true );
    QToolTip::add( filterBtn_, tr( "Remove filter" ) );
    connect( filterBtn_, SIGNAL( clicked() ), SLOT( OnClearFilter() ) );
    filterBtn_->hide();
    toolbarBox->addWidget( filterBtn_, 0, 1, 1, 1, Qt::AlignRight );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: OrbatToolbar::GetToolbar
// Created: JSR 2012-06-28
// -----------------------------------------------------------------------------
const gui::AggregateToolbar* OrbatToolbar::GetToolbar() const
{
    return pAggregateToolbar_;
}

// -----------------------------------------------------------------------------
// Name: OrbatToolbar destructor
// Created: SBO 2007-03-05
// -----------------------------------------------------------------------------
OrbatToolbar::~OrbatToolbar()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: OrbatToolbar::OnSetFilter
// Created: AGE 2006-12-13
// -----------------------------------------------------------------------------
void OrbatToolbar::OnSetFilter()
{
    if( entity_ )
        Filter( *entity_ );
}

// -----------------------------------------------------------------------------
// Name: OrbatToolbar::Filter
// Created: LGY 2010-07-30
// -----------------------------------------------------------------------------
void OrbatToolbar::Filter( const kernel::Entity_ABC& entity )
{
    filter_.SetFilter( entity );
    entity.Select( controllers_.actions_ );
}

// -----------------------------------------------------------------------------
// Name: OrbatToolbar::OnClearFilter
// Created: AGE 2006-12-13
// -----------------------------------------------------------------------------
void OrbatToolbar::OnClearFilter()
{
    filter_.RemoveFilter();
}

// -----------------------------------------------------------------------------
// Name: OrbatToolbar::NotifyUpdated
// Created: SBO 2009-03-04
// -----------------------------------------------------------------------------
void OrbatToolbar::NotifyUpdated( const kernel::Filter_ABC& filter )
{
    const QString name = filter.GetFilter();
    filterBtn_->setTextLabel( name );
    if( name == "" )
        filterBtn_->hide();
    else
    {
        filterBtn_->show();
        filterBtn_->repaint();
    }
}

// -----------------------------------------------------------------------------
// Name: OrbatToolbar::UpdateContextMenu
// Created: MMC 2013-09-11
// -----------------------------------------------------------------------------
void OrbatToolbar::UpdateContextMenu( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu )
{
    if( !filter_.IsVisible( entity ) || entity.GetId() == 0 ) // no side team
        return;
    entity_ = &entity;
    menu.InsertItem( "Interface", tr( "Filter view" ), this, SLOT( OnSetFilter() ) );
}

// -----------------------------------------------------------------------------
// Name: OrbatToolbar::NotifyContextMenu
// Created: MMC 2013-09-05
// -----------------------------------------------------------------------------
void OrbatToolbar::NotifyContextMenu( const kernel::Agent_ABC& entity, kernel::ContextMenu& menu )
{
    UpdateContextMenu( entity, menu );
}

// -----------------------------------------------------------------------------
// Name: OrbatToolbar::NotifyContextMenu
// Created: MMC 2013-09-05
// -----------------------------------------------------------------------------
void OrbatToolbar::NotifyContextMenu( const kernel::Automat_ABC& entity, kernel::ContextMenu& menu )
{
    UpdateContextMenu( entity, menu );
}

// -----------------------------------------------------------------------------
// Name: OrbatToolbar::NotifyContextMenu
// Created: MMC 2013-09-05
// -----------------------------------------------------------------------------
void OrbatToolbar::NotifyContextMenu( const kernel::Formation_ABC& entity, kernel::ContextMenu& menu )
{
    UpdateContextMenu( entity, menu );
}

// -----------------------------------------------------------------------------
// Name: OrbatToolbar::NotifyContextMenu
// Created: MMC 2013-09-05
// -----------------------------------------------------------------------------
void OrbatToolbar::NotifyContextMenu( const kernel::Population_ABC& entity, kernel::ContextMenu& menu )
{
    UpdateContextMenu( entity, menu );
}

// -----------------------------------------------------------------------------
// Name: OrbatToolbar::NotifyContextMenu
// Created: MMC 2013-09-05
// -----------------------------------------------------------------------------
void OrbatToolbar::NotifyContextMenu( const kernel::Object_ABC& entity, kernel::ContextMenu& menu )
{
    UpdateContextMenu( entity, menu );
}

// -----------------------------------------------------------------------------
// Name: OrbatToolbar::NotifyContextMenu
// Created: MMC 2013-09-05
// -----------------------------------------------------------------------------
void OrbatToolbar::NotifyContextMenu( const kernel::Team_ABC& entity, kernel::ContextMenu& menu )
{
    UpdateContextMenu( entity, menu );
}

// -----------------------------------------------------------------------------
// Name: OrbatToolbar::NotifyContextMenu
// Created: MMC 2013-09-05
// -----------------------------------------------------------------------------
void OrbatToolbar::NotifyContextMenu( const kernel::AgentKnowledge_ABC& entity, kernel::ContextMenu& menu )
{
    UpdateContextMenu( entity, menu );
}

// -----------------------------------------------------------------------------
// Name: OrbatToolbar::NotifyContextMenu
// Created: MMC 2013-09-05
// -----------------------------------------------------------------------------
void OrbatToolbar::NotifyContextMenu( const kernel::ObjectKnowledge_ABC& entity, kernel::ContextMenu& menu )
{
    UpdateContextMenu( entity, menu );
}

// -----------------------------------------------------------------------------
// Name: OrbatToolbar::NotifyUpdated
// Created: AGE 2007-05-09
// -----------------------------------------------------------------------------
void OrbatToolbar::NotifyUpdated( const Simulation& simu )
{
    if( ! simu.IsConnected() )
        OnClearFilter();
}

// -----------------------------------------------------------------------------
// Name: OrbatToolbar::NotifyUpdated
// Created: LGY 2011-11-25
// -----------------------------------------------------------------------------
void OrbatToolbar::NotifyUpdated( const Profile& profile )
{
    if( current_ != profile.GetLogin() )
    {
        OnClearFilter();
        current_ = profile.GetLogin();
    }
}

// -----------------------------------------------------------------------------
// Name: OrbatToolbar::minimumSizeHint
// Created: SBO 2007-03-05
// -----------------------------------------------------------------------------
QSize OrbatToolbar::minimumSizeHint() const
{
    return QSize( 64 + filterBtn_->isShown() ? 32 : 0, 32 );
}
