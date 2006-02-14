// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "ObjectKnowledges.h"

// -----------------------------------------------------------------------------
// Name: ObjectKnowledges constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
ObjectKnowledges::ObjectKnowledges()
{

}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledges destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
ObjectKnowledges::~ObjectKnowledges()
{

}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledges::Update
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void ObjectKnowledges::Update( const ASN1T_MsgObjectKnowledgeCreation&    asnMsg )
{
    Gtia*& gtia = knowledgeGroups_[ asnMsg.oid_connaissance ];
    if( ! gtia )
    {
    }


    ObjectKnowledge* pObjectKnowledge = new ObjectKnowledge( asnMsg, *this );
    objectKnowledges_.insert( std::make_pair( pObjectKnowledge->GetID(), pObjectKnowledge ) );

    return true;
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledges::Update
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void ObjectKnowledges::Update( const ASN1T_MsgObjectKnowledgeUpdate&      asnMsg )
{
IT_ObjectKnowledgeMap itObjectKnowledge = objectKnowledges_.find( asnMsg.oid_connaissance );
    assert( itObjectKnowledge != objectKnowledges_.end() );

    itObjectKnowledge->second->Update( asnMsg );
//    App::GetApp().NotifyObjectKnowledgeUpdated( *this, *(itObjectKnowledge->second) );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledges::Update
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void ObjectKnowledges::Update( const ASN1T_MsgObjectKnowledgeDestruction& asnMsg )
{
    IT_ObjectKnowledgeMap itObjectKnowledge = objectKnowledges_.find( asnMsg.oid_connaissance );
    assert( itObjectKnowledge != objectKnowledges_.end() );
//    App::GetApp().NotifyObjectKnowledgeDeleted( *this, *(itObjectKnowledge->second) );
    Object_ABC::GetIDManagerForObjectType( itObjectKnowledge->second->GetObjectTypeID() ).ReleaseIdentifier( itObjectKnowledge->second->GetID() ) ;
    delete itObjectKnowledge->second;
    objectKnowledges_.erase( itObjectKnowledge );
}
