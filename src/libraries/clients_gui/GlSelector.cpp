// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "GlSelector.h"
#include "moc_GlSelector.cpp"

#include "CompositionPass.h"
#include "GlPlaceHolder.h"
#include "GlWidget.h"
#include "Gl3dWidget.h"
#include "GLOptions.h"
#include "GlProxy.h"
#include "IconLayout.h"
#include "Layer.h"
#include "LayersRenderPass.h"
#include "SignalAdapter.h"
#include "TextureRenderPass.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/OptionsController.h"
#include "clients_kernel/OptionVariant.h"
#include "tools/ExerciseConfig.h"
#include <graphics/DragMovementLayer.h>

using namespace gui;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: GlSelector constructor
// Created: AGE 2007-03-09
// -----------------------------------------------------------------------------
GlSelector::GlSelector( QWidget* parent,
                        GlProxy& proxy,
                        Controllers& controllers,
                        const tools::ExerciseConfig& config,
                        DetectionMap& map,
                        EventStrategy_ABC& strategy,
                        const DrawingTypes& drawingTypes )
    : QStackedWidget( parent )
    , proxy_       ( proxy )
    , controllers_ ( controllers )
    , config_      ( config )
    , map_         ( map )
    , strategy_    ( strategy )
    , drawingTypes_( drawingTypes )
    , iconLayout_  ( new IconLayout() )
    , displayTimer_( new QTimer( this ) )
{
    setObjectName( "GlSelector" );
    insertWidget( eWidget_Empty, new GlPlaceHolder( this ) );
    ChangeTo( eWidget_Empty );
    controllers_.options_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: GlSelector destructor
// Created: AGE 2007-03-09
// -----------------------------------------------------------------------------
GlSelector::~GlSelector()
{
    emit Widget3dChanged( 0 );
    emit Widget2dChanged( 0 );
    controllers_.options_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: GlSelector::Load
// Created: AGE 2007-03-09
// -----------------------------------------------------------------------------
void GlSelector::Load()
{
    if( widget2d_ && widget3d_ )
    {
        widget2d_->Load( config_ );
        widget3d_->Load( config_ );
        return;
    }

    widget2d_ = std::make_shared< GlWidget >( this, proxy_, config_.GetTerrainWidth(),config_.GetTerrainHeight(), *iconLayout_, drawingTypes_ );
    widget3d_ = std::make_shared< Gl3dWidget >(this, proxy_, config_.GetTerrainWidth(), config_.GetTerrainHeight(), map_, strategy_, drawingTypes_, widget2d_.get() );

    widget2d_->Load( config_ );
    widget2d_->Configure( strategy_ );
    auto movementLayer = std::make_shared< DragMovementLayer >( *widget2d_ );
    gui::connect( movementLayer.get(), SIGNAL( Translated() ), [&]() {
        proxy_.GetOptions().SetLockedEntity( 0 );
    } );
    widget2d_->Configure( movementLayer );
    widget3d_->Load( config_ );

    proxy_.SetWidget2D( widget2d_ );
    proxy_.SetWidget3D( widget3d_ );

    insertWidget( eWidget_2D, widget2d_.get() );
    insertWidget( eWidget_3D, widget3d_.get() );

    connect( widget2d_.get(), SIGNAL( MouseMove( const geometry::Point2f& ) ), this, SIGNAL( MouseMove( const geometry::Point2f& ) ) );
    connect( widget3d_.get(), SIGNAL( MouseMove( const geometry::Point3f& ) ), this, SIGNAL( MouseMove( const geometry::Point3f& ) ) );
    connect( displayTimer_, SIGNAL( timeout() ), this, SIGNAL( UpdateGL() ) );

    InitializePasses();
    displayTimer_->start( controllers_.options_.GetOption( "RefreshRate" ).To< int >() );
    ChangeTo( eWidget_2D );
}

// -----------------------------------------------------------------------------
// Name: GlSelector::ChangeTo
// Created: ABR 2014-09-24
// -----------------------------------------------------------------------------
void GlSelector::ChangeTo( E_Widget type )
{
    auto currentType = static_cast< E_Widget >( currentIndex() );
    if( type == currentType )
        return;

    if( currentType == eWidget_2D )
        disconnect( displayTimer_, SIGNAL( timeout() ), widget2d_.get(), SLOT( updateGL() ) );
    else if( currentType == eWidget_3D )
        disconnect( displayTimer_, SIGNAL( timeout() ), widget3d_.get(), SLOT( updateGL() ) );

    if( type == eWidget_2D )
    {
        proxy_.ChangeTo( widget2d_ );
        emit Widget2dChanged( widget2d_.get() );
        emit Widget3dChanged( 0 );
        connect( displayTimer_, SIGNAL( timeout() ), widget2d_.get(), SLOT( updateGL() ) );
    }
    else if( type == eWidget_3D )
    {
        proxy_.ChangeTo( widget3d_ );
        emit Widget2dChanged( 0 );
        emit Widget3dChanged( widget3d_.get() );
        connect( displayTimer_, SIGNAL( timeout() ), widget3d_.get(), SLOT( updateGL() ) );
    }

    setCurrentIndex( type );
}

// -----------------------------------------------------------------------------
// Name: GlSelector::Close
// Created: AGE 2007-03-09
// -----------------------------------------------------------------------------
void GlSelector::Close()
{
    ChangeTo( eWidget_Empty );
    removeWidget( widget2d_.get() );
    removeWidget( widget3d_.get() );
    widget2d_.reset();
    widget3d_.reset();
}

// -----------------------------------------------------------------------------
// Name: GlSelector::OptionChanged
// Created: AGE 2007-03-09
// -----------------------------------------------------------------------------
void GlSelector::OptionChanged( const std::string& name, const OptionVariant& value )
{
    if( name == "3D" && widget2d_ && widget3d_ )
        ChangeTo( value.To< bool >() ? eWidget_3D : eWidget_2D );
    else if( name == "RefreshRate" )
        displayTimer_->start( value.To< int >() );
}

// -----------------------------------------------------------------------------
// Name: GlSelector::AddIcon
// Created: AGE 2007-03-09
// -----------------------------------------------------------------------------
void GlSelector::AddIcon( const char** xpm, int x, int y )
{
    iconLayout_->AddIcon( xpm, x, y );
}

// -----------------------------------------------------------------------------
// Name: GlSelector::InitializePasses
// Created: SBO 2008-04-15
// -----------------------------------------------------------------------------
void GlSelector::InitializePasses()
{
    if( !widget2d_ )
        return;
    widget2d_->SetPassOrder( "icons,miniviews,main,fog,composition,tooltip" );
    LayersRenderPass*  tooltip     = new LayersRenderPass ( *widget2d_, "tooltip", false );
    TextureRenderPass* main        = new TextureRenderPass( *widget2d_, "main", controllers_ );
    TextureRenderPass* fog         = new TextureRenderPass( *widget2d_, "fog", controllers_, "FogOfWar" );
    GlRenderPass_ABC*  composition = new CompositionPass  ( *main, *fog, controllers_, "FogOfWar" );
    widget2d_->AddPass( *tooltip );
    widget2d_->AddPass( *composition );
    widget2d_->AddPass( *fog );
    widget2d_->AddPass( *main );
}

// -----------------------------------------------------------------------------
// Name: GlSelector::SetFocus
// Created: ABR 2012-07-10
// -----------------------------------------------------------------------------
void GlSelector::SetFocus()
{
    if( widget2d_ )
        widget2d_->setFocus();
    else if( widget3d_ )
        widget3d_->setFocus();
}
