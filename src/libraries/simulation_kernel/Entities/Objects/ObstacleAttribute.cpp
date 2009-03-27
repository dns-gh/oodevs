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
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Knowledge/DEC_Knowledge_ObjectAttributeObstacle.h"
#include "CheckPoints/MIL_CheckPointInArchive.h"
#include "CheckPoints/MIL_CheckPointOutArchive.h"

#include <xeumeuleu/xml.h>

BOOST_CLASS_EXPORT_GUID( ObstacleAttribute, "ObstacleAttribute" )

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute constructor
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
ObstacleAttribute::ObstacleAttribute()
    : obstacle_ ( EnumDemolitionTargetType::preliminary )
    , bActivated_ ( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute constructor
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
ObstacleAttribute::ObstacleAttribute( bool reserved )
    : obstacle_ ( ( reserved ) ? EnumDemolitionTargetType::reserved : EnumDemolitionTargetType::preliminary )
    , bActivated_ ( !reserved )
{
    // NOTHING
}

namespace 
{
    EnumDemolitionTargetType::Root ExtractObstacle( const std::string& obstacle )
    {
        if ( obstacle == "reserved" )
            return EnumDemolitionTargetType::reserved;
        return EnumDemolitionTargetType::preliminary;
    }

    std::string ExtractObstacle( EnumDemolitionTargetType::Root obstacle )
    {
        switch ( obstacle )
        {
        case EnumDemolitionTargetType::reserved:
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
    , bActivated_ ( obstacle_ == EnumDemolitionTargetType::preliminary )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::ObstacleAttribute
// Created: JCR 2008-07-21
// -----------------------------------------------------------------------------
ObstacleAttribute::ObstacleAttribute( const ASN1T_ObjectAttributes& asn )
    : obstacle_  ( asn.obstacle.type )
    , bActivated_ ( asn.obstacle.activated != 0 )
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
template < typename Archive > void ObstacleAttribute::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< ObjectAttribute_ABC >( *this );
    file & obstacle_
         & bActivated_;
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::SetType
// Created: LDC 2009-03-23
// -----------------------------------------------------------------------------
void ObstacleAttribute::SetType( ASN1T_EnumDemolitionTargetType obstacleType )
{
    obstacle_ = obstacleType;
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::IsActivable
// Created: JCR 2008-06-12
// -----------------------------------------------------------------------------
bool ObstacleAttribute::IsActivable() const
{
    return obstacle_ == EnumDemolitionTargetType::reserved;
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
// Name: ObstacleAttribute::Send
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void ObstacleAttribute::SendFullState( ASN1T_ObjectAttributes& asn ) const
{
    asn.m.obstaclePresent = 1;
    asn.obstacle.type = obstacle_;
    asn.obstacle.activated = bActivated_;        
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::Send
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void ObstacleAttribute::SendUpdate( ASN1T_ObjectAttributes& asn ) const
{
    if ( NeedUpdate() )
    {
        asn.m.obstaclePresent = 1;
        asn.obstacle.activated = bActivated_;
        asn.obstacle.type = EnumDemolitionTargetType::reserved;
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
void ObstacleAttribute::OnUpdate( const ASN1T_ObjectAttributes& asn )
{
    if( asn.m.obstaclePresent != 0 && bActivated_ != ( asn.obstacle.activated != 0 ) )
    {
        bActivated_ = (asn.obstacle.activated != 0);
        NotifyAttributeUpdated( eOnUpdate );
    }
}
