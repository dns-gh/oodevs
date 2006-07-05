x// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "GlProxy.h"
#include "Gl3dWidget.h"
#include "GlWidget.h"
#include "Layer_ABC.h"

// -----------------------------------------------------------------------------
// Name: GlProxy constructor
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
GlProxy::GlProxy()
    : view_   ( 0 )
    , tools_  ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GlProxy destructor
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
GlProxy::~GlProxy()
{
    for( IT_Layers it = layers_.begin(); it != layers_.end(); ++it )
        delete *it;
    for( IT_Layers it = defaults_.begin(); it != defaults_.end(); ++it )
        delete *it;
}

// -----------------------------------------------------------------------------
// Name: GlProxy::Register
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void GlProxy::Register( Layer_ABC& layer )
{
    layers_.push_back( & layer );
}
    
// -----------------------------------------------------------------------------
// Name: GlProxy::SetDefaultLayer
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void GlProxy::AddDefaultLayer( Layer_ABC& layer )
{
    defaults_.push_back( & layer );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::RemoveAll
// Created: SBO 2006-05-24
// -----------------------------------------------------------------------------
void GlProxy::RemoveAll()
{
    for( IT_Layers it = layers_.begin(); it != layers_.end(); ++it )
        delete *it;
    layers_.clear();
}

// -----------------------------------------------------------------------------
// Name: GlProxy::ReallyRegisterTo
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
template< typename Widget >
void GlProxy::ReallyRegisterTo( Widget* widget )
{
    for( IT_Layers it = layers_.begin(); it != layers_.end(); ++it )
        (*it)->RegisterIn( *widget );
    for( IT_Layers it = defaults_.begin(); it != defaults_.end(); ++it )
        (*it)->SetDefaultIn( *widget );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::ChangeTo
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void GlProxy::ChangeTo( Gl3dWidget* newWidget )
{
    view_  = newWidget;
    tools_ = newWidget;
}

// -----------------------------------------------------------------------------
// Name: GlProxy::ChangeTo
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void GlProxy::ChangeTo( GlWidget* newWidget )
{
    view_  = newWidget;
    tools_ = newWidget;
}

// -----------------------------------------------------------------------------
// Name: GlProxy::RegisterTo
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void GlProxy::RegisterTo( Gl3dWidget* newWidget )
{
    ReallyRegisterTo( newWidget );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::RegisterTo
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void GlProxy::RegisterTo( GlWidget* newWidget )
{
    ReallyRegisterTo( newWidget );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::CheckView
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void GlProxy::CheckView() const
{
    if( ! view_ )
        throw std::runtime_error( "View not set !" );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::CheckTools
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void GlProxy::CheckTools() const
{
    if( ! tools_ )
        throw std::runtime_error( "Tools not set !" );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::CenterOn
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void GlProxy::CenterOn( const geometry::Point2f& point )
{
//    CheckView();
    view_->CenterOn( point );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::Select
// Created: AGE 2006-03-30
// -----------------------------------------------------------------------------
bool GlProxy::Select( bool b ) const
{
//    CheckTools();
    return tools_->Select( b );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::ShouldDisplay
// Created: AGE 2006-03-30
// -----------------------------------------------------------------------------
bool GlProxy::ShouldDisplay( const std::string& name ) const
{
//    CheckTools();
    return tools_->ShouldDisplay( name );
}
    
// -----------------------------------------------------------------------------
// Name: GlProxy::ShouldDisplay
// Created: AGE 2006-03-30
// -----------------------------------------------------------------------------
bool GlProxy::ShouldDisplay( const std::string& name, bool autoCondition ) const
{
//    CheckTools();
    return tools_->ShouldDisplay( name, autoCondition );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::Pixels
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
float GlProxy::Pixels() const
{
//    CheckTools();
    return tools_->Pixels();
}

// -----------------------------------------------------------------------------
// Name: GlProxy::StipplePattern
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
unsigned short GlProxy::StipplePattern( int factor /*= 1*/ ) const
{
//    CheckTools();
    return tools_->StipplePattern( factor );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::DrawCross
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void GlProxy::DrawCross( const geometry::Point2f& at, float size /*= -1.f*/ ) const
{
//    CheckTools();
    tools_->DrawCross( at, size );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::DrawLine
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void GlProxy::DrawLine( const geometry::Point2f& from, const geometry::Point2f& to ) const
{
//    CheckTools();
    tools_->DrawLine( from, to );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::DrawLines
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void GlProxy::DrawLines( const T_PointVector& points ) const
{
//    CheckTools();
    tools_->DrawLines( points );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::DrawArrow
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void GlProxy::DrawArrow( const geometry::Point2f& from, const geometry::Point2f& to, float size /*= -1.f*/ ) const
{
//    CheckTools();
    tools_->DrawArrow( from, to, size );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::DrawCurvedArrow
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void GlProxy::DrawCurvedArrow( const geometry::Point2f& from, const geometry::Point2f& to, float curveRatio /*= 0.2f*/, float size /*= -1.f*/ ) const
{
//    CheckTools();
    tools_->DrawCurvedArrow( from, to, curveRatio, size );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::DrawArc
// Created: AGE 2006-05-17
// -----------------------------------------------------------------------------
void GlProxy::DrawArc( const geometry::Point2f& center, const geometry::Point2f& p1, const geometry::Point2f& p2 ) const
{
    tools_->DrawArc( center, p1, p2 );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::DrawCircle
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void GlProxy::DrawCircle( const geometry::Point2f& center, float radius /*= -1.f*/ ) const
{
//    CheckTools();
    tools_->DrawCircle( center, radius );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::DrawDisc
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void GlProxy::DrawDisc( const geometry::Point2f& center, float radius /*= -1.f*/ ) const
{
//    CheckTools();
    tools_->DrawDisc( center, radius );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::DrawRectangle
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void GlProxy::DrawRectangle( const geometry::Point2f& center, float height, float factor /*= 1.f*/ ) const
{
//    CheckTools();
    tools_->DrawRectangle( center, height, factor );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::Print
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void GlProxy::Print( const std::string& message, const geometry::Point2f& where ) const
{
//    CheckTools();
    tools_->Print( message, where );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::DrawApp6Symbol
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void GlProxy::DrawApp6Symbol( const std::string& symbol, const geometry::Point2f& where, float factor /*= 1.f*/ ) const
{
//    CheckTools();
    tools_->DrawApp6Symbol( symbol, where, factor );
}

// -----------------------------------------------------------------------------
// Name: GlProxy::DrawIcon
// Created: AGE 2006-04-07
// -----------------------------------------------------------------------------
void GlProxy::DrawIcon( const char** xpm, const geometry::Point2f& where, float size /*= -1.f*/ ) const
{
//    CheckTools();
    tools_->DrawIcon( xpm, where, size );
}
