// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "InputPropagationAttribute.h"
#include "MIL_AgentServer.h"
#include "protocol/Protocol.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "simulation_terrain/TER_Polygon.h"
#include "simulation_terrain/TER_World.h"
#include "propagation/PropagationManager.h"
#include "propagation/ASCExtractor.h"
#include <boost/date_time/posix_time/posix_time.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( InputPropagationAttribute )

namespace bpt = boost::posix_time;

// -----------------------------------------------------------------------------
// Name: InputPropagationAttribute constructor
// Created: LGY 2012-10-05
// -----------------------------------------------------------------------------
InputPropagationAttribute::InputPropagationAttribute()
    : pManager_( new PropagationManager() )
{
    // NOTHING
}

namespace
{
    std::string BuildPropagationFile( const std::string& path )
    {
        return MIL_AgentServer::GetWorkspace().GetConfig().GetPropagationFile( path );
    }
}

// -----------------------------------------------------------------------------
// Name: InputPropagationAttribute constructor
// Created: LGY 2012-10-05
// -----------------------------------------------------------------------------
InputPropagationAttribute::InputPropagationAttribute( xml::xistream& xis )
    : model_   ( BuildPropagationFile( xis.attribute< std::string >( "source" ) ) )
    , pManager_( new PropagationManager() )
{
    pManager_->Initialize( model_ );
}

// -----------------------------------------------------------------------------
// Name: InputPropagationAttribute::operator=
// Created: LGY 2012-11-08
// -----------------------------------------------------------------------------
InputPropagationAttribute& InputPropagationAttribute::operator=( const InputPropagationAttribute& rhs )
{
    model_ = rhs.model_;
    pManager_->Initialize( model_ );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: InputPropagationAttribute destructor
// Created: LGY 2012-10-05
// -----------------------------------------------------------------------------
InputPropagationAttribute::~InputPropagationAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InputPropagationAttribute::SendFullState
// Created: LGY 2012-10-08
// -----------------------------------------------------------------------------
void InputPropagationAttribute::SendFullState( sword::ObjectAttributes& asn ) const
{
    asn.mutable_propagation()->set_model( model_ );
}

// -----------------------------------------------------------------------------
// Name: InputPropagationAttribute::load
// Created: LGY 2012-11-08
// -----------------------------------------------------------------------------
void InputPropagationAttribute::load( MIL_CheckPointInArchive& archive, const unsigned int )
{
    archive >> boost::serialization::base_object< ObjectAttribute_ABC >( *this )
            >> model_;
    pManager_->Initialize( model_ );
}

// -----------------------------------------------------------------------------
// Name: InputPropagationAttribute::save
// Created: LGY 2012-11-08
// -----------------------------------------------------------------------------
void InputPropagationAttribute::save( MIL_CheckPointOutArchive& archive, const unsigned int ) const
{
    archive << boost::serialization::base_object< ObjectAttribute_ABC >( *this )
            << model_;
}

namespace
{
    T_PointVector Convert( const std::vector< geometry::Point2d >& points )
    {
        T_PointVector convexhull;
        for( std::vector< geometry::Point2d >::const_iterator it = points.begin(); it != points.end(); ++it )
        {
            MT_Vector2D position;
            TER_World::GetWorld().MosToSimMgrsCoord( it->Y(), it->X(), position );
            convexhull.push_back( position );
        }
        return convexhull;
    }
}

// -----------------------------------------------------------------------------
// Name: InputPropagationAttribute::UpdateLocalisation
// Created: LGY 2012-11-08
// -----------------------------------------------------------------------------
void InputPropagationAttribute::UpdateLocalisation( MIL_Object_ABC& object, unsigned int time )
{
    const bpt::ptime realTime( bpt::from_time_t( MIL_AgentServer::GetWorkspace().TickToRealTime( time ) ) );
    PropagationManager::T_Files files = pManager_->GetFiles( boost::posix_time::to_iso_string( realTime ) );
    if( !files.empty() )
    {
        std::vector< geometry::Point2d > points;
        for( std::size_t i = 0; i < files.size(); ++i )
        {
            boost::shared_ptr< ASCExtractor > extractor( new ASCExtractor( files[ i ], pManager_->GetProjectionFile() ) );
            extractor->Fill( points );
            values_.push_back( extractor );
        }

        TER_Polygon polygon;
        polygon.Reset( Convert( points ), true );
        object.UpdateLocalisation( polygon );
    }
}
