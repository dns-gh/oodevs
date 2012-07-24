// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "UndergroundCapacity.h"
#include "UndergroundAttribute.h"
#include "MIL_Object_ABC.h"
#include "MIL_AgentServer.h"
#include "Entities/MIL_EntityManager.h"
#include "Urban/MIL_UrbanCache.h"
#include "Urban/MIL_UrbanObject_ABC.h"

BOOST_CLASS_EXPORT_IMPLEMENT( UndergroundCapacity )

// -----------------------------------------------------------------------------
// Name: UndergroundCapacity constructor
// Created: JSR 2011-05-30
// -----------------------------------------------------------------------------
UndergroundCapacity::UndergroundCapacity()
    : urbanBlock_( 0 )
    , attribute_ ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UndergroundCapacity constructor
// Created: JSR 2011-05-30
// -----------------------------------------------------------------------------
UndergroundCapacity::UndergroundCapacity( xml::xistream& /*xis*/ )
    : urbanBlock_( 0 )
    , attribute_ ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UndergroundCapacity destructor
// Created: JSR 2011-05-30
// -----------------------------------------------------------------------------
UndergroundCapacity::~UndergroundCapacity()
{
    if( urbanBlock_ )
        urbanBlock_->Unregister( *static_cast< MIL_StructuralStateNotifier_ABC* >( this ) );
}

// -----------------------------------------------------------------------------
// Name: UndergroundCapacity constructor
// Created: JSR 2011-05-30
// -----------------------------------------------------------------------------
UndergroundCapacity::UndergroundCapacity( const UndergroundCapacity& rhs )
{
    urbanBlock_ = rhs.urbanBlock_;
    attribute_ = rhs.attribute_;
}

// -----------------------------------------------------------------------------
// Name: UndergroundCapacity::serialize
// Created: JSR 2011-05-30
// -----------------------------------------------------------------------------
template< typename Archive >
void UndergroundCapacity::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< ObjectCapacity_ABC >( *this );
}

// -----------------------------------------------------------------------------
// Name: UndergroundCapacity::Instanciate
// Created: JSR 2011-05-30
// -----------------------------------------------------------------------------
void UndergroundCapacity::Instanciate( MIL_Object_ABC& object ) const
{
    UndergroundCapacity* capacity = new UndergroundCapacity( *this );
    object.AddCapacity( capacity );
}

// -----------------------------------------------------------------------------
// Name: UndergroundCapacity::Register
// Created: JSR 2011-05-30
// -----------------------------------------------------------------------------
void UndergroundCapacity::Register( MIL_Object_ABC& object )
{
    object.AddCapacity( this );
}

// -----------------------------------------------------------------------------
// Name: UndergroundCapacity::RegisterUrbanBlock
// Created: JSR 2012-07-24
// -----------------------------------------------------------------------------
void UndergroundCapacity::RegisterUrbanBlock( MIL_Object_ABC& object )
{
    try
    {
        if( MIL_AgentServer::IsInitialized() )
        {
            attribute_ = object.RetrieveAttribute< UndergroundAttribute >();
            if( attribute_ )
            {
                const MT_Vector2D vector = object.GetLocalisation().ComputeBarycenter();
                if( const MIL_UrbanObject_ABC* object = MIL_AgentServer::GetWorkspace().GetUrbanCache().FindBlock( MT_Vector2D( static_cast< float >( vector.rX_ ), static_cast< float >( vector.rY_ ) ) ) )
                {
                    urbanBlock_ = const_cast< MIL_UrbanObject_ABC* >( object );
                    urbanBlock_->MIL_Object::Register( *static_cast< MIL_StructuralStateNotifier_ABC* >( this ) );
                }
            }
        }
    }
    catch( ... )
    {
        // NOTHING
    }
}

// -----------------------------------------------------------------------------
// Name: UndergroundCapacity::Finalize
// Created: JSR 2012-07-24
// -----------------------------------------------------------------------------
void UndergroundCapacity::Finalize( MIL_Object_ABC& object )
{
    RegisterUrbanBlock( object );
}

// -----------------------------------------------------------------------------
// Name: UndergroundCapacity::NotifyStructuralStateChanged
// Created: JSR 2012-07-23
// -----------------------------------------------------------------------------
void UndergroundCapacity::NotifyStructuralStateChanged( float structuralState, const MIL_Object_ABC& /*object*/ )
{
    if( attribute_ && structuralState < 0.25f )
        attribute_->SetActivation( false );
}
