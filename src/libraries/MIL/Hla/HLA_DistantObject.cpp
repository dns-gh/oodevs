// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-11-30 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Hla/HLA_DistantObject.cpp $
// $Author: Age $
// $Modtime: 31/01/05 12:00 $
// $Revision: 6 $
// $Workfile: HLA_DistantObject.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "HLA_DistantObject.h"
#include "HLA_InteractionManager_ABC.h"
#include "HLA_Serialization.h"
#include "HLA_Construction.h"
#include "HLA_Mining.h"
#include "HLA_Bypassing.h"
#include "HLA_Activation.h"

#include "MIL_AgentServer.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Objects/MIL_ObjectManager.h"
#include "Entities/Objects/MIL_RealObject_ABC.h"
#include "Entities/Objects/MIL_RealObjectType.h"
#include "hla/Deserializer.h"
#include "hla/AttributeIdentifier.h"

// -----------------------------------------------------------------------------
// Name: HLA_DistantObject constructor
// Created: AGE 2004-11-30
// -----------------------------------------------------------------------------
HLA_DistantObject::HLA_DistantObject( const ObjectIdentifier& objectId, HLA_InteractionManager_ABC& interactionManager )
    : pObject_( 0 )
    , rConstructionPercentage_( 0 )
    , rMiningPercentage_      ( 0 )
    , rBypassPercentage_      ( 0 )
    , id_( objectId )
    , interactionManager_( interactionManager )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: HLA_DistantObject destructor
// Created: AGE 2004-11-30
// -----------------------------------------------------------------------------
HLA_DistantObject::~HLA_DistantObject()
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: HLA_DistantObject::Construct
// Created: AGE 2004-12-06
// -----------------------------------------------------------------------------
void HLA_DistantObject::Construct( double rDeltaPercentage )
{
    interactionManager_.Send( HLA_Construction( *this, rDeltaPercentage ) );
}

// -----------------------------------------------------------------------------
// Name: HLA_DistantObject::Destroy
// Created: AGE 2004-12-06
// -----------------------------------------------------------------------------
void HLA_DistantObject::Destroy( double rDeltaPercentage )
{
    interactionManager_.Send( HLA_Construction( *this, - rDeltaPercentage ) );
}

// -----------------------------------------------------------------------------
// Name: HLA_DistantObject::Mine
// Created: AGE 2004-12-06
// -----------------------------------------------------------------------------
void HLA_DistantObject::Mine( double rDeltaPercentage )
{
    interactionManager_.Send( HLA_Mining( *this, rDeltaPercentage ) );
}

// -----------------------------------------------------------------------------
// Name: HLA_DistantObject::Demine
// Created: AGE 2004-12-06
// -----------------------------------------------------------------------------
void HLA_DistantObject::Demine( double rDeltaPercentage )
{
    interactionManager_.Send( HLA_Mining( *this, - rDeltaPercentage ) );
}

// -----------------------------------------------------------------------------
// Name: HLA_DistantObject::Bypass
// Created: AGE 2004-12-06
// -----------------------------------------------------------------------------
void HLA_DistantObject::Bypass( double rDeltaPercentage )
{
    interactionManager_.Send( HLA_Bypassing( *this, rDeltaPercentage ) );
}

// -----------------------------------------------------------------------------
// Name: HLA_DistantObject::Prepare
// Created: AGE 2004-12-06
// -----------------------------------------------------------------------------
void HLA_DistantObject::Prepare()
{
    interactionManager_.Send( HLA_Activation( *this, false ) );
}
    
// -----------------------------------------------------------------------------
// Name: HLA_DistantObject::Activate
// Created: AGE 2004-12-06
// -----------------------------------------------------------------------------
void HLA_DistantObject::Activate()
{
    interactionManager_.Send( HLA_Activation( *this, true ) );
}

// -----------------------------------------------------------------------------
// Name: HLA_DistantObject::Deserialize
// Created: AGE 2004-11-30
// -----------------------------------------------------------------------------
void HLA_DistantObject::Deserialize( const AttributeIdentifier& attributeID, const Deserializer& deserializer )
{
    if( pObject_ )
        pObject_->Deserialize( attributeID, deserializer );
    else
    {
        DeserializeAttribute( attributeID, deserializer );
        pObject_ = InstanciateObject();
        if( pObject_ )
            MIL_AgentServer::GetWorkspace().GetEntityManager().RegisterObject( *pObject_ );
    }
}

// -----------------------------------------------------------------------------
// Name: HLA_DistantObject::InstanciateObject
// Created: AGE 2004-11-30
// -----------------------------------------------------------------------------
MIL_RealObject_ABC* HLA_DistantObject::InstanciateObject()
{
    const MIL_RealObjectType* pType = MIL_RealObjectType::FindObjectType( strObjectType_ );
    if( ! pType )
        return 0;
    MIL_Army* pArmy = MIL_AgentServer::GetWorkspace().GetEntityManager().FindArmy( strArmy_ );
    if( ! pArmy )
        return 0;
    if( localisation_.GetType() == TER_Localisation::eNone )
        return 0;

    MIL_RealObject_ABC& object = pType->InstanciateObject();
    object.Initialize( *pArmy, localisation_ );
    if( object.Initialize( strOption_, strExtra_, rConstructionPercentage_, rMiningPercentage_, rBypassPercentage_ ) )
    {
        object.SetHLAView( *this );
        return &object;
    }
    else
    {
        delete &object;
        return 0;
    }
}

// -----------------------------------------------------------------------------
// Name: HLA_DistantObject::DeserializeAttribute
// Created: AGE 2004-11-30
// -----------------------------------------------------------------------------
void HLA_DistantObject::DeserializeAttribute( const AttributeIdentifier& attributeID, Deserializer deserializer )
{
    if( attributeID == "type" )
        deserializer >> strObjectType_;
    else if( attributeID == "armee" )
        deserializer >> strArmy_;
    else if( attributeID == "option" )
        deserializer >> strOption_;
    else if( attributeID == "extra" )
        deserializer >> strExtra_;
    else if( attributeID == "coordonnees" )
        deserializer >> localisation_;
    else if( attributeID == "completion" )
        deserializer >> rConstructionPercentage_;
    else if( attributeID == "valorisation" )
        deserializer >> rMiningPercentage_;
    else if( attributeID == "contournement" )
        deserializer >> rBypassPercentage_;
}

// -----------------------------------------------------------------------------
// Name: HLA_DistantObject::Serialize
// Created: AGE 2004-11-30
// -----------------------------------------------------------------------------
void HLA_DistantObject::Serialize( UpdateFunctor_ABC&, bool ) const
{
    throw std::exception( "distant objects should not be serialized" );
}

// -----------------------------------------------------------------------------
// Name: HLA_DistantObject::Destroy
// Created: AGE 2004-11-30
// -----------------------------------------------------------------------------
void HLA_DistantObject::Destroy()
{
    if( pObject_ )
        pObject_->MarkForDestruction();
}

// -----------------------------------------------------------------------------
// Name: HLA_DistantObject::GetId
// Created: AGE 2004-12-06
// -----------------------------------------------------------------------------
ObjectIdentifier HLA_DistantObject::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: HLA_DistantObject::HideObject
// Created: AGE 2004-12-13
// -----------------------------------------------------------------------------
bool HLA_DistantObject::HideObject() const
{
    return true;
}
