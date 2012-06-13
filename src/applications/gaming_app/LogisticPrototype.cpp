// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "LogisticPrototype.h"
#include "clients_kernel/Automat_ABC.h"
#include "protocol/Protocol.h"
#include "actions/ParameterList.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: LogisticPrototype::LogisticPrototype
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
LogisticPrototype::LogisticPrototype( QWidget* parent, Controllers& controllers, actions::parameters::ParameterList*& attributesList )
    : LogisticPrototype_ABC( parent, controllers )
    , attributesList_( attributesList )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticPrototype::~LogisticPrototype
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
LogisticPrototype::~LogisticPrototype()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticPrototype::Commit
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
void LogisticPrototype::Commit( const kernel::Team_ABC& team )
{
    if( CheckValidity( team ) )
    {
        actions::parameters::ParameterList& list = attributesList_->AddList( "Logistic" );
        list.AddIdentifier( "AttributeId", sword::ObjectMagicAction::logistic );
        list.AddIdentifier( "tc2", logSuperiors_->GetValue()->GetId() ); //$$ tc2 � renommer ...
    }
}
