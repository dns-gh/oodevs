// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Propagation.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/DisasterType.h"
#include "propagation/Extractor_ABC.h"
#include "propagation/PropagationManager.h"
#include <graphics/TextureVisitor_ABC.h>

// -----------------------------------------------------------------------------
// Name: Propagation constructor
// Created: LGY 2012-10-26
// -----------------------------------------------------------------------------
Propagation::Propagation( const tools::Path& file, const PropagationManager& manager,
                          const kernel::CoordinateConverter_ABC& converter, const kernel::DisasterType& disasterType )
{
    boost::shared_ptr< Extractor_ABC > extractor = manager.CreateExtractor( file );
    auto values = extractor->GetValues();

    std::vector< unsigned char > rgba( values.size() * 4 );
    const int rowsCount = extractor->GetRows();
    const int colsCount = extractor->GetCols();

    for( int i = 0; i < rowsCount; i++ )
    {
        const size_t rowOffset = ( rowsCount - i - 1 ) * ( colsCount * 4 );
        for( int j = 0; j < colsCount; j++ )
        {
            const float value = values[ i * colsCount + j ];
            QColor color = disasterType.GetColor( value );
            bool valid = color.isValid();
            rgba[ rowOffset + 4 * j ]       = valid ? static_cast< char >( color.red() ) : 0u;
            rgba[ rowOffset + 4 * j + 1 ]   = valid ? static_cast< char >( color.green() ) : 0u;
            rgba[ rowOffset + 4 * j + 2 ]   = valid ? static_cast< char >( color.blue() ) : 0u;
            rgba[ rowOffset + 4 * j + 3 ]   = valid ? static_cast< char >( value > 0.f ? 255u : 0u ) : 0u;
        }
    }

    const geometry::Rectangle2d& extent = extractor->GetExtent();
    globalExtent_= geometry::Rectangle2f( converter.ConvertFromGeo( extent.BottomLeft() ),
                                          converter.ConvertFromGeo( extent.TopRight() ) );
    const double pixelSize = extractor->GetPixelSize().X();
    pFactory_.reset( new RGBATextureFactory( &rgba[ 0 ], colsCount, pixelSize ) );
    pTree_.reset( new TextureTree( *pFactory_, colsCount, rowsCount ) );
}

// -----------------------------------------------------------------------------
// Name: Propagation destructor
// Created: LGY 2012-10-26
// -----------------------------------------------------------------------------
Propagation::~Propagation()
{
    // NOTHING
}

namespace
{
    struct VisitorProxy : public TextureVisitor_ABC
    {
        explicit VisitorProxy( const geometry::Rectangle2f& globalExtent )
            : globalExtent_( globalExtent )
        {};
        virtual ~VisitorProxy()
        {};
        virtual void Visit( const geometry::Rectangle2f& extent, unsigned int, unsigned int )
        {
            glBegin( GL_QUADS );
            glTexCoord2d( 0., 0. );
            glVertex2d( extent.Left()  + globalExtent_.Left(), extent.Bottom() + globalExtent_.Bottom() );
            glTexCoord2d( 1., 0. );
            glVertex2d( extent.Right() + globalExtent_.Left(), extent.Bottom() + globalExtent_.Bottom() );
            glTexCoord2d( 1.,1. );
            glVertex2d( extent.Right() + globalExtent_.Left(), extent.Top()    + globalExtent_.Bottom() );
            glTexCoord2d(0., 1. );
            glVertex2d( extent.Left() + globalExtent_.Left(), extent.Top()     + globalExtent_.Bottom() );
            glEnd();
        }
    private:
        const geometry::Rectangle2f& globalExtent_;
    };
}

// -----------------------------------------------------------------------------
// Name: Propagation::Draw
// Created: LGY 2012-10-26
// -----------------------------------------------------------------------------
void Propagation::Draw() const
{
    if( !pTree_.get() )
        return;
    glPushAttrib( GL_TEXTURE_BIT );

    float color[ 4 ];
    glGetFloatv( GL_CURRENT_COLOR, color );

    glColor4f( 1, 1, 1, color[ 3 ] );
    VisitorProxy visitor( globalExtent_ );
    pTree_->Accept( visitor );
    glPopAttrib();
}
