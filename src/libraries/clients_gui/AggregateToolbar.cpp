// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "AggregateToolbar.h"
#include "moc_AggregateToolbar.cpp"
#include "AutomatsLayer.h"
#include "FormationLayer.h"
#include "resources.h"
#include "RichWidget.h"
#include "SubObjectName.h"
#include "ImageWrapper.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/HierarchyLevel_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/ContextMenu.h"
#include "tools/GeneralConfig.h"
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
AggregateToolbar::AggregateToolbar( kernel::Controller& controller,
                                    AutomatsLayer& automatsLayer, FormationLayer& formationsLayer, bool showDisplayModes )
    : QHBoxLayout()
    , controller_     ( controller )
    , automatsLayer_  ( automatsLayer )
    , formationsLayer_( formationsLayer )
    , displayMenu_( 0 )
{
    SubObjectName subObject( "AggregateToolbar" );
    RichWidget< QToolButton >* btn = new RichWidget< QToolButton >( "aggregate" );
    btn->setAutoRaise( true );
    btn->setPopupDelay( 0 );
    btn->setIconSet( MAKE_ICON( aggregate ) );
    btn->setPopupMode( RichWidget< QToolButton >::MenuButtonPopup );
    QToolTip::add( btn, tools::translate( "AggregateToolbar", "Aggregate all automats" ) );
    connect( btn, SIGNAL( clicked() ), SLOT( Aggregate() ) );
    addWidget( btn );

    levelMenu_ = new kernel::ContextMenu( btn );
    for( unsigned int i = 0u; i < LEVELS.size(); ++i )
        levelMenu_->insertItem( tools::translate( "ENT_Tr", LEVELS[ i ].c_str() ), i );
    btn->setPopup( levelMenu_ );
    connect( levelMenu_, SIGNAL( activated( int ) ), SLOT( Aggregate( int ) ) );

    btn = new RichWidget< QToolButton >( "disaggregateAll" );
    btn->setAutoRaise( true );
    btn->setIconSet( MAKE_ICON( desaggregate ) );
    QToolTip::add( btn, tools::translate( "AggregateToolbar", "Disaggregate all" ) );
    connect( btn, SIGNAL( clicked() ), SLOT( DisaggregateAll() ) );
    addWidget( btn );

    QIcon dndIcon;
    dndIcon.addPixmap( MAKE_PIXMAP( dnd_lock ), QIcon::Normal, QIcon::On );
    dndIcon.addPixmap( MAKE_PIXMAP( dnd_unlock ), QIcon::Normal, QIcon::Off );
    btn = new RichWidget< QToolButton >( "lockUnlockDragDrop" );
    btn->setIcon( dndIcon );
    btn->setToggleButton( true );
    btn->setAutoRaise( true );
    QToolTip::add( btn, tools::translate( "AggregateToolbar", "Lock/Unlock drag-and-drop" ) );
    connect( btn, SIGNAL( toggled( bool ) ), SLOT( OnLockDragAndDropToggled( bool ) ) );
    addWidget( btn );

    if( showDisplayModes)
    {
        btn = new RichWidget< QToolButton >( "changeOrbatDisplayMode" );
        btn->setIcon( gui::Icon( tools::GeneralConfig::BuildResourceChildFile( "images/gaming/eye.png" ) ) );
        btn->setAutoRaise( true );
        btn->setPopupMode( RichWidget< QToolButton >::InstantPopup );
        QToolTip::add( btn, tools::translate( "AggregateToolbar", "Change Orbat display mode" ) );
        addWidget( btn );

        displayMenu_ = new kernel::ContextMenu( btn );
        displayMenu_->insertItem( tools::translate( "AggregateToolbar", "Observable units (Default)" ), 0 );
        displayMenu_->insertItem( tools::translate( "AggregateToolbar", "Controlled units" ), 1 );
        displayMenu_->insertItem( tools::translate( "AggregateToolbar", "Sides" ), 2 );
        displayMenu_->setItemChecked( 0, true );
        btn->setPopup( displayMenu_ );
        connect( displayMenu_, SIGNAL( activated( int ) ), SLOT( OnChangeDisplay( int ) ) );
    }

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
    for( auto it = automats_.begin(); it != automats_.end(); ++it )
        automatsLayer_.Aggregate( **it );
}

// -----------------------------------------------------------------------------
// Name: AggregateToolbar::DisaggregateAll
// Created: LGY 2011-10-14
// -----------------------------------------------------------------------------
void AggregateToolbar::DisaggregateAll()
{
    for( unsigned int i = 0u; i < LEVELS.size(); ++i )
        levelMenu_->setItemChecked( i, false );
    for( auto it = formations_.begin(); it != formations_.end(); ++it )
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
        if( IsValid( ENT_Tr::ConvertFromNatureLevel( (*it)->GetLevel() ), level ) )
            formationsLayer_.Aggregate( **it );
    for( IT_Automats it = automats_.begin(); it != automats_.end(); ++it )
    {
        const std::string path = (*it)->Get< kernel::TacticalHierarchies >().GetLevel();
        if( path != "" && IsValid( path.substr( 7, path.length() ), level ) )
            automatsLayer_.Aggregate( **it );
    }
    levelMenu_->setItemChecked( id, true );
}

// -----------------------------------------------------------------------------
// Name: AggregateToolbar::OnLockDragAndDropToggled
// Created: JSR 2012-06-28
// -----------------------------------------------------------------------------
void AggregateToolbar::OnLockDragAndDropToggled( bool toggled )
{
    emit LockDragAndDrop( toggled );
}

// -----------------------------------------------------------------------------
// Name: AggregateToolbar::OnChangeDisplay
// Created: JSR 2013-01-18
// -----------------------------------------------------------------------------
void AggregateToolbar::OnChangeDisplay( int id )
{
    emit ChangeDisplay( id );
    displayMenu_->setItemChecked( 0, 0 == id );
    displayMenu_->setItemChecked( 1, 1 == id );
    displayMenu_->setItemChecked( 2, 2 == id );
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
