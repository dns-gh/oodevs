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
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Options.h"
#include "clients_gui/BooleanOptionButton.h"
#include "clients_gui/resources.h"
#include "clients_gui/AutomatsLayer.h"
#include "gaming/ProfileFilter.h"

// -----------------------------------------------------------------------------
// Name: OrbatToolbar constructor
// Created: SBO 2007-03-05
// -----------------------------------------------------------------------------
OrbatToolbar::OrbatToolbar( QWidget* parent, kernel::Controllers& controllers, ProfileFilter& filter, gui::AutomatsLayer& automats )
    : QHBox         ( parent )
    , controllers_  ( controllers )
    , filter_       ( filter )
    , entity_       ( controllers_ )
    , automatsLayer_( automats )
{
    setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Maximum );
    setBackgroundMode( Qt::PaletteButton );
    setFrameStyle( QFrame::ToolBarPanel | QFrame::Raised );

    QToolButton* btn = new QToolButton( this );
    btn->setAutoRaise( true );
    btn->setIconSet( MAKE_ICON( aggregate ) );
    QToolTip::add( btn, tr( "Aggregate all automats" ) );
    connect( btn, SIGNAL( clicked() ), SLOT( AggregateAll() ) );

    btn = new QToolButton( this );
    btn->setAutoRaise( true );
    btn->setIconSet( MAKE_ICON( desaggregate ) );
    QToolTip::add( btn, tr( "Disaggregate all automats" ) );
    connect( btn, SIGNAL( clicked() ), SLOT( DisaggregateAll() ) );

    filterBtn_ = new QToolButton( this );
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

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: OrbatToolbar destructor
// Created: SBO 2007-03-05
// -----------------------------------------------------------------------------
OrbatToolbar::~OrbatToolbar()
{
    controllers_.Remove( *this );
}


// -----------------------------------------------------------------------------
// Name: OrbatToolbar::OnSetFilter
// Created: AGE 2006-12-13
// -----------------------------------------------------------------------------
void OrbatToolbar::OnSetFilter()
{
    if( entity_ )
    {
        filterBtn_->setTextLabel( entity_->GetName() );
        filter_.SetFilter( *entity_ );
        filterBtn_->show();
    }
}

// -----------------------------------------------------------------------------
// Name: OrbatToolbar::OnClearFilter
// Created: AGE 2006-12-13
// -----------------------------------------------------------------------------
void OrbatToolbar::OnClearFilter()
{
    filterBtn_->setTextLabel( "" );
    filter_.RemoveFilter();
    filterBtn_->hide();
}

// -----------------------------------------------------------------------------
// Name: OrbatToolbar::NotifyContextMenu
// Created: AGE 2006-12-13
// -----------------------------------------------------------------------------
void OrbatToolbar::NotifyContextMenu( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu )
{
    entity_ = &entity;
    menu.InsertItem( "Interface", tr( "Filter view" ), this, SLOT( OnSetFilter() ) );
}

// -----------------------------------------------------------------------------
// Name: OrbatToolbar::NotifyCreated
// Created: SBO 2007-03-05
// -----------------------------------------------------------------------------
void OrbatToolbar::NotifyCreated( const kernel::Automat_ABC& entity )
{
    if( std::find( automats_.begin(), automats_.end(), &entity ) == automats_.end() )
        automats_.push_back( &entity );
}

// -----------------------------------------------------------------------------
// Name: OrbatToolbar::NotifyDeleted
// Created: SBO 2007-03-05
// -----------------------------------------------------------------------------
void OrbatToolbar::NotifyDeleted( const kernel::Automat_ABC& entity )
{
    IT_Agents it = std::find( automats_.begin(), automats_.end(), &entity );
    if( it != automats_.end() )
    {
        std::swap( *it, automats_.back() );
        automats_.pop_back();
    }
}

// -----------------------------------------------------------------------------
// Name: OrbatToolbar::AggregateAll
// Created: SBO 2007-03-05
// -----------------------------------------------------------------------------
void OrbatToolbar::AggregateAll()
{
    for( CIT_Agents it = automats_.begin(); it != automats_.end(); ++it )
        automatsLayer_.Aggregate( **it );
}

// -----------------------------------------------------------------------------
// Name: OrbatToolbar::DisaggregateAll
// Created: SBO 2007-03-05
// -----------------------------------------------------------------------------
void OrbatToolbar::DisaggregateAll()
{
    for( CIT_Agents it = automats_.begin(); it != automats_.end(); ++it )
        automatsLayer_.Disaggregate( **it );
}

// -----------------------------------------------------------------------------
// Name: OrbatToolbar::minimumSizeHint
// Created: SBO 2007-03-05
// -----------------------------------------------------------------------------
QSize OrbatToolbar::minimumSizeHint() const
{
    return QSize( 64 + filterBtn_->isShown() ? 32 : 0, 32 );
}
