// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "vrforces_plugin_pch.h"
#include "ForceResolver.h"
#include "clients_kernel/Karma.h"
#undef max
#include "dispatcher/Agent_ABC.h"
#include "dispatcher/Automat_ABC.h"
#include "dispatcher/Model_ABC.h"
#include "dispatcher/Team_ABC.h"

using namespace plugins::vrforces;

// -----------------------------------------------------------------------------
// Name: ForceResolver constructor
// Created: SBO 2011-03-16
// -----------------------------------------------------------------------------
ForceResolver::ForceResolver( const dispatcher::Model_ABC& model )
    : model_( model )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ForceResolver destructor
// Created: SBO 2011-03-16
// -----------------------------------------------------------------------------
ForceResolver::~ForceResolver()
{
    // NOTHING
}

namespace
{
    DtForceType GetForce( const kernel::Karma& karma )
    {
        if( karma == kernel::Karma::friend_ )
            return DtForceFriendly;
        if( karma == kernel::Karma::enemy_ )
            return DtForceOpposing;
        return DtForceNeutral;
    }
}

// -----------------------------------------------------------------------------
// Name: ForceResolver::Resolve
// Created: SBO 2011-03-16
// -----------------------------------------------------------------------------
DtForceType ForceResolver::Resolve( unsigned long identifier ) const
{
    return GetForce( model_.Agents().Get( identifier ).GetSuperior().GetTeam().GetKarma() );
}
