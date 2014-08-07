// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "ContourLinesLayer.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/DetectionMap.h"
#include "tools/ContourComputer.h"
#include <boost/bind.hpp>
#include <boost/smart_ptr/make_shared.hpp>
#include <deque>

using namespace kernel;
using namespace gui;

// $$$$ JSR 2012-02-08: hack parce qu'on ne peux pas tuer le process
bool ContourLinesLayer::valid_ =  true;

// -----------------------------------------------------------------------------
// Name: ContourLinesLayer constructor
// Created: SBO 2010-03-23
// -----------------------------------------------------------------------------
ContourLinesLayer::ContourLinesLayer( Controllers& controllers, DetectionMap& map )
    : controllers_  ( controllers )
    , map_          ( map )
    , modelLoaded_  ( false )
    , enabled_      ( false )
    , color_        ( 245, 245, 220 )
    , callListId_   ( 0 )
    , linesHeight_  ( 100 )
    , stopThread_   ( false )
    , threadRunning_( false )
    , computed_     ( false )
    , glMaxVerticesSize_( 1000 )
{
    controllers_.Register( *this );
    thread_.reset( new tools::thread::ThreadPool( 1 ) );
    glGetIntegerv( GL_MAX_TEXTURE_SIZE, &glMaxVerticesSize_ );
}

