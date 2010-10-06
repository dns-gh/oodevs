// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************


#include "simulation_kernel_pch.h"
#include "HLA_DistantObject.h"
#include "HLA_InteractionManager_ABC.h"
#include "HLA_Serialization.h"
#include "HLA_Construction.h"
#include "HLA_Mining.h"
#include "HLA_Bypassing.h"
#include "HLA_Activation.h"
#include "MIL_AgentServer.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/MIL_Army.h"
#include "Entities/Objects/MIL_ObjectManager.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Objects/MIL_ObjectManipulator_ABC.h"
#include <hla/Deserializer.h>
#include <hla/AttributeIdentifier.h>
#include <boost/bind.hpp>

using namespace hla;

// -----------------------------------------------------------------------------
// Name: HLA_DistantObject constructor
// Created: AGE 2004-11-30
// -----------------------------------------------------------------------------
HLA_DistantObject::HLA_DistantObject( const ObjectIdentifier& objectId, HLA_InteractionManager_ABC& interactionManager )
    : pObject_               ( 0 )
    , constructionPercentage_( 0 )
    , miningPercentage_      ( 0 )
    , bypassPercentage_      ( 0 )
    , id_                    ( objectId )
    , interactionManager_    ( interactionManager )
    , isDestroying_          ( false )
{
    // NOTHING
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
//void HLA_DistantObject::Prepare()
//{
//    interactionManager_.Send( HLA_Activation( *this, false ) );
//}

// -----------------------------------------------------------------------------
// Name: HLA_DistantObject::Activate
// Created: AGE 2004-12-06
// -----------------------------------------------------------------------------
void HLA_DistantObject::Activate()
{
    assert( false );
//    interactionManager_.Send( HLA_Activation( *this, true ) );
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
        if( !name_.empty() && !army_.empty() )
            pObject_ = InstanciateObject();
    }
}

namespace
{
    void Resolve( const std::string& name, MIL_Army_ABC*& pArmy, const MIL_Army_ABC& army )
    {
        if( army.GetName() == name ) // $$$$ _RC_ SLI 2010-10-04: army name is unique?
            pArmy = const_cast< MIL_Army_ABC* >( &army ); // $$$$ _RC_ SLI 2010-10-04: dirty const_cast
    }
}

// -----------------------------------------------------------------------------
// Name: HLA_DistantObject::InstanciateObject
// Created: AGE 2004-11-30
// -----------------------------------------------------------------------------
MIL_Object_ABC* HLA_DistantObject::InstanciateObject()
{
    MIL_Army_ABC* pArmy = 0;
    MIL_AgentServer::GetWorkspace().GetEntityManager().GetArmies().Apply( boost::bind( &Resolve, boost::cref( army_ ), boost::ref( pArmy ), _1 ) );
    if( !pArmy )
        return 0;
    if( localisation_.GetType() == TER_Localisation::eNone )
        return 0;
    MIL_Object_ABC* pObject = MIL_AgentServer::GetWorkspace().GetEntityManager().CreateDistantObject( objectType_, *pArmy, localisation_, name_ );
    if( pObject )
    {
        MIL_Object_ABC& object = *pObject;
        if( object().CanBeConstructed() )
            object().Construct( constructionPercentage_ );
        if( object().CanBeMined() )
            object().Mine( miningPercentage_ );
        if( object().CanBeBypassed() )
            object().Bypass( bypassPercentage_ );
        pObject->SetHLAView( *this );
    }
    return pObject;
}

// -----------------------------------------------------------------------------
// Name: HLA_DistantObject::DeserializeAttribute
// Created: AGE 2004-11-30
// -----------------------------------------------------------------------------
void HLA_DistantObject::DeserializeAttribute( const AttributeIdentifier& attributeID, Deserializer deserializer )
{
    if( attributeID == "nom" )
        deserializer >> name_;
    else if( attributeID == "type" )
        deserializer >> objectType_;
    else if( attributeID == "armee" )
        deserializer >> army_;
    else if( attributeID == "option" )
        deserializer >> option_;
    else if( attributeID == "extra" )
        deserializer >> extra_;
    else if( attributeID == "coordonnees" )
        deserializer >> localisation_;
    else if( attributeID == "completion" )
        deserializer >> constructionPercentage_;
    else if( attributeID == "valorisation" )
        deserializer >> miningPercentage_;
    else if( attributeID == "contournement" )
        deserializer >> bypassPercentage_;
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
    isDestroying_ = true;
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
    return ! isDestroying_;
}
