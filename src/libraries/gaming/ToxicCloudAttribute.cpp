// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Tools.h"
#include "ToxicCloudAttribute.h"
#include "clients_gui/GLView_ABC.h"
#include "clients_gui/Viewport_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include <graphics/extensions.h>
#include "protocol/Protocol.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ToxicCloudAttribute constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
ToxicCloudAttribute::ToxicCloudAttribute( Controller& controller, const CoordinateConverter_ABC& converter )
    : controller_( controller )
    , converter_ ( converter )
{
//    CreateTexture();
}

// -----------------------------------------------------------------------------
// Name: ToxicCloudAttribute destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
ToxicCloudAttribute::~ToxicCloudAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ToxicCloudAttribute::CreateTexture()
// Created: SBO 2008-04-14
// -----------------------------------------------------------------------------
void ToxicCloudAttribute::CreateTexture()
{
    glGenTextures( 1, &texture_ );
    glBindTexture( GL_TEXTURE_2D, texture_ );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, gl::GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, gl::GL_CLAMP_TO_EDGE );
}

// -----------------------------------------------------------------------------
// Name: ToxicCloudAttribute::UpdateTexture
// Created: JCR 2008-06-13
// -----------------------------------------------------------------------------
void ToxicCloudAttribute::UpdateTexture() const
{
    static const float step = 510. / 2.; // meters
    const float range = ( 1.f + float( boundaries_.second - boundaries_.first ) ) / float( boundaries_.second + boundaries_.first );
    for( auto it = cloud_.begin(); it != cloud_.end(); ++it )
    {
        const geometry::Point2f& p = it->first;
        glColor4d( std::min( 1., 2. * it->second * range ), std::max( 0., 2. * ( 1.f - it->second * range ) ), 0., 0.5f );
        glBegin( GL_QUADS );
            glTexCoord1f( 0.125f );
            glVertex3f( p.X() - step, p.Y() - step, 200 );
            glVertex3f( p.X() - step, p.Y() + step, 200 );
            glVertex3f( p.X() + step, p.Y() + step, 200 );
            glVertex3f( p.X() + step, p.Y() - step, 200 );
        glEnd();
    }
}

// -----------------------------------------------------------------------------
// Name: ToxicCloudAttribute::UpdateToxicCloud
// Created: JCR 2008-06-12
// -----------------------------------------------------------------------------
void ToxicCloudAttribute::UpdateToxicCloud( const sword::LocatedQuantityList& cloud )
{
    cloud_.resize( cloud.elem_size() );
    boundaries_ = QuantityBoundaries();
    boundingBox_ = geometry::Rectangle2f();
    for( int i = 0; i < cloud.elem_size(); ++i )
    {
        const sword::LocatedQuantity& quantity = cloud.elem( i );
        const geometry::Point2f position( float( quantity.coordinate().longitude() ), float( quantity.coordinate().latitude() ) );
        boundingBox_.Incorporate( position );
        boundaries_.Incorporate( quantity.quantity() );
        cloud_[ i ] = std::make_pair( position, quantity.quantity() );
    }
}

// -----------------------------------------------------------------------------
// Name: ToxicCloudAttribute::UpdateData
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
template< typename T >
void ToxicCloudAttribute::UpdateData( const T& message )
{
    if( message.has_toxic_cloud()  )
    {
        UpdateToxicCloud( message.toxic_cloud().quantities() );
        controller_.Update( *(ToxicCloudAttribute_ABC*)this );
    }
}

// -----------------------------------------------------------------------------
// Name: ToxicCloudAttribute::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void ToxicCloudAttribute::DoUpdate( const sword::ObjectUpdate& message )
{
    UpdateData( message.attributes() );
}

// -----------------------------------------------------------------------------
// Name: ToxicCloudAttribute::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void ToxicCloudAttribute::DoUpdate( const sword::ObjectKnowledgeUpdate& message )
{
    UpdateData( message.attributes() );
}

// -----------------------------------------------------------------------------
// Name: ToxicCloudAttribute::Draw
// Created: JCR 2008-06-12
// -----------------------------------------------------------------------------
void ToxicCloudAttribute::Draw( const geometry::Point2f& /*where*/, const gui::Viewport_ABC& viewport, gui::GLView_ABC& /*tools*/ ) const
{
    if( ! viewport.IsVisible( boundingBox_ ) || cloud_.empty() )
        return;
    UpdateTexture();
}
