// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "NBCPrototype.h"
#include "clients_kernel/NBCAgent.h"
#include "clients_kernel/Object_ABC.h"
#include "preparation/NBCAttributes.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: NBCPrototype constructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
NBCPrototype::NBCPrototype( QWidget* parent, const Resolver_ABC< NBCAgent >& resolver, Object_ABC*& creation )
    : NBCPrototype_ABC( parent, resolver )
    , creation_( creation )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: NBCPrototype destructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
NBCPrototype::~NBCPrototype()
{
    Clean();
}

// -----------------------------------------------------------------------------
// Name: NBCPrototype::Commit
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void NBCPrototype::Commit()
{
    if( !creation_ )
        return;

    static_cast< NBCAttributes& >( creation_->Get< NBCAttributes_ABC >() ).SetAgent( *nbcAgents_->GetValue() );
}

// -----------------------------------------------------------------------------
// Name: NBCPrototype::Clean
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
void NBCPrototype::Clean()
{
    // NOTHING
}
