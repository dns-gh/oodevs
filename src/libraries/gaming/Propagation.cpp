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
#include "propagation/ASCExtractor.h"
#include "propagation/PropagationManager.h"
#include <graphics/Visitor2d.h>
#include <graphics/TextureVisitor_ABC.h>


// -----------------------------------------------------------------------------
// Name: Propagation constructor
// Created: LGY 2012-10-26
// -----------------------------------------------------------------------------
Propagation::Propagation( const std::string& file, const PropagationManager& manager,
                          const kernel::CoordinateConverter_ABC& converter, const kernel::DisasterType& disasterType )
{
     ASCExtractor extractor( file, manager.GetProjectionFile() );
    const ASCExtractor::T_Values& values = extractor.GetValues();
    std::vector< unsigned char > rgba( values.size() * 4 );
    const size_t rowsCount = extractor.GetRows();
    const size_t colsCount = extractor.GetCols();

    for( size_t i = 0; i < rowsCount; i++ )
    {
        const size_t rowOffset = ( rowsCount - i - 1 ) * ( colsCount * 4 );
        for( size_t j = 0; j < colsCount; j++ )
        {
            const float value = values[ i * colsCount + j ];
            QColor color = disasterType.GetColor( value );
            rgba[ rowOffset + 4 * j ]       = static_cast< char >( color.red() );
            rgba[ rowOffset + 4 * j + 1 ]   = static_cast< char >( color.green() );
            rgba[ rowOffset + 4 * j + 2 ]   = static_cast< char >( color.blue() );
            rgba[ rowOffset + 4 * j + 3 ]   = static_cast< char >( value > 0.f ? 255 : 0 );
        }
    }

    pFactory_.reset( new RGBATextureFactory( &rgba[ 0 ], colsCount, extractor.GetPixelSize().X() ) );
    pTree_.reset( new TextureTree( *pFactory_, colsCount, rowsCount ) );
    const geometry::Rectangle2d& extent = extractor.GetExtent();
    globalExtent_= geometry::Rectangle2f( converter.ConvertFromGeo( extent.BottomLeft() ),
                                          converter.ConvertFromGeo( extent.TopRight() ) );
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
                        , private boost::noncopyable
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
