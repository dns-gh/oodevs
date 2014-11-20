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
#include "GL2DWidget.h"
#include "GL3DWidget.h"
#include "GLOptions.h"
#include "GL2D3DProxy.h"
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
                                  const GLView_ABC::T_View& proxy,
                                  const tools::ExerciseConfig& config,
                                  DetectionMap& map,
                                  EventStrategy_ABC& strategy,
                                  const std::shared_ptr< IconLayout >& iconLayout,
                                  QGLWidget* shareWidget /* = 0 */ )
    : QStackedWidget( parent )
    , proxy_( proxy )
    , config_( config )
    , map_( map )
    , strategy_( strategy )
    , iconLayout_( iconLayout )
    , shareWidget_( shareWidget )
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
    if( widget2d_ || widget3d_ )
        throw MASA_EXCEPTION( "GLStackedWidget already loaded" );

    widget2d_ = std::make_shared< GL2DWidget >( this,
                                                *proxy_,
                                                config_.GetTerrainWidth(),
                                                config_.GetTerrainHeight(),
                                                iconLayout_,
                                                shareWidget_ );
    widget3d_ = std::make_shared< GL3DWidget >( this,
                                                *proxy_,
                                                map_,
                                                strategy_,
                                                shareWidget_ ? shareWidget_ : widget2d_.get() );

    widget2d_->Configure( strategy_ );
    auto movementLayer = std::make_shared< DragMovementLayer >( *widget2d_ );
    gui::connect( movementLayer.get(), SIGNAL( Translated() ), [&]() {
        proxy_->GetActiveOptions().SetLockedEntity( 0 );
    } );
    widget2d_->Configure( movementLayer );
    
    insertWidget( eWidget_2D, widget2d_.get() );
    insertWidget( eWidget_3D, widget3d_.get() );

    connect( widget2d_.get(), SIGNAL( MouseMove( const geometry::Point2f& ) ),
             this, SIGNAL( MouseMove( const geometry::Point2f& ) ) );
    connect( widget3d_.get(), SIGNAL( MouseMove( const geometry::Point3f& ) ),
             this, SIGNAL( MouseMove( const geometry::Point3f& ) ) );

    InitializePasses();
    ChangeTo( proxy_->GetCurrentOptions().Get( "3D" ).To< bool >() ? eWidget_3D : eWidget_2D );
}

// -----------------------------------------------------------------------------
// Name: GLStackedWidget::ChangeTo
// Created: ABR 2014-09-24
// -----------------------------------------------------------------------------
void GLStackedWidget::ChangeTo( E_Widget type )
{
    const auto currentType = static_cast< E_Widget >( currentIndex() );
    if( type == currentType )
        return;

    if( currentType == eWidget_2D )
        proxy_->Unregister( widget2d_ );
    else if( currentType == eWidget_3D )
        proxy_->Unregister( widget3d_ );

    if( type == eWidget_2D )
        proxy_->Register( widget2d_ );
    else if( type == eWidget_3D )
        proxy_->Register( widget3d_ );

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
    auto tooltip = new LayersRenderPass( "tooltip", false );
    auto main = new TextureRenderPass( "main" );
    auto fog = new TextureRenderPass( "fog", "FogOfWar" );
    auto compo = new CompositionPass( *main, *fog, "FogOfWar" );
    widget2d_->AddPass( *tooltip );
    widget2d_->AddPass( *compo );
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
// Name: GLStackedWidget::GetWidget2d
// Created: ABR 2012-07-10
// -----------------------------------------------------------------------------
std::shared_ptr< GL2DWidget > GLStackedWidget::GetWidget2d() const
{
    return widget2d_;
}

// -----------------------------------------------------------------------------
// Name: GLStackedWidget::GetProxy
// Created: ABR 2012-07-10
// -----------------------------------------------------------------------------
std::shared_ptr< GLView_ABC > GLStackedWidget::GetProxy() const
{
    return proxy_;
}

// -----------------------------------------------------------------------------
// Name: GLStackedWidget::GetCurrentWidget
// Created: ABR 2012-07-10
// -----------------------------------------------------------------------------
std::shared_ptr< QGLWidget > GLStackedWidget::GetCurrentWidget() const
{
    auto currentType = static_cast< E_Widget >( currentIndex() );
    if( currentType == eWidget_2D )
        return widget2d_;
    if( currentType == eWidget_3D )
        return widget3d_;
    return std::shared_ptr< QGLWidget >();
}
