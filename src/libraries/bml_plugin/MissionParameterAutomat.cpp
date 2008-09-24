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
#include "dispatcher/Automat.h"

using namespace plugins::bml;

// -----------------------------------------------------------------------------
// Name: MissionParameterAutomat constructor
// Created: SBO 2008-06-02
// -----------------------------------------------------------------------------
MissionParameterAutomat::MissionParameterAutomat( const kernel::OrderParameter& type, const dispatcher::Automat& automat )
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
void MissionParameterAutomat::Serialize( ASN1T_MissionParameter& parameter ) const
{
    parameter.null_value = 0;
    parameter.value.t = T_MissionParameter_value_automat;
    parameter.value.u.automat = automat_.GetId();
}

// -----------------------------------------------------------------------------
// Name: MissionParameterAutomat::Clean
// Created: SBO 2008-06-02
// -----------------------------------------------------------------------------
void MissionParameterAutomat::Clean( ASN1T_MissionParameter& ) const
{
    // NOTHING
}
