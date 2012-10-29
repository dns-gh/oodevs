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
#include <boost/chrono.hpp>

namespace
{
    class ColorFactory : public ElevationColor_ABC
    {
    public:
                 ColorFactory() {};
        virtual ~ColorFactory() {};

        virtual void SelectColor( short elevation, short max, unsigned char* color )
        {
            color[ 0 ] = 255 - unsigned char( float( elevation ) / float( max ) * 128 );
            color[ 1 ] = 0;
            color[ 2 ] = 0;
            color[ 3 ] = elevation > 0 ? 255 : 0;
        }
        virtual bool IsTransparencyEnabled() const
        {
            return true;
        }
    };
}

// -----------------------------------------------------------------------------
// Name: Propagation constructor
// Created: LGY 2012-10-26
// -----------------------------------------------------------------------------
Propagation::Propagation( const std::string& file, const std::string& projection,
                          const kernel::CoordinateConverter_ABC& converter )
    : converter_ ( converter )
    , pColor_    ( new ColorFactory() )
    , pExtractor_( new kernel::ASCExtractor( file, projection ) )
    , pMap_      ( new ElevationMap( static_cast< std::size_t >( pExtractor_->GetCols() ),
                                     static_cast< std::size_t >( pExtractor_->GetRows() ),
                                     pExtractor_->GetPixelSize().X(),
                                     &pExtractor_->GetValues()[0] ) )
    , pFactory_  ( new ElevationFactory( *pMap_, *pColor_ ) )
    , pTree_     ( new TextureTree( *pFactory_, pMap_->Width(), pMap_->Height() ) )
{
    // NOTHING
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
            glDisable ( GL_TEXTURE_GEN_S );
            glDisable ( GL_TEXTURE_GEN_T );
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
