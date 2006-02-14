// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "ObjectKnowledgeAutomataPerception.h"

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeAutomataPerception constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
ObjectKnowledgeAutomataPerception::ObjectKnowledgeAutomataPerception( const Resolver_ABC< Agent >& resolver )
    : resolver_( resolver )
    , set_( false )
{

}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeAutomataPerception destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
ObjectKnowledgeAutomataPerception::~ObjectKnowledgeAutomataPerception()
{

}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeAutomataPerception::Update
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void ObjectKnowledgeAutomataPerception::Update( const ASN1T_MsgObjectKnowledgeUpdate& message )
{
    if( message.m.perception_par_compagniePresent )
    {
        set_ = true;
        detectingAutomats_.clear();
        for( uint i = 0; i < message.perception_par_compagnie.n; ++i )
            detectingAutomats_.insert( & resolver_.Get( message.perception_par_compagnie.elem[i] ) );
    }
}
