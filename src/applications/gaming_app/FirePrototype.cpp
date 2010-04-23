// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "FirePrototype.h"
#include "clients_kernel/FireClass.h"
#include "protocol/Protocol.h"
#include "tools/Iterator.h"
#include "actions/ParameterList.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: FirePrototype constructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
FirePrototype::FirePrototype( QWidget* parent, const tools::Resolver_ABC< FireClass >& resolver, actions::parameters::ParameterList*& attributesList )
    : FirePrototype_ABC( parent, resolver )
    , attributesList_( attributesList )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: FirePrototype destructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
FirePrototype::~FirePrototype()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FirePrototype::Commit
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void FirePrototype::Commit()
{
    if( const kernel::FireClass* fire = fireClass_->GetValue() )
    {
        actions::parameters::ParameterList& list = attributesList_->AddList( "Fire" );
        list.AddIdentifier( "AttributeId", MsgsClientToSim::MsgObjectMagicAction_Attribute_fire );
        list.AddIdentifier( "ClassId", fire->GetId() );
        list.AddQuantity( "Heat", fire->GetDefaultHeat() );
    }
}