// -----------------------------------------------------------------------------
// Name: ContourLinesLayer destructor
// Created: SBO 2010-03-23
// -----------------------------------------------------------------------------
ContourLinesLayer::~ContourLinesLayer()
{
    valid_ =  false;
    stopThread_ = true;
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ContourLinesLayer::NotifyUpdated
// Created: SBO 2006-05-24
// -----------------------------------------------------------------------------
void ContourLinesLayer::NotifyUpdated( const ModelLoaded& /*modelLoaded*/ )
{
    modelLoaded_ = true;
}

// -----------------------------------------------------------------------------
// Name: ContourLinesLayer::SetAlpha
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
void ContourLinesLayer::SetAlpha( float alpha )
{
    Layer2D::SetAlpha( alpha );
}

// -----------------------------------------------------------------------------
// Name: ContourLinesLayer::OptionChanged
// Created: SBO 2010-03-23
// -----------------------------------------------------------------------------
void ContourLinesLayer::OptionChanged( const std::string& name, const OptionVariant& value )
{
    if( name == "ContourLinesColor" )
        color_.setNamedColor( value.To< QString >() );
    else if( name == "ContourLinesEnabled" )
        enabled_ = value.To< bool >();
    else if( name == "ContourLinesHeight" )
    {
        int h = value.To< int >();
        if( h > 0 && h != linesHeight_ )
        {
            linesHeight_ = h;
            bool modelLoaded = modelLoaded_;
            Reset();
            modelLoaded_ = modelLoaded;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ContourLinesLayer::Paint
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void ContourLinesLayer::Paint( const geometry::Rectangle2f& /*viewport*/ )
{
    if( !ShouldDrawPass() || GetAlpha() == 0 || !enabled_  )
        return;
    if( !modelLoaded_ )
        return;
    if( callListId_ == 0 )
    {
        CreateCallList();
        return;
    }

    glPushAttrib( GL_LINE_BIT | GL_CURRENT_BIT | GL_STENCIL_BUFFER_BIT );

    glColor4f( color_.red() / 256.f, color_.green() / 256.f, color_.blue() / 256.f, GetAlpha() );
    glEnable( GL_LINE_SMOOTH );
    glCallList( callListId_ );

    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: ContourLinesLayer::Reset
// Created: AGE 2007-01-19
// -----------------------------------------------------------------------------
void ContourLinesLayer::Reset()
{
    boost::mutex::scoped_lock locker( mutex_ );
    modelLoaded_ = false;
    computed_ = false;
    if( threadRunning_ )
        stopThread_ = true;
    if( callListId_ )
    {
        glDeleteLists( callListId_, 1 );
        callListId_ = 0;
    }
}

// -----------------------------------------------------------------------------
// Name: ContourLinesLayer::Conrec
// Created: JSR 2012-01-19
// -----------------------------------------------------------------------------
void ContourLinesLayer::Conrec()
{
    const ElevationMap& elevation = map_.GetMap();
    const short* data  = elevation.Data();
    const unsigned int width = elevation.Width();
    const unsigned int height = elevation.Height();
    const float cellSize = static_cast< float >( elevation.GetCellSize() );

    const int nc = elevation.MaximumElevation() / linesHeight_;

    const auto dataFunc = [&]( int x, int y )
    {
        return  data[ ( y )  * width + ( x ) ];
    };
    const auto progressFunc = [&]( short p )
    {
        observer_.SetPercentage( p );
        controllers_.controller_.Update( observer_ );
    };
    const auto validFunc = [&]{ return valid_; };
    const auto loopFunc = [&]( boost::shared_ptr< T_PointVector > v, int k, bool loop )
    {
        const bool large = ( k +1 ) % 5 == 0;
        if( loop )
            loops_[ large ? 0 : 2 ].push_back( boost::shared_ptr< T_PointVector >( v ) );
        else
            loops_[ large ? 1 : 3 ].push_back( boost::shared_ptr< T_PointVector >( v ) );
    };
    const auto checkStopFunc = [&]() -> bool
    {
        const bool retval = stopThread_;
        if( stopThread_ )
        {
            boost::mutex::scoped_lock locker( mutex_ );
            loops_[ 0 ].clear();
            loops_[ 1 ].clear();
            loops_[ 2 ].clear();
            loops_[ 3 ].clear();
            stopThread_ = false;
            threadRunning_ = false;
            observer_.SetPercentage( 0 );
            controllers_.controller_.Update( observer_ );
        }
        return retval;
    };
    if( !tools::ComputeContour( width, height, linesHeight_, cellSize,
            dataFunc, progressFunc, validFunc, loopFunc, checkStopFunc, nc ) )
        return;

    observer_.SetPercentage( 0 );
    controllers_.controller_.Update( observer_ );
    boost::mutex::scoped_lock locker( mutex_ );
    computed_ = true;
    threadRunning_ = false;
}

namespace
{
    void CreateGLArrays( GLenum mode, const std::vector< boost::shared_ptr< T_PointVector > >& contours, unsigned int maxSize )
    {
        const std::size_t size = contours.size();
        for( register std::size_t i = 0; i < size; ++i )
        {
            const T_PointVector* l = contours[ i ].get();
            unsigned int incr = 0;
            while( incr* maxSize < l->size() )
            {
                glVertexPointer( 2, GL_FLOAT, 0, &( ( *l )[ incr* maxSize ] ) );
                glDrawArrays( mode, 0, static_cast< GLsizei >( maxSize < l->size() - incr* maxSize? maxSize : l->size() - incr* maxSize ) );
                ++incr;
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ContourLinesLayer::CreateCallList
// Created: JSR 2012-01-18
// -----------------------------------------------------------------------------
void ContourLinesLayer::CreateCallList()
{
    {
        boost::mutex::scoped_lock locker( mutex_ );
        if( !computed_ )
        {
            if( !threadRunning_ )
            {
                threadRunning_  = true;
                loops_[ 0 ].clear();
                loops_[ 1 ].clear();
                loops_[ 2 ].clear();
                loops_[ 3 ].clear();
                thread_->Enqueue( boost::bind( &ContourLinesLayer::Conrec, this ) );
            }
            return;
        }
    }
    callListId_ = glGenLists( 1 );
    glNewList( callListId_, GL_COMPILE );
    glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
    glEnableClientState( GL_VERTEX_ARRAY );
    glLineWidth( 2.f );
    CreateGLArrays( GL_LINE_STRIP, loops_[ 0 ], glMaxVerticesSize_ );
    CreateGLArrays( GL_LINE_STRIP, loops_[ 1 ], glMaxVerticesSize_ );
    glLineWidth( 1.f );
    CreateGLArrays( GL_LINE_STRIP, loops_[ 2 ], glMaxVerticesSize_ );
    CreateGLArrays( GL_LINE_STRIP, loops_[ 3 ], glMaxVerticesSize_ );
    glDisableClientState( GL_VERTEX_ARRAY );
    glEndList();
    loops_[ 0 ].clear();
    loops_[ 1 ].clear();
    loops_[ 2 ].clear();
    loops_[ 3 ].clear();
}
