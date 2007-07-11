// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
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
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Options.h"
#include "graphics/DragMovementLayer.h"

using namespace gui;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: GlSelector constructor
// Created: AGE 2007-03-09
// -----------------------------------------------------------------------------
GlSelector::GlSelector( QMainWindow* parent, GlProxy& proxy, Controllers& controllers, const ExerciseConfig& config, DetectionMap& map, EventStrategy_ABC& strategy )
    : QObject       ( parent )
    , parent_       ( parent )
    , proxy_        ( proxy )
    , controllers_  ( controllers )
    , config_       ( config )
    , map_          ( map )
    , strategy_     ( strategy )
    , iconLayout_   ( new IconLayout() )
    , widget2d_     ( 0 )
    , widget3d_     ( 0 )
    , glPlaceHolder_( 0 )
    , b3d_          ( false )

{
    displayTimer_ = new QTimer( this );
    glPlaceHolder_ = new GlPlaceHolder( parent );
    parent->setCentralWidget( glPlaceHolder_ );

    controllers_.options_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: GlSelector destructor
// Created: AGE 2007-03-09
// -----------------------------------------------------------------------------
GlSelector::~GlSelector()
{
    controllers_.options_.Unregister( *this );
    delete glPlaceHolder_;
    delete widget2d_;
    delete widget3d_;
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
    moveLayer_.reset( new DragMovementLayer( *widget2d_ ) );
    widget2d_->Configure( strategy_ );
    widget2d_->Configure( *moveLayer_ );
    proxy_.ChangeTo( widget2d_ );
    proxy_.RegisterTo( widget2d_ );
    delete glPlaceHolder_; glPlaceHolder_ = 0;
    parent_->setCentralWidget( widget2d_ );
    b3d_ = false;
    controllers_.options_.Change( "3D", b3d_ );
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
    delete glPlaceHolder_;
    glPlaceHolder_ = new GlPlaceHolder( parent_ );
    parent_->setCentralWidget( glPlaceHolder_ );
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
        delete widget3d_; widget3d_ = 0;
    }
    if( widget2d_ )
    {
        widget2d_->makeCurrent();
        proxy_.Reset2d();
        delete widget2d_; widget2d_ = 0;
        emit Widget2dChanged( widget2d_ );
    }
}

// -----------------------------------------------------------------------------
// Name: GlSelector::OptionChanged
// Created: AGE 2007-03-09
// -----------------------------------------------------------------------------
void GlSelector::OptionChanged( const std::string& name, const OptionVariant& value )
{
    if( name == "3D" && !glPlaceHolder_ )
    {
        bool new3d = value.To< bool >();
        if( new3d != b3d_ )
        {
            parent_->centralWidget()->hide();
            disconnect( displayTimer_, SIGNAL( timeout() ), parent_->centralWidget(), SLOT( updateGL() ) );
            if( new3d )
            {
                if( ! widget3d_ )
                {
                    widget3d_ = new Gl3dWidget( parent_, controllers_, config_, map_, strategy_ );
                    connect( widget3d_, SIGNAL( MouseMove( const geometry::Point3f& ) ), this, SIGNAL( MouseMove( const geometry::Point3f& ) ) );
                    proxy_.RegisterTo( widget3d_ );
                }
                proxy_.ChangeTo( widget3d_ );
                parent_->setCentralWidget( widget3d_ );
            }
            else
            {
                proxy_.ChangeTo( widget2d_ );
                parent_->setCentralWidget( widget2d_ );
            }
            parent_->centralWidget()->show();
            connect( displayTimer_, SIGNAL( timeout()), parent_->centralWidget(), SLOT( updateGL() ) );
            b3d_ = new3d;
        }
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
