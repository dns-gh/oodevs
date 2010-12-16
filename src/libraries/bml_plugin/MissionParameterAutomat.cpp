// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "bml_plugin_pch.h"
#include "MissionParameterAutomat.h"
#include "dispatcher/Automat_ABC.h"

using namespace plugins::bml;

// -----------------------------------------------------------------------------
// Name: MissionParameterAutomat constructor
// Created: SBO 2008-06-02
// -----------------------------------------------------------------------------
MissionParameterAutomat::MissionParameterAutomat( const kernel::OrderParameter& type, const dispatcher::Automat_ABC& automat )
    : MissionParameter_ABC( type )
    , automat_( automat )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameterAutomat destructor
// Created: SBO 2008-06-02
// -----------------------------------------------------------------------------
MissionParameterAutomat::~MissionParameterAutomat()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameterAutomat::Serialize
// Created: SBO 2008-06-02
// -----------------------------------------------------------------------------
void MissionParameterAutomat::Serialize( sword::MissionParameter& parameter ) const
{
    parameter.set_null_value( false );
    parameter.mutable_value()->Add()->mutable_automat()->set_id( automat_.GetId() );
}
