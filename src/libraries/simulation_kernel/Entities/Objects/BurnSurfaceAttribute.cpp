// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "BurnSurfaceAttribute.h"
#include "FireAttribute.h"
#include "CheckPoints/MIL_CheckPointInArchive.h"
#include "CheckPoints/MIL_CheckPointOutArchive.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "MIL_BurningCells.h"
#include "MIL_AgentServer.h"
#include "protocol/Protocol.h"
#include "Network/NET_AsnException.h"
#include "Tools/MIL_Tools.h"

BOOST_CLASS_EXPORT_IMPLEMENT( BurnSurfaceAttribute )

BOOST_CLASS_EXPORT_KEY( DEC_Knowledge_ObjectAttributeProxyPassThrough< BurnSurfaceAttribute > )
BOOST_CLASS_EXPORT_IMPLEMENT( DEC_Knowledge_ObjectAttributeProxyPassThrough< BurnSurfaceAttribute > )

// -----------------------------------------------------------------------------
// Name: BurnSurfaceAttribute constructor
// Created: BCI 2010-12-09
// -----------------------------------------------------------------------------
BurnSurfaceAttribute::BurnSurfaceAttribute( MIL_Object_ABC* pObject )
    : burningCells_( MIL_AgentServer::GetWorkspace().GetBurningCells() )
    , pObject_( pObject )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: BurnSurfaceAttribute constructor
// Created: BCI 2010-12-09
// -----------------------------------------------------------------------------
BurnSurfaceAttribute::BurnSurfaceAttribute( const BurnSurfaceAttribute& other )
: burningCells_( other.burningCells_ )
, pObject_( other.pObject_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BurnSurfaceAttribute destructor
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
BurnSurfaceAttribute::~BurnSurfaceAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BurnSurfaceAttribute::operator=
// Created: BCI 2010-12-09
// -----------------------------------------------------------------------------
BurnSurfaceAttribute& BurnSurfaceAttribute::operator=( const BurnSurfaceAttribute& other )
{
    pObject_ = other.pObject_;
    NotifyAttributeUpdated( eOnUpdate);
    return *this;
}

// -----------------------------------------------------------------------------
// Name: BurnSurfaceAttribute::load
// Created: BCI 2010-12-09
// -----------------------------------------------------------------------------
void BurnSurfaceAttribute::load( MIL_CheckPointInArchive& ar, const unsigned int version )
{
    ar >> boost::serialization::base_object< ObjectAttribute_ABC >( *this );
    unsigned int objectId;
    ar >> objectId;
    burningCells_.load( ar, objectId, version );
}

// -----------------------------------------------------------------------------
// Name: BurnSurfaceAttribute::save
// Created: BCI 2010-12-09
// -----------------------------------------------------------------------------
void BurnSurfaceAttribute::save( MIL_CheckPointOutArchive& ar, const unsigned int version ) const
{
    if( !pObject_ )
        throw std::runtime_error( __FUNCTION__ ": cannot save unitialized attribute" );

    ar << boost::serialization::base_object< ObjectAttribute_ABC >( *this );
    unsigned int objectId = pObject_->GetID();
    ar << objectId;
    burningCells_.save( ar, *pObject_, version );
}

// -----------------------------------------------------------------------------
// Name: BurnSurfaceAttribute::Instanciate
// Created: BCI 2010-12-09
// -----------------------------------------------------------------------------
void BurnSurfaceAttribute::Instanciate( DEC_Knowledge_Object& object ) const
{
    object.Attach< DEC_Knowledge_ObjectAttributeProxy_ABC< BurnSurfaceAttribute > >( *new T_KnowledgeProxyType() );
}

// -----------------------------------------------------------------------------
// Name: BurnSurfaceAttribute::Register
// Created: BCI 2010-12-09
// -----------------------------------------------------------------------------
void BurnSurfaceAttribute::Register( MIL_Object_ABC& object ) const
{
    BurnSurfaceAttribute attribute( *this );
    attribute.pObject_ = &object;
    object.SetAttribute< BurnSurfaceAttribute, BurnSurfaceAttribute >( attribute );
}

// -----------------------------------------------------------------------------
// Name: BurnSurfaceAttribute::SendFullState
// Created: BCI 2010-12-09
// -----------------------------------------------------------------------------
void BurnSurfaceAttribute::SendFullState( sword::ObjectAttributes& asn ) const
{
    if( pObject_ )
        burningCells_.SendFullState( asn, *pObject_ );
}

// -----------------------------------------------------------------------------
// Name: BurnSurfaceAttribute::Send
// Created: BCI 2010-12-09
// -----------------------------------------------------------------------------
void BurnSurfaceAttribute::SendUpdate( sword::ObjectAttributes& asn ) const
{
    if( NeedUpdate( eOnUpdate ) && pObject_ )
    {
        burningCells_.SendUpdate( asn, *pObject_ );
        Reset( eOnUpdate );
    }
}

// -----------------------------------------------------------------------------
// Name: BurnSurfaceAttribute::WriteODB
// Created: BCI 2010-12-09
// -----------------------------------------------------------------------------
void BurnSurfaceAttribute::WriteODB( xml::xostream& xos ) const
{
    if( pObject_ )
        burningCells_.WriteODB( xos, *pObject_ );
}

// -----------------------------------------------------------------------------
// Name: BurnSurfaceAttribute::NotifyCellsUpdated
// Created: BCI 2010-12-22
// -----------------------------------------------------------------------------
void BurnSurfaceAttribute::NotifyCellsUpdated()
{
    NotifyAttributeUpdated( eOnUpdate );
}

// -----------------------------------------------------------------------------
// Name: BurnSurfaceAttribute::ComputePathCost
// Created: BCI 2011-01-05
// -----------------------------------------------------------------------------
double BurnSurfaceAttribute::ComputePathCost( const MT_Vector2D& from, const MT_Vector2D& to ) const
{
    return burningCells_.ComputePathCost( from, to );
}

// -----------------------------------------------------------------------------
// Name: BurnSurfaceAttribute::Update
// Created: BCI 2011-01-06
// -----------------------------------------------------------------------------
bool BurnSurfaceAttribute::Update( const BurnSurfaceAttribute& rhs )
{
    // $$$$ BCI 2011-01-06: pas trop compris le but de cette méthode...
    if( pObject_ != rhs.pObject_ )
    {
        NotifyAttributeUpdated( eOnUpdate );
        pObject_ = rhs.pObject_;
    }
    return NeedUpdate( eOnUpdate );
}

// -----------------------------------------------------------------------------
// Name: BurnSurfaceAttribute::OnReceiveBurningCellRequest
// Created: BCI 2011-03-01
// -----------------------------------------------------------------------------
void BurnSurfaceAttribute::OnReceiveBurningCellRequest( const sword::BurningCellRequest& message )
{
    burningCells_.OnRequest( message.x(), message.y() );
}

