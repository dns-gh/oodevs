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
#include "GLMainProxy.h"
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
#include "clients_kernel/Profile_ABC.h"
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
    bool IsLess( const std::string& lhs, const std::string& rhs )
    {
        return std::find( LEVELS.begin(), LEVELS.end(), lhs ) <= std::find( LEVELS.begin(), LEVELS.end(), rhs );
    }
}

// -----------------------------------------------------------------------------
// Name: AggregateToolbar constructor
// Created: LGY 2011-10-14
// -----------------------------------------------------------------------------
AggregateToolbar::AggregateToolbar( kernel::Controllers& controllers,
                                    GLMainProxy& mainProxy,
                                    const kernel::Profile_ABC& profile,
                                    const tools::Resolver< kernel::Formation_ABC >& formations,
                                    const tools::Resolver< kernel::Automat_ABC >& automats,
                                    bool showDisplayModes )
    : QHBoxLayout()
    , controllers_( controllers )
    , view_( mainProxy )
    , profile_( profile )
    , automats_( automats )
    , formations_( formations )
    , displayMenu_( 0 )
    , selected_( controllers )
{
    SubObjectName subObject( "AggregateToolbar" );
    RichWidget< QToolButton >* btn = new RichWidget< QToolButton >( "aggregate" );
    btn->setAutoRaise( true );
    btn->setPopupDelay( 0 );
    btn->setIconSet( MAKE_ICON( aggregate ) );
    btn->setPopupMode( RichWidget< QToolButton >::MenuButtonPopup );
    QToolTip::add( btn, tools::translate( "AggregateToolbar", "Aggregate all automats" ) );
    connect( btn, SIGNAL( clicked() ), SLOT( AggregateAllAutomat() ) );
    addWidget( btn );

    levelMenu_ = new RichWidget< QMenu >( "aggregationLevelMenu", btn );
    for( unsigned int i = 0u; i < LEVELS.size(); ++i )
        if( auto action = levelMenu_->addAction( tools::translate( "ENT_Tr", LEVELS[ i ].c_str() ) ) )
        {
            action->setData( QString::fromStdString( LEVELS[ i ] ) );
            action->setCheckable( true );
            action->setChecked( false);
        }
    btn->setPopup( levelMenu_ );
    connect( levelMenu_, SIGNAL( triggered( QAction* ) ), SLOT( ToggleAggregationLevel( QAction* ) ) );

    btn = new RichWidget< QToolButton >( "disaggregateAll" );
    btn->setAutoRaise( true );
    btn->setIconSet( MAKE_ICON( desaggregate ) );
    QToolTip::add( btn, tools::translate( "AggregateToolbar", "Disaggregate all" ) );
    connect( btn, SIGNAL( clicked() ), SLOT( DisaggregateAll() ) );
    connect( btn, SIGNAL( clicked() ), SLOT( UpdateLevelMenu() ) );
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

        displayMenu_ = new RichWidget< QMenu >( "displayMenu", btn );
        displayMenu_->insertItem( tools::translate( "AggregateToolbar", "Observable units (Default)" ), 0 );
        displayMenu_->insertItem( tools::translate( "AggregateToolbar", "Controlled units" ), 1 );
        displayMenu_->insertItem( tools::translate( "AggregateToolbar", "Sides" ), 2 );
        displayMenu_->setItemChecked( 0, true );
        btn->setPopup( displayMenu_ );
        connect( displayMenu_, SIGNAL( activated( int ) ), SLOT( OnChangeDisplay( int ) ) );
    }

    mainProxy.AddActiveChangeObserver( this, [&]( const GLView_ABC::T_View& ) { UpdateLevelMenu(); } );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: AggregateToolbar destructor
// Created: LGY 2011-10-14
// -----------------------------------------------------------------------------
AggregateToolbar::~AggregateToolbar()
{
    controllers_.Unregister( *this );
}

