// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

/* TRANSLATOR gui::AggregateToolbar */

#include "clients_gui_pch.h"
#include "AggregateToolbar.h"
#include "moc_AggregateToolbar.cpp"
#include "AutomatsLayer.h"
#include "FormationLayer.h"
#include "resources.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/HierarchyLevel_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Tools.h"
#include <boost/assign/list_of.hpp>

using namespace gui;

namespace
{
    static const std::vector< std::string > LEVELS = boost::assign::list_of( "o" )( "oo" )( "ooo" )
                                                                           ( "i" )( "ii" )( "iii" )
                                                                           ( "x" )( "xx" )( "xxx" )( "xxxx" );
}

// -----------------------------------------------------------------------------
// Name: AggregateToolbar constructor
// Created: LGY 2011-10-14
// -----------------------------------------------------------------------------
AggregateToolbar::AggregateToolbar( QWidget* parent, kernel::Controller& controller,
                                    AutomatsLayer& automatsLayer, FormationLayer& formationsLayer )
    : Q3HBox( parent, "AggregateToolbar" )
    , controller_     ( controller )
    , automatsLayer_  ( automatsLayer )
    , formationsLayer_( formationsLayer )
{
    QToolButton* btn = new QToolButton( this );
    btn->setAutoRaise( true );
    btn->setPopupDelay( 0 );
    btn->setIconSet( MAKE_ICON( aggregate ) );
    btn->setPopupMode( QToolButton::MenuButtonPopup );
    QToolTip::add( btn, tools::translate( "AggregateToolbar", "Aggregate all automats" ) );
    connect( btn, SIGNAL( clicked() ), SLOT( Aggregate() ) );

    menu_ = new Q3PopupMenu( btn );
    for( unsigned int i = 0u; i < LEVELS.size(); ++i )
        menu_->insertItem( LEVELS[ i ].c_str(), i );
    btn->setPopup( menu_ );
    connect( menu_, SIGNAL( activated( int ) ), SLOT( Aggregate( int ) ) );

    btn = new QToolButton( this );
    btn->setAutoRaise( true );
    btn->setIconSet( MAKE_ICON( desaggregate ) );
    QToolTip::add( btn, tools::translate( "AggregateToolbar", "Disaggregate all" ) );
    connect( btn, SIGNAL( clicked() ), SLOT( DisaggregateAll() ) );
    controller_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: AggregateToolbar destructor
// Created: LGY 2011-10-14
// -----------------------------------------------------------------------------
AggregateToolbar::~AggregateToolbar()
{
    controller_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: AggregateToolbar::Aggregate
// Created: LGY 2011-10-14
// -----------------------------------------------------------------------------
void AggregateToolbar::Aggregate()
{
    DisaggregateAll();
    for( CIT_Automats it = automats_.begin(); it != automats_.end(); ++it )
        automatsLayer_.Aggregate( **it );
}

// -----------------------------------------------------------------------------
// Name: AggregateToolbar::DisaggregateAll
// Created: LGY 2011-10-14
// -----------------------------------------------------------------------------
void AggregateToolbar::DisaggregateAll()
{
    for( unsigned int i = 0u; i < LEVELS.size(); ++i )
        menu_->setItemChecked( i, false );
    for( CIT_Formations it = formations_.begin(); it != formations_.end(); ++it )
        formationsLayer_.Disaggregate( **it );
}

namespace
{
    bool IsValid( const std::string& level, const std::string& maxLevel )
    {
        if( std::distance( LEVELS.begin(), std::find( LEVELS.begin(), LEVELS.end(), level ) ) <=
            std::distance( LEVELS.begin(), std::find( LEVELS.begin(), LEVELS.end(), maxLevel ) ) )
            return true;
        return false;
    }
}

// -----------------------------------------------------------------------------
// Name: AggregateToolbar::Aggregate
// Created: LGY 2011-10-14
// -----------------------------------------------------------------------------
void AggregateToolbar::Aggregate( int id )
{
    DisaggregateAll();
    const std::string level = LEVELS[ id ];
    for( IT_Formations it = formations_.begin(); it != formations_.end(); ++it )
        if( IsValid( (*it)->GetLevel().GetName().ascii(), level ) )
            formationsLayer_.Aggregate( **it );
    for( IT_Automats it = automats_.begin(); it != automats_.end(); ++it )
    {
        const std::string path = (*it)->Get< kernel::TacticalHierarchies >().GetLevel();
        if( path != "" && IsValid( path.substr( 7, path.length() ), level ) )
            automatsLayer_.Aggregate( **it );
    }
    menu_->setItemChecked( id, true );
}

// -----------------------------------------------------------------------------
// Name: AggregateToolbar::NotifyCreated
// Created: LGY 2011-10-14
// -----------------------------------------------------------------------------
void AggregateToolbar::NotifyCreated( const kernel::Automat_ABC& automat )
{
    if( std::find( automats_.begin(), automats_.end(), &automat ) == automats_.end() )
        automats_.push_back( &automat );
}

// -----------------------------------------------------------------------------
// Name: AggregateToolbar::NotifyDeleted
// Created: LGY 2011-10-14
// -----------------------------------------------------------------------------
void AggregateToolbar::NotifyDeleted( const kernel::Automat_ABC& automat )
{
    IT_Automats it = std::find( automats_.begin(), automats_.end(), &automat );
    if( it != automats_.end() )
    {
        std::swap( *it, automats_.back() );
        automats_.pop_back();
    }
}

// -----------------------------------------------------------------------------
// Name: AggregateToolbar::NotifyCreated
// Created: LGY 2011-10-14
// -----------------------------------------------------------------------------
void AggregateToolbar::NotifyCreated( const kernel::Formation_ABC& formation )
{
    if( std::find( formations_.begin(), formations_.end(), &formation ) == formations_.end() )
        formations_.push_back( &formation );
}

// -----------------------------------------------------------------------------
// Name: AggregateToolbar::NotifyDeleted
// Created: LGY 2011-10-14
// -----------------------------------------------------------------------------
void AggregateToolbar::NotifyDeleted( const kernel::Formation_ABC& formation )
{
    IT_Formations it = std::find( formations_.begin(), formations_.end(), &formation );
    if( it != formations_.end() )
    {
        std::swap( *it, formations_.back() );
        formations_.pop_back();
    }
}
