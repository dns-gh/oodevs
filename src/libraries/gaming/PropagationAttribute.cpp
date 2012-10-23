// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "PropagationAttribute.h"
#include "Simulation.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Tools.h"
#include "protocol/Protocol.h"
#include <boost/filesystem/path.hpp>
#include <boost/foreach.hpp>
#include <xeumeuleu/xml.hpp>

namespace bfs = boost::filesystem;

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: PropagationAttribute constructor
// Created: LGY 2012-10-12
// -----------------------------------------------------------------------------
PropagationAttribute::PropagationAttribute( Controller& controller, const CoordinateConverter_ABC& converter )
    : controller_( controller )
    , converter_ ( converter )
{
    controller_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: PropagationAttribute destructor
// Created: LGY 2012-10-12
// -----------------------------------------------------------------------------
PropagationAttribute::~PropagationAttribute()
{
    controller_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: PropagationAttribute::DoUpdate
// Created: LGY 2012-10-12
// -----------------------------------------------------------------------------
void PropagationAttribute::DoUpdate( const sword::ObjectUpdate& message )
{
    if( message.attributes().has_propagation() )
    {
        bfs::path path( message.attributes().propagation().model() );
        xml::xifstream xis( path.string() );
        xis >> xml::start( "config" )
                >> xml::content( "projection", projection_ )
                >> xml::start( "files" )
                    >> xml::list( "file", *this, &PropagationAttribute::ReadFile )
                >> xml::end
                >> xml::optional
                    >> xml::start( "colors" )
                        >> xml::list( "color", *this, &PropagationAttribute::ReadColor )
                    >> xml::end
            >> xml::end;
        path_ = path.parent_path();
    }
}

// -----------------------------------------------------------------------------
// Name: PropagationAttribute::ReadColor
// Created: LGY 2012-10-22
// -----------------------------------------------------------------------------
void PropagationAttribute::ReadColor( xml::xistream& xis )
{
    colors_[ xis.attribute< float >( "threshold" ) ] = QColor( xis.value< std::string >().c_str() );
}

// -----------------------------------------------------------------------------
// Name: PropagationAttribute::ReadFile
// Created: LGY 2012-10-12
// -----------------------------------------------------------------------------
void PropagationAttribute::ReadFile( xml::xistream& xis )
{
    propagation_[ QDateTime::fromString( xis.attribute< std::string >( "time" ).c_str(), "yyyy-MM-dd'T'HH:mm:ss" ) ].push_back( xis.value< std::string >() );
}

// -----------------------------------------------------------------------------
// Name: PropagationAttribute::NotifyUpdated
// Created: LGY 2012-10-16
// -----------------------------------------------------------------------------
void PropagationAttribute::NotifyUpdated( const Simulation& simulation )
{
    if( !propagation_.empty() )
    {
        QDateTime current = simulation.GetDateTime();
        IT_Propagation it = propagation_.begin();
        if( current >= it->first )
        {
            tiles_.clear();
            T_Files files = it->second;
            for( std::size_t i = 0; i < files.size(); ++i )
            {
                bfs::path file( it->second.at( i ) );
                bfs::path path( path_ / file.filename() );
                bfs::path projection( path_ / projection_ );
                kernel::ASCExtractor extractor( path.string(), projection.string() );
                const kernel::ASCExtractor::T_Tiles& tmp = extractor.GetTiles();
                tiles_.insert( tiles_.end(), tmp.begin(), tmp.end() );
            }
            propagation_.erase( it );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PropagationAttribute::Draw
// Created: LGY 2012-10-12
// -----------------------------------------------------------------------------
void PropagationAttribute::Draw( const geometry::Point2f& /*where*/, const kernel::Viewport_ABC& /*viewport*/, const kernel::GlTools_ABC& /*tools*/ ) const
{
    glPushAttrib( GL_LINE_BIT | GL_CURRENT_BIT | GL_STENCIL_BUFFER_BIT | GL_LIGHTING_BIT );
    BOOST_FOREACH( kernel::ASCExtractor::T_Tile tile, tiles_ )
    {
        QColor color = colors_.empty() ? Qt::green : colors_.lower_bound( tile.second )->second;
        glColor4f( color.red() / 255.f, color.green() / 255.f, color.blue() / 255.f, 1.f );
        geometry::Point2f lb = converter_.ConvertFromGeo( tile.first.BottomLeft( ) );
        geometry::Point2f rt = converter_.ConvertFromGeo( tile.first.TopRight( ) );
        glRectfv( reinterpret_cast< const GLfloat* >( &lb ), reinterpret_cast< const GLfloat* >( &rt ) );
    }
    glPopAttrib();
}