void AggregateToolbar::UpdateLevelMenu()
{
    const auto& options = view_.GetActiveOptions();
    const auto& aggregatedEntities = options.GetAggregatedEntities();
    bool done = false;
    auto actions = levelMenu_->actions();
    for( int i = actions.size() - 1; i >= 0; --i )
        if( auto action = actions[ i ] )
        {
            bool levelOn = false;
            if( !done )
            {
                bool hasChanged = false;
                levelOn = true;
                const auto level = action->data().toString().toStdString();
                automats_.Apply( [ &]( const kernel::Automat_ABC& automat ) {
                    const auto automatLevel = automat.Get< kernel::TacticalHierarchies >().GetLevel();
                    if( automatLevel.size() >= 7 &&
                        IsLess( automatLevel.substr( 7, automatLevel.length() ), level ) )
                    {
                        levelOn &= std::find( aggregatedEntities.begin(), aggregatedEntities.end(), &automat ) != aggregatedEntities.end();
                        hasChanged = true;
                    }
                } );
                formations_.Apply( [ &]( const kernel::Formation_ABC& formation ) {
                    if( IsLess( ENT_Tr::ConvertFromNatureLevel( formation.GetLevel() ), level ) )
                    {
                        levelOn &= std::find( aggregatedEntities.begin(), aggregatedEntities.end(), &formation ) != aggregatedEntities.end();
                        hasChanged = true;
                    }
                } );
                if( !hasChanged )
                    levelOn = false;
            }
            action->setChecked( !done && levelOn );
            if( levelOn )
                done = true;
        }
}

void AggregateToolbar::AggregateAllAutomat()
{
    auto& options = view_.GetActiveOptions();
    options.Disaggregate();
    automats_.Apply( [&]( const kernel::Automat_ABC& automat ) {
        options.Aggregate( automat );
    } );
    UpdateLevelMenu();
}

void AggregateToolbar::DisaggregateAll()
{
    view_.GetActiveOptions().Disaggregate();
    UpdateLevelMenu();
}

void AggregateToolbar::ToggleAggregationLevel( QAction* action )
{
    if( !action )
        return;
    const auto level = action->data().toString().toStdString();
    if( action->isChecked() )
        AggregateLevel( level );
    else
        DisaggregateLevel( level );
    UpdateLevelMenu();
}

void AggregateToolbar::AggregateLevel( const std::string& level )
{
    auto& options = view_.GetActiveOptions();
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

void AggregateToolbar::DisaggregateLevel( const std::string& level )
{
    auto& options = view_.GetActiveOptions();
    formations_.Apply( [&]( const kernel::Formation_ABC& formation ) {
        if( IsLess( ENT_Tr::ConvertFromNatureLevel( formation.GetLevel() ), level ) )
            options.Disaggregate( &formation );
    } );
    automats_.Apply( [&]( const kernel::Automat_ABC& automat ) {
        const auto automatLevel = automat.Get< kernel::TacticalHierarchies >().GetLevel();
        if( automatLevel.size() >= 7 && IsLess( automatLevel.substr( 7, automatLevel.length() ), level ) )
            options.Disaggregate( &automat );
    } );
}

void AggregateToolbar::OnLockDragAndDropToggled( bool toggled )
{
    emit LockDragAndDrop( toggled );
}

void AggregateToolbar::OnChangeDisplay( int id )
{
    emit ChangeDisplay( id );
    displayMenu_->setItemChecked( 0, 0 == id );
    displayMenu_->setItemChecked( 1, 1 == id );
    displayMenu_->setItemChecked( 2, 2 == id );
}

void AggregateToolbar::Aggregate()
{
    if( selected_ )
    {
        view_.GetActiveOptions().Aggregate( *selected_ );
        UpdateLevelMenu();
    }
}

void AggregateToolbar::Disaggregate()
{
    if( selected_ )
    {
        view_.GetActiveOptions().Disaggregate( selected_ );
        UpdateLevelMenu();
    }
}

void AggregateToolbar::NotifyContextMenu( const kernel::Formation_ABC& formation, kernel::ContextMenu& menu )
{
    OnContextMenu( formation, menu );
}

void AggregateToolbar::NotifyContextMenu( const kernel::Automat_ABC& automat, kernel::ContextMenu& menu )
{
    OnContextMenu( automat, menu );
}

void AggregateToolbar::OnContextMenu( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu )
{
    if( !profile_.IsVisible( entity ) )
        return;
    selected_ = &entity;
    if( !entity.IsAnAggregatedSubordinate() )
    {
        if( !view_.GetActiveOptions().IsAggregated( entity ) )
            menu.InsertItem( "Interface", tr( "Aggregate" ), this, SLOT( Aggregate() ) );
        else
            menu.InsertItem( "Interface", tr( "Disaggregate" ), this, SLOT( Disaggregate() ) );
    }
}
