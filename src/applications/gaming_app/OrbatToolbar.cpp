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
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_gui/BooleanOptionButton.h"
#include "clients_gui/resources.h"
#include "clients_gui/AutomatsLayer.h"
#include "clients_gui/FormationLayer.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/HierarchyLevel_ABC.h"
#include "gaming/ProfileFilter.h"
#include "gaming/Simulation.h"
#include "gaming/KnowledgeGroup.h"
#include <boost/assign/list_of.hpp>

namespace
{
    static const std::vector< std::string > LEVELS = boost::assign::list_of( "o" )( "oo" )( "ooo" )
                                                                           ( "i" )( "ii" )( "iii" )
                                                                           ( "x" )( "xx" )( "xxx" )( "xxxx" );
}

// -----------------------------------------------------------------------------
// Name: OrbatToolbar constructor
// Created: SBO 2007-03-05
// -----------------------------------------------------------------------------
OrbatToolbar::OrbatToolbar( QWidget* parent, kernel::Controllers& controllers, ProfileFilter& filter,
                            gui::AutomatsLayer& automats, gui::FormationLayer& formations )
    : QHBox( parent )
    , controllers_    ( controllers )
    , filter_         ( filter )
    , entity_         ( controllers_ )
    , automatsLayer_  ( automats )
    , formationsLayer_( formations )
{
    setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Maximum );
    setBackgroundMode( Qt::PaletteButton );
    setFrameStyle( QFrame::ToolBarPanel | QFrame::Raised );

    QToolButton* btn = new QToolButton( this );
    btn->setAutoRaise( true );
    btn->setPopupDelay( 0 );
    btn->setIconSet( MAKE_ICON( aggregate ) );
    QToolTip::add( btn, tr( "Aggregate all automats" ) );
    connect( btn, SIGNAL( clicked() ), SLOT( Aggregate() ) );

    menu_ = new QPopupMenu( btn );
    for( unsigned int i = 0u; i < LEVELS.size(); ++i )
        menu_->insertItem( LEVELS[ i ].c_str(), i );
    btn->setPopup( menu_ );
    connect( menu_, SIGNAL( activated( int ) ), SLOT( Aggregate( int ) ) );

    btn = new QToolButton( this );
    btn->setAutoRaise( true );
    btn->setIconSet( MAKE_ICON( desaggregate ) );
    QToolTip::add( btn, tr( "Disaggregate all" ) );
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
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: OrbatToolbar::OnSetFilter
// Created: AGE 2006-12-13
// -----------------------------------------------------------------------------
void OrbatToolbar::OnSetFilter()
{
    if( entity_ )
    {
        const kernel::CommunicationHierarchies* pCommunicationHierarchies = entity_->Retrieve< kernel::CommunicationHierarchies >();
        if( !pCommunicationHierarchies )
        {
            const kernel::TacticalHierarchies* pTacticalHierarchies = entity_->Retrieve< kernel::TacticalHierarchies >();
            if( pTacticalHierarchies )
                Filter( pTacticalHierarchies->GetTop() );
        }
        else
            Filter( *entity_ );
    }
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
void OrbatToolbar::NotifyUpdated( const ProfileFilter& filter )
{
    entity_ = filter.GetFilter();
    if( entity_ == 0 )
    {
        filterBtn_->setTextLabel( "" );
        filterBtn_->hide();
    }
    else
    {
        filterBtn_->setTextLabel( entity_->GetName() );
        filterBtn_->show();
        filterBtn_->repaint();
    }
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
    IT_Automats it = std::find( automats_.begin(), automats_.end(), &entity );
    if( it != automats_.end() )
    {
        std::swap( *it, automats_.back() );
        automats_.pop_back();
    }
}

// -----------------------------------------------------------------------------
// Name: OrbatToolbar::NotifyCreated
// Created: LGY 2011-03-09
// -----------------------------------------------------------------------------
void OrbatToolbar::NotifyCreated( const kernel::Formation_ABC& entity )
{
    if( std::find( formations_.begin(), formations_.end(), &entity ) == formations_.end() )
        formations_.push_back( &entity );
}

// -----------------------------------------------------------------------------
// Name: OrbatToolbar::NotifyDeleted
// Created: LGY 2011-03-09
// -----------------------------------------------------------------------------
void OrbatToolbar::NotifyDeleted( const kernel::Formation_ABC& entity )
{
    IT_Formations it = std::find( formations_.begin(), formations_.end(), &entity );
    if( it != formations_.end() )
    {
        std::swap( *it, formations_.back() );
        formations_.pop_back();
    }
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
// Name: OrbatToolbar::Aggregate
// Created: SBO 2007-03-05
// -----------------------------------------------------------------------------
void OrbatToolbar::Aggregate()
{
    DisaggregateAll();
    for( CIT_Automats it = automats_.begin(); it != automats_.end(); ++it )
        automatsLayer_.Aggregate( **it );
}

namespace
{
    bool IsValid( const std::string& level, const std::string& maxLevel ) // $$$$ _RC_ LGY 2011-03-09: à remanier
    {
        if( std::distance( LEVELS.begin(), std::find( LEVELS.begin(), LEVELS.end(), level ) ) <=
            std::distance( LEVELS.begin(), std::find( LEVELS.begin(), LEVELS.end(), maxLevel ) ) )
            return true;
        return false;
    }
}

// -----------------------------------------------------------------------------
// Name: OrbatToolbar::Aggregate
// Created: LGY 2011-03-09
// -----------------------------------------------------------------------------
void OrbatToolbar::Aggregate( int id )
{
    DisaggregateAll();
    const std::string level = LEVELS[ id ];
    for( IT_Formations it = formations_.begin(); it != formations_.end(); ++it )
        if( IsValid( (*it)->GetLevel().GetName().ascii(), level ) )
            formationsLayer_.Aggregate( **it );
    for( IT_Automats it = automats_.begin(); it != automats_.end(); ++it )
    {
        const std::string path = (*it)->Get< kernel::TacticalHierarchies >().GetLevel();
        if( path != "" && IsValid( path.substr( 7, path.length() ), level ) ) // $$$$ _RC_ LGY 2011-03-09: à remanier
            automatsLayer_.Aggregate( **it );
    }
    menu_->setItemChecked( id, true );
}

// -----------------------------------------------------------------------------
// Name: OrbatToolbar::Disaggregate
// Created: SBO 2007-03-05
// -----------------------------------------------------------------------------
void OrbatToolbar::DisaggregateAll()
{
    for( unsigned int i = 0u; i < LEVELS.size(); ++i )
        menu_->setItemChecked( i, false );
    for( CIT_Formations it = formations_.begin(); it != formations_.end(); ++it )
        formationsLayer_.Disaggregate( **it );
}

// -----------------------------------------------------------------------------
// Name: OrbatToolbar::minimumSizeHint
// Created: SBO 2007-03-05
// -----------------------------------------------------------------------------
QSize OrbatToolbar::minimumSizeHint() const
{
    return QSize( 64 + filterBtn_->isShown() ? 32 : 0, 32 );
}
