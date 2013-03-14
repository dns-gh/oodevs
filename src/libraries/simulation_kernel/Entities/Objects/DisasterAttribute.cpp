// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DisasterAttribute.h"
#include "MIL_AgentServer.h"
#include "protocol/Protocol.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "simulation_terrain/TER_Polygon.h"
#include "simulation_terrain/TER_World.h"
#include "propagation/PropagationManager.h"
#include "propagation/ASCExtractor.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/foreach.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( DisasterAttribute )

BOOST_CLASS_EXPORT_KEY( DEC_Knowledge_ObjectAttributeProxyPassThrough< DisasterAttribute > )
BOOST_CLASS_EXPORT_IMPLEMENT( DEC_Knowledge_ObjectAttributeProxyPassThrough< DisasterAttribute > )

namespace bpt = boost::posix_time;

// -----------------------------------------------------------------------------
// Name: DisasterAttribute constructor
// Created: LGY 2012-10-05
// -----------------------------------------------------------------------------
DisasterAttribute::DisasterAttribute()
    : pManager_( new PropagationManager() )
{
    // NOTHING
}

namespace
{
    tools::Path BuildPropagationFile( const tools::Path& path )
    {
        return MIL_AgentServer::GetWorkspace().GetConfig().GetPropagationFile( path );
    }
}

// -----------------------------------------------------------------------------
// Name: DisasterAttribute constructor
// Created: LGY 2012-10-05
// -----------------------------------------------------------------------------
DisasterAttribute::DisasterAttribute( xml::xistream& xis )
    : model_   ( BuildPropagationFile( xis.attribute< tools::Path >( "source" ) ) )
    , date_    ( xis.attribute< std::string >( "date", "" ) )
    , pManager_( new PropagationManager() )
{
    pManager_->Initialize( model_, date_ );
}

// -----------------------------------------------------------------------------
// Name: DisasterAttribute constructor
// Created: LGY 2012-12-12
// -----------------------------------------------------------------------------
DisasterAttribute::DisasterAttribute( const sword::MissionParameter_Value& attributes )
    : model_   ( BuildPropagationFile( tools::Path::FromUTF8( attributes.list( 1 ).acharstr() ) ) )
    , pManager_( new PropagationManager() )
{
    if( attributes.list_size() > 2 )
        date_ = attributes.list( 2 ).acharstr();
    pManager_->Initialize( model_, date_ );
}

// -----------------------------------------------------------------------------
// Name: DisasterAttribute::operator=
// Created: LGY 2012-11-08
// -----------------------------------------------------------------------------
DisasterAttribute& DisasterAttribute::operator=( const DisasterAttribute& rhs )
{
    model_ = rhs.model_;
    date_ = rhs.date_;
    pManager_->Initialize( model_, date_ );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: DisasterAttribute destructor
// Created: LGY 2012-10-05
// -----------------------------------------------------------------------------
DisasterAttribute::~DisasterAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DisasterAttribute::SendFullState
// Created: LGY 2012-10-08
// -----------------------------------------------------------------------------
void DisasterAttribute::SendFullState( sword::ObjectAttributes& asn ) const
{
    asn.mutable_propagation()->set_model( model_.ToUTF8() );
    if( date_ != "" )
        asn.mutable_propagation()->set_date( date_ );
}

// -----------------------------------------------------------------------------
// Name: DisasterAttribute::load
// Created: LGY 2012-11-08
// -----------------------------------------------------------------------------
void DisasterAttribute::load( MIL_CheckPointInArchive& archive, const unsigned int )
{
    archive >> boost::serialization::base_object< ObjectAttribute_ABC >( *this )
            >> model_
            >> date_
            >> files_;
    pManager_->Initialize( model_, date_ );
}

// -----------------------------------------------------------------------------
// Name: DisasterAttribute::save
// Created: LGY 2012-11-08
// -----------------------------------------------------------------------------
void DisasterAttribute::save( MIL_CheckPointOutArchive& archive, const unsigned int ) const
{
    archive << boost::serialization::base_object< ObjectAttribute_ABC >( *this )
            << model_
            << date_
            << files_;
}

namespace
{
    T_PointVector Convert( const std::vector< geometry::Point2d >& points )
    {
        T_PointVector result;
        for( std::vector< geometry::Point2d >::const_iterator it = points.begin(); it != points.end(); ++it )
        {
            MT_Vector2D position;
            TER_World::GetWorld().MosToSimMgrsCoord( it->Y(), it->X(), position );
            result.push_back( position );
        }
        return result;
    }
}

// -----------------------------------------------------------------------------
// Name: DisasterAttribute::UpdateLocalisation
// Created: LGY 2012-11-08
// -----------------------------------------------------------------------------
void DisasterAttribute::UpdateLocalisation( MIL_Object_ABC& object, unsigned int time )
{
    const bpt::ptime realTime( bpt::from_time_t( MIL_AgentServer::GetWorkspace().TickToRealTime( time ) ) );
    tools::Path::T_Paths files = pManager_->GetFiles( boost::posix_time::to_iso_string( realTime ) );
    if( files_ != files )
    {
        values_.clear();
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

        files_ = files;
    }
}

// -----------------------------------------------------------------------------
// Name: DisasterAttribute::GetDose
// Created: LGY 2012-11-22
// -----------------------------------------------------------------------------
float DisasterAttribute::GetDose( const MT_Vector2D& position ) const
{
    double latitude, longitude;
    TER_World::GetWorld().SimToMosMgrsCoord( position, latitude, longitude );

    BOOST_FOREACH( boost::shared_ptr< ASCExtractor > value, values_ )
    {
        float dose = value->GetValue( longitude, latitude );
        if( dose > 0.f )
            return dose;
    }
    return 0.f;
}

// -----------------------------------------------------------------------------
// Name: DisasterAttribute::Register
// Created: LGY 2012-12-06
// -----------------------------------------------------------------------------
void DisasterAttribute::Register( MIL_Object_ABC& object ) const
{
    object.SetAttribute< DisasterAttribute, DisasterAttribute >( *this );
}

// -----------------------------------------------------------------------------
// Name: DisasterAttribute::Instanciate
// Created: LGY 2012-12-06
// -----------------------------------------------------------------------------
void DisasterAttribute::Instanciate( DEC_Knowledge_Object& object ) const
{
    object.Attach< DEC_Knowledge_ObjectAttributeProxy_ABC< DisasterAttribute > >( *new T_KnowledgeProxyType() );
}

// -----------------------------------------------------------------------------
// Name: DisasterAttribute::Update
// Created: LGY 2012-12-06
// -----------------------------------------------------------------------------
bool DisasterAttribute::Update( const DisasterAttribute& rhs )
{
    if( files_ != rhs.files_ || model_ != rhs.model_ || date_ != rhs.date_ )
    {
        NotifyAttributeUpdated( eOnUpdate );
        files_ = rhs.files_;
        values_ = rhs.values_;
        date_ = rhs.date_;
        model_ = rhs.model_;
    }
    return NeedUpdate( eOnUpdate );
}
