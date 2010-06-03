// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "ObstacleAttribute.h"
#include "Object.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Knowledge/DEC_Knowledge_ObjectAttributeObstacle.h"
#include "CheckPoints/MIL_CheckPointInArchive.h"
#include "CheckPoints/MIL_CheckPointOutArchive.h"
#include "protocol/protocol.h"
#include <xeumeuleu/xml.h>

BOOST_CLASS_EXPORT_IMPLEMENT( ObstacleAttribute )

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute constructor
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
ObstacleAttribute::ObstacleAttribute()
    : obstacle_ ( Common::ObstacleType_DemolitionTargetType_preliminary )
    , bActivated_ ( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute constructor
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
ObstacleAttribute::ObstacleAttribute( bool reserved )
    : obstacle_( reserved ? Common::ObstacleType_DemolitionTargetType_reserved : Common::ObstacleType_DemolitionTargetType_preliminary )
    , bActivated_( !reserved )
{
    // NOTHING
}

namespace 
{
    Common::ObstacleType_DemolitionTargetType ExtractObstacle( const std::string& obstacle )
    {
        if ( obstacle == "reserved" )
            return Common::ObstacleType_DemolitionTargetType_reserved;
        return Common::ObstacleType_DemolitionTargetType_preliminary;
    }

    std::string ExtractObstacle( Common::ObstacleType_DemolitionTargetType obstacle )
    {
        switch ( obstacle )
        {
        case Common::ObstacleType_DemolitionTargetType_reserved:
            return "reserved";
        default:
            return "preliminary";
        }        
    }
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute constructor
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
ObstacleAttribute::ObstacleAttribute( xml::xistream& xis )
    : obstacle_ ( ExtractObstacle( xml::attribute( xis, "type", std::string() ) ) )
    , bActivated_ ( obstacle_ == Common::ObstacleType_DemolitionTargetType_preliminary )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::ObstacleAttribute
// Created: JCR 2008-07-21
// -----------------------------------------------------------------------------
ObstacleAttribute::ObstacleAttribute( const Common::MsgMissionParameter_Value& attributes )
    : obstacle_  ( ( Common::ObstacleType_DemolitionTargetType ) attributes.list( 1 ).identifier() )
    , bActivated_ ( attributes.list( 2 ).abool() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute destructor
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
ObstacleAttribute::~ObstacleAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: template < typename Archive > void ObstacleAttribute::serialize
// Created: JCR 2008-06-19
// -----------------------------------------------------------------------------
template < typename Archive > void ObstacleAttribute::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectAttribute_ABC >( *this );
    file & obstacle_
         & bActivated_;
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::SetType
// Created: LDC 2009-03-23
// -----------------------------------------------------------------------------
void ObstacleAttribute::SetType( Common::ObstacleType_DemolitionTargetType obstacleType )
{
    obstacle_ = obstacleType;
    if( Common::ObstacleType_DemolitionTargetType_preliminary == obstacleType )
        bActivated_ = true;
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::IsActivable
// Created: JCR 2008-06-12
// -----------------------------------------------------------------------------
bool ObstacleAttribute::IsActivable() const
{
    return obstacle_ == Common::ObstacleType_DemolitionTargetType_reserved;
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::IsActivated
// Created: JCR 2008-06-12
// -----------------------------------------------------------------------------
bool ObstacleAttribute::IsActivated() const
{
    return bActivated_;
}
    
// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::Activate
// Created: JCR 2008-06-12
// -----------------------------------------------------------------------------
void ObstacleAttribute::Activate()
{
    if ( ! bActivated_ && IsActivable() )
    {
        bActivated_ = true;
        NotifyAttributeUpdated( eOnUpdate );
    }

}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::Instanciate
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void ObstacleAttribute::Instanciate( DEC_Knowledge_Object& object ) const
{
    object.Attach( *new DEC_Knowledge_ObjectAttributeObstacle( *this ) );
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::Register
// Created: JSR 2010-03-15
// -----------------------------------------------------------------------------
void ObstacleAttribute::Register( Object& object ) const
{
    object.SetAttribute< ObstacleAttribute, ObstacleAttribute >( *this );
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::Send
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void ObstacleAttribute::SendFullState( Common::MsgObjectAttributes& asn ) const
{
    asn.mutable_obstacle()->set_type( obstacle_ );
    asn.mutable_obstacle()->set_activated( bActivated_ );   
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::Send
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void ObstacleAttribute::SendUpdate( Common::MsgObjectAttributes& asn ) const
{
    if( NeedUpdate() )
    {
        asn.mutable_obstacle()->set_activated( bActivated_ );
        asn.mutable_obstacle()->set_type( Common::ObstacleType_DemolitionTargetType_reserved );
        Reset();
    }
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::WriteODB
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void ObstacleAttribute::WriteODB( xml::xostream& xos ) const
{
    xos << xml::start( "obstacle" ) 
            << xml::attribute( "type", ExtractObstacle( obstacle_ ) )
        << xml::end();
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::operator=
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
ObstacleAttribute& ObstacleAttribute::operator=( const ObstacleAttribute& rhs )
{
    obstacle_ = rhs.obstacle_;
    bActivated_ = rhs.bActivated_;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::OnUpdate
// Created: LDC 2009-03-16
// -----------------------------------------------------------------------------
void ObstacleAttribute::OnUpdate( const Common::MsgMissionParameter_Value& attribute )
{
    if( attribute.list_size() > 2 && bActivated_ != attribute.list( 2 ).abool() )
    {
        bActivated_ = attribute.list( 2 ).abool();
        NotifyAttributeUpdated( eOnUpdate );
    }
}
