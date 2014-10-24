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
#include "GLOptions.h"
#include "GLView_ABC.h"
#include "resources.h"
#include "RichWidget.h"
#include "SubObjectName.h"
#include "ImageWrapper.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Formation_ABC.h"
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
    bool IsLess( const std::string& level, const std::string& maxLevel )
    {
        return std::distance( LEVELS.begin(), std::find( LEVELS.begin(), LEVELS.end(), level ) ) <=
               std::distance( LEVELS.begin(), std::find( LEVELS.begin(), LEVELS.end(), maxLevel ) );
    }
}

// -----------------------------------------------------------------------------
// Name: AggregateToolbar constructor
// Created: LGY 2011-10-14
// -----------------------------------------------------------------------------
AggregateToolbar::AggregateToolbar( GLView_ABC& view,
                                    const tools::Resolver< kernel::Formation_ABC >& formations,
                                    const tools::Resolver< kernel::Automat_ABC >& automats,
                                    bool showDisplayModes )
    : QHBoxLayout()
    , view_( view )
    , automats_( automats )
    , formations_( formations )
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
}

// -----------------------------------------------------------------------------
// Name: AggregateToolbar destructor
// Created: LGY 2011-10-14
// -----------------------------------------------------------------------------
AggregateToolbar::~AggregateToolbar()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AggregateToolbar::Aggregate
// Created: LGY 2011-10-14
// -----------------------------------------------------------------------------
void AggregateToolbar::Aggregate()
{
    auto& options = view_.GetOptions();
    automats_.Apply( [&]( const kernel::Automat_ABC& automat ) {
        options.Aggregate( automat );
    } );
}

// -----------------------------------------------------------------------------
// Name: AggregateToolbar::DisaggregateAll
// Created: LGY 2011-10-14
// -----------------------------------------------------------------------------
void AggregateToolbar::DisaggregateAll()
{
    view_.GetOptions().Disaggregate();
    for( unsigned int i = 0u; i < LEVELS.size(); ++i )
        levelMenu_->setItemChecked( i, false );
}


// -----------------------------------------------------------------------------
// Name: AggregateToolbar::Aggregate
// Created: LGY 2011-10-14
// -----------------------------------------------------------------------------
void AggregateToolbar::Aggregate( int id )
{
    const auto& level = LEVELS[ id ];
    levelMenu_->setItemChecked( id, true );
    auto& options = view_.GetOptions();
    formations_.Apply( [&]( const kernel::Formation_ABC& formation ) {
        if( IsLess( ENT_Tr::ConvertFromNatureLevel( formation.GetLevel() ), level ) )
            options.Aggregate( formation );
    } );
    automats_.Apply( [&]( const kernel::Automat_ABC& automat ) {
        const auto automatLevel = automat.Get< kernel::TacticalHierarchies >().GetLevel();
        if( automatLevel.size() >= 7 && IsLess( automatLevel.substr( 7, automatLevel.length() ), level ) )
            options.Aggregate( automat );
    } );
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
