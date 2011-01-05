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

BOOST_CLASS_EXPORT_IMPLEMENT( BurnSurfaceAttribute )

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
    NotifyAttributeUpdated( eOnUpdate | eOnHLAUpdate );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: BurnSurfaceAttribute::load
// Created: BCI 2010-12-09
// -----------------------------------------------------------------------------
void BurnSurfaceAttribute::load( MIL_CheckPointInArchive& ar, const unsigned int version )
{
    std::string className;
    ar >> boost::serialization::base_object< ObjectAttribute_ABC >( *this );
	if( pObject_ )
		burningCells_.load( ar, *pObject_, version );
}

// -----------------------------------------------------------------------------
// Name: BurnSurfaceAttribute::save
// Created: BCI 2010-12-09
// -----------------------------------------------------------------------------
void BurnSurfaceAttribute::save( MIL_CheckPointOutArchive& ar, const unsigned int version ) const
{
    ar << boost::serialization::base_object< ObjectAttribute_ABC >( *this );
	if( pObject_ )
		burningCells_.save( ar, *pObject_, version );
}


// -----------------------------------------------------------------------------
// Name: BurnSurfaceAttribute::Instanciate
// Created: BCI 2010-12-09
// -----------------------------------------------------------------------------
void BurnSurfaceAttribute::Instanciate( DEC_Knowledge_Object& /*object*/ ) const
{
     // $$$$ BCI 2010-12-21: todo
    /*object.Attach< DEC_Knowledge_ObjectAttributeProxy_ABC< BurnSurfaceAttribute > >( *new T_KnowledgeProxyType() );*/
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
	NotifyAttributeUpdated( eOnUpdate | eOnHLAUpdate );
}
