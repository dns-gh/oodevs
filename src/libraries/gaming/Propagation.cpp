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
#include "clients_kernel/ASCExtractor.h"
#include <graphics/Visitor2d.h>
#include <graphics/TextureVisitor_ABC.h>

namespace
{

}

// -----------------------------------------------------------------------------
// Name: Propagation constructor
// Created: LGY 2012-10-26
// -----------------------------------------------------------------------------
Propagation::Propagation( const std::string& file, const std::string& projection,
                          const kernel::CoordinateConverter_ABC& converter,
                          const std::map< double, QColor >& colors )
    : converter_ ( converter )
    , pExtractor_( new kernel::ASCExtractor( file, projection ) )
{
    const kernel::ASCExtractor::T_Values& values = pExtractor_->GetValues();
    std::vector< unsigned char > rgba( values.size() * 4 );
    for( unsigned int i = 0; i < values.size(); ++i )
    {
        float value = values[ i ];
        QColor color = colors.empty() ? Qt::green : colors.lower_bound( value )->second;
        rgba[ 4 * i ] = static_cast< char >( color.red() );
        rgba[ 4 * i + 1] = static_cast< char >(  color.green() );
        rgba[ 4 * i + 2 ] = static_cast< char >( color.blue() );
        rgba[ 4 * i + 3 ] = static_cast< char >( value > 0.f ? 255 : 0 );
    }

    pFactory_.reset( new RGBATextureFactory( &rgba[ 0 ], pExtractor_->GetCols(), pExtractor_->GetPixelSize().X() ) );
    pTree_.reset( new TextureTree( *pFactory_, pExtractor_->GetCols(), pExtractor_->GetRows() ) );
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
        VisitorProxy( kernel::ASCExtractor& extractor, const kernel::CoordinateConverter_ABC& converter )
            : extractor_( extractor )
            , converter_( converter )
        {};
        virtual ~VisitorProxy()
        {};
        virtual void Visit( const geometry::Rectangle2f&, unsigned int, unsigned int )
        {
            const geometry::Rectangle2d& extent = extractor_.GetExtent();
            const geometry::Rectangle2f project( converter_.ConvertFromGeo( extent.BottomLeft() ),
                                                 converter_.ConvertFromGeo( extent.TopRight() ) );
            glBegin( GL_QUADS );
            glTexCoord2d( 0., 0. );
            glVertex2d( project.Left(), project.Top() );
            glTexCoord2d( 1., 0. );
            glVertex2d( project.Right(), project.Top() );
            glTexCoord2d( 1.,1. );
            glVertex2d( project.Right(), project.Bottom() );
            glTexCoord2d(0., 1. );
            glVertex2d( project.Left(), project.Bottom() );
            glEnd();
        }
    private:
        kernel::ASCExtractor& extractor_;
        const kernel::CoordinateConverter_ABC& converter_;
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

    glColor4f( 1, 1, 1, 1 );
    VisitorProxy visitor( *pExtractor_, converter_ );
    pTree_->Accept( visitor );
    glPopAttrib();
}
