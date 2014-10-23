// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "GLStackedWidget.h"
#include "moc_GLStackedWidget.cpp"

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
// Name: GLStackedWidget constructor
// Created: AGE 2007-03-09
// -----------------------------------------------------------------------------
GLStackedWidget::GLStackedWidget( QWidget* parent,
                                  GlProxy& proxy,
                                  const tools::ExerciseConfig& config,
                                  DetectionMap& map,
                                  EventStrategy_ABC& strategy,
                                  const DrawingTypes& drawingTypes,
                                  const IconLayout& iconLayout )
    : QStackedWidget( parent )
    , proxy_       ( proxy )
    , config_      ( config )
    , map_         ( map )
    , strategy_    ( strategy )
    , drawingTypes_( drawingTypes )
    , iconLayout_  ( iconLayout )
{
    setObjectName( "GLStackedWidget" );
    insertWidget( eWidget_Empty, new GlPlaceHolder( this ) );
    ChangeTo( eWidget_Empty );
}

// -----------------------------------------------------------------------------
// Name: GLStackedWidget destructor
// Created: AGE 2007-03-09
// -----------------------------------------------------------------------------
GLStackedWidget::~GLStackedWidget()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GLStackedWidget::Load
// Created: AGE 2007-03-09
// -----------------------------------------------------------------------------
void GLStackedWidget::Load()
{
    if( widget2d_ && widget3d_ )
    {
        widget2d_->Load( config_ );
        widget3d_->Load( config_ );
        return;
    }

    widget2d_ = std::make_shared< GlWidget >( this, proxy_, config_.GetTerrainWidth(),config_.GetTerrainHeight(), iconLayout_, drawingTypes_ );
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

    InitializePasses();
    ChangeTo( controllers_.options_.GetOption( "3D" ).To< bool >() ? eWidget_3D : eWidget_2D );
}

// -----------------------------------------------------------------------------
// Name: GLStackedWidget::ChangeTo
// Created: ABR 2014-09-24
// -----------------------------------------------------------------------------
void GLStackedWidget::ChangeTo( E_Widget type )
{
    auto currentType = static_cast< E_Widget >( currentIndex() );
    if( type == currentType )
        return;

    if( type == eWidget_2D )
        proxy_.ChangeTo( widget2d_ );
    else if( type == eWidget_3D )
        proxy_.ChangeTo( widget3d_ );

    setCurrentIndex( type );
}

// -----------------------------------------------------------------------------
// Name: GLStackedWidget::Close
// Created: AGE 2007-03-09
// -----------------------------------------------------------------------------
void GLStackedWidget::Close()
{
    ChangeTo( eWidget_Empty );
    removeWidget( widget2d_.get() );
    removeWidget( widget3d_.get() );
    widget2d_.reset();
    widget3d_.reset();
}

// -----------------------------------------------------------------------------
// Name: GLStackedWidget::InitializePasses
// Created: SBO 2008-04-15
// -----------------------------------------------------------------------------
void GLStackedWidget::InitializePasses()
{
    if( !widget2d_ )
        return;
    widget2d_->SetPassOrder( "main,fog,composition,tooltip" );
    LayersRenderPass*  tooltip     = new LayersRenderPass ( *widget2d_, "tooltip", false );
    TextureRenderPass* main        = new TextureRenderPass( *widget2d_, "main", proxy_ );
    TextureRenderPass* fog         = new TextureRenderPass( *widget2d_, "fog", proxy_, "FogOfWar" );
    GlRenderPass_ABC*  composition = new CompositionPass  ( *main, *fog, proxy_, "FogOfWar" );
    widget2d_->AddPass( *tooltip );
    widget2d_->AddPass( *composition );
    widget2d_->AddPass( *fog );
    widget2d_->AddPass( *main );
}

// -----------------------------------------------------------------------------
// Name: GLStackedWidget::SetFocus
// Created: ABR 2012-07-10
// -----------------------------------------------------------------------------
void GLStackedWidget::SetFocus()
{
    if( widget2d_ )
        widget2d_->setFocus();
    else if( widget3d_ )
        widget3d_->setFocus();
}

// -----------------------------------------------------------------------------
// Name: GLStackedWidget::GetView
// Created: ABR 2012-07-10
// -----------------------------------------------------------------------------
std::shared_ptr< GlWidget > GLStackedWidget::GetWidget2d()
{
    return widget2d_;
}
