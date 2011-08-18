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
#include "GlPlaceHolder.h"
#include "GlWidget.h"
#include "Gl3dWidget.h"
#include "GlProxy.h"
#include "IconLayout.h"
#include "LayersRenderPass.h"
#include "TextureRenderPass.h"
#include "CompositionPass.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Options.h"
#include <graphics/DragMovementLayer.h>

using namespace gui;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: GlSelector constructor
// Created: AGE 2007-03-09
// -----------------------------------------------------------------------------
GlSelector::GlSelector( QStackedWidget* parent, GlProxy& proxy, Controllers& controllers, const tools::ExerciseConfig& config, DetectionMap& map, EventStrategy_ABC& strategy )
    : QObject( parent )
    , parent_           ( parent )
    , proxy_            ( proxy )
    , controllers_      ( controllers )
    , config_           ( config )
    , map_              ( map )
    , strategy_         ( strategy )
    , iconLayout_       ( new IconLayout() )
    , widget2d_         ( 0 )
    , widget3d_         ( 0 )
    , glPlaceHolder_    ( 0 )
    , b3d_              ( false )
    , bDragMapWithWheel_( false )
{
    setObjectName( "GlSelector" );
    displayTimer_ = new QTimer( this );
    glPlaceHolder_ = new GlPlaceHolder( parent );
    parent->addWidget( glPlaceHolder_ );
    parent->setCurrentWidget( glPlaceHolder_ );

    controllers_.options_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: GlSelector destructor
// Created: AGE 2007-03-09
// -----------------------------------------------------------------------------
GlSelector::~GlSelector()
{
    emit Widget2dChanged( 0 );
    controllers_.options_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: GlSelector::Load
// Created: AGE 2007-03-09
// -----------------------------------------------------------------------------
void GlSelector::Load()
{
    if( widget2d_ )
        return;
    widget2d_ = new GlWidget( parent_, controllers_, config_, *iconLayout_ );
    parent_->addWidget( widget2d_ );
    InitializePasses();
    moveLayer_.reset( new DragMovementLayer( *widget2d_ ) );
    widget2d_->Configure( strategy_ );
    widget2d_->Configure( *moveLayer_ );
    proxy_.ChangeTo( widget2d_ );
    proxy_.RegisterTo( widget2d_ );
    parent_->setCurrentWidget( widget2d_ );
    b3d_ = false;
    controllers_.options_.Change( "3D", b3d_ );
    controllers_.options_.Change( "MapDraggingType", static_cast <int> ( !bDragMapWithWheel_ ));
    connect( displayTimer_, SIGNAL(timeout()), widget2d_, SLOT(updateGL()) );
    displayTimer_->start( 50 );
    connect( widget2d_, SIGNAL( MouseMove( const geometry::Point2f& ) ), this, SIGNAL( MouseMove( const geometry::Point2f& ) ) );
    widget2d_->show();
    emit Widget2dChanged( widget2d_ );
}

// -----------------------------------------------------------------------------
// Name: GlSelector::Close
// Created: AGE 2007-03-09
// -----------------------------------------------------------------------------
void GlSelector::Close()
{
    parent_->setCurrentWidget( glPlaceHolder_ );
    glPlaceHolder_->show();
    Clean();
}

// -----------------------------------------------------------------------------
// Name: GlSelector::Clean
// Created: AGE 2007-03-09
// -----------------------------------------------------------------------------
void GlSelector::Clean()
{
    if( widget3d_ )
    {
        widget3d_->makeCurrent();
        proxy_.Reset3d();
        delete widget3d_;
        widget3d_ = 0;
    }
    if( widget2d_ )
    {
        widget2d_->makeCurrent();
        proxy_.Reset2d();
        emit Widget2dChanged( 0 );
        delete widget2d_;
        widget2d_ = 0;
    }
}

// -----------------------------------------------------------------------------
// Name: GlSelector::OptionChanged
// Created: AGE 2007-03-09
// -----------------------------------------------------------------------------
void GlSelector::OptionChanged( const std::string& name, const OptionVariant& value )
{
    if( name == "3D" && glPlaceHolder_ != parent_->currentWidget() )
    {
        bool new3d = value.To< bool >();
        if( new3d != b3d_ )
        {
            disconnect( displayTimer_, SIGNAL( timeout() ), parent_->currentWidget(), SLOT( updateGL() ) );
            if( new3d )
            {
                if( ! widget3d_ )
                {
                    widget3d_ = new Gl3dWidget( parent_, controllers_, config_, map_, strategy_ );
                    parent_->addWidget( widget3d_ );
                    connect( widget3d_, SIGNAL( MouseMove( const geometry::Point3f& ) ), this, SIGNAL( MouseMove( const geometry::Point3f& ) ) );
                    proxy_.RegisterTo( widget3d_ );
                }
                proxy_.ChangeTo( widget3d_ );
                parent_->setCurrentWidget( widget3d_ );
            }
            else
            {
                proxy_.ChangeTo( widget2d_ );
                parent_->setCurrentWidget( widget2d_ );
            }
            connect( displayTimer_, SIGNAL( timeout()), parent_->currentWidget(), SLOT( updateGL() ) );
            b3d_ = new3d;
        }
    }
    else if( name == "MapDraggingType" )
    {
        bDragMapWithWheel_ = value.To< int >() == 0 ? false : true;
    }
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
