// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "HLA_LocalObject.h"
#include "HLA_Federate.h"
#include "HLA_InteractionManager_ABC.h"
#include "HLA_UpdateFunctor.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "MIL_AgentServer.h"

using namespace hla;

// -----------------------------------------------------------------------------
// Name: HLA_LocalObject constructor
// Created: AGE 2004-11-30
// -----------------------------------------------------------------------------
HLA_LocalObject::HLA_LocalObject( MIL_Object_ABC& object )
    : object_( object )
    , id_    ( unsigned int( -1 ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: HLA_LocalObject destructor
// Created: AGE 2004-11-30
// -----------------------------------------------------------------------------
HLA_LocalObject::~HLA_LocalObject()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: HLA_LocalObject::Activate
// Created: LGY 2010-10-01
// -----------------------------------------------------------------------------
void HLA_LocalObject::Activate()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: HLA_LocalObject::Deserialize
// Created: AGE 2004-11-30
// -----------------------------------------------------------------------------
void HLA_LocalObject::Deserialize( const AttributeIdentifier& , const Deserializer& )
{
    throw std::exception( "local object should not be deserialized" );
}

// -----------------------------------------------------------------------------
// Name: HLA_LocalObject::Serialize
// Created: AGE 2004-11-30
// -----------------------------------------------------------------------------
void HLA_LocalObject::Serialize( UpdateFunctor_ABC& functor, bool bUpdateAll ) const
{
    HLA_UpdateFunctor updateFunctor( functor, bUpdateAll );
    object_.Serialize( updateFunctor );
}

// -----------------------------------------------------------------------------
// Name: HLA_LocalObject::GetId
// Created: AGE 2004-12-06
// -----------------------------------------------------------------------------
ObjectIdentifier HLA_LocalObject::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: HLA_LocalObject::SetId
// Created: AGE 2004-12-06
// -----------------------------------------------------------------------------
void HLA_LocalObject::SetId( const ObjectIdentifier& objectId )
{
    id_ = objectId;
}

// -----------------------------------------------------------------------------
// Name: HLA_LocalObject::Destroy
// Created: AGE 2004-12-06
// -----------------------------------------------------------------------------
void HLA_LocalObject::Destroy()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: HLA_LocalObject::HideObject
// Created: AGE 2004-12-13
// -----------------------------------------------------------------------------
bool HLA_LocalObject::HideObject() const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: HLA_LocalObject::Construct
// Created: LGY 2010-10-01
// -----------------------------------------------------------------------------
void HLA_LocalObject::Construct( double /*rDeltaPercentage*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: HLA_LocalObject::Destroy
// Created: LGY 2010-10-01
// -----------------------------------------------------------------------------
void HLA_LocalObject::Destroy( double /*rDeltaPercentage*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: HLA_LocalObject::Mine
// Created: LGY 2010-10-01
// -----------------------------------------------------------------------------
void HLA_LocalObject::Mine( double /*rDeltaPercentage*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: HLA_LocalObject::Demine
// Created: LGY 2010-10-01
// -----------------------------------------------------------------------------
void HLA_LocalObject::Demine( double /*rDeltaPercentage*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: HLA_LocalObject::Bypass
// Created: LGY 2010-10-01
// -----------------------------------------------------------------------------
void HLA_LocalObject::Bypass( double /*rDeltaPercentage*/ )
{
    // NOTHING
}
