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
#include "Propagation.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Tools.h"
#include "protocol/Protocol.h"
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
    propagationFiles_[ QDateTime::fromString( xis.attribute< std::string >( "time" ).c_str(), "yyyy-MM-dd'T'HH:mm:ss" ) ].push_back( xis.value< std::string >() );
}

// -----------------------------------------------------------------------------
// Name: PropagationAttribute::NotifyUpdated
// Created: LGY 2012-10-16
// -----------------------------------------------------------------------------
void PropagationAttribute::NotifyUpdated( const Simulation& simulation )
{
    if( !propagationFiles_.empty() )
    {
        QDateTime current = simulation.GetDateTime();
        IT_PropagationFiles it = propagationFiles_.begin();
        if( current >= it->first )
        {
            propagations_.clear();
            T_Files& files = it->second;
            bfs::path projection( path_ / projection_ );
            for( std::size_t i = 0; i < files.size(); ++i )
            {
                bfs::path file( path_ / bfs::path( it->second.at( i ) ).filename() );
                propagations_.push_back( boost::shared_ptr< Propagation >( new Propagation( file.string(), projection.string() , converter_, colors_ ) ) );
            }
            propagationFiles_.erase( it );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PropagationAttribute::Draw
// Created: LGY 2012-10-12
// -----------------------------------------------------------------------------
void PropagationAttribute::Draw( const geometry::Point2f& /*where*/, const kernel::Viewport_ABC& /*viewport*/, const kernel::GlTools_ABC& /*tools*/ ) const
{
    for( CIT_Propagations it = propagations_.begin(); it != propagations_.end(); ++it )
        (*it)->Draw();
}
