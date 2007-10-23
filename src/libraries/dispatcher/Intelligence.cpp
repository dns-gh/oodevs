// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "Intelligence.h"
#include "Network_Def.h"
#include "Formation.h"
#include "Model.h"

using namespace dispatcher;

namespace
{
    Formation& ResolveDiffusion( Model& model, const ASN1T_IntelligenceDiffusion& diffusion )
    {
        if( diffusion.t == T_IntelligenceDiffusion_formation )
            return model.GetFormations().Get( diffusion.u.formation );
        throw std::runtime_error( "Intelligence creation: unknown diffusion" ); // $$$$ SBO 2007-10-22: 
    }
}

// -----------------------------------------------------------------------------
// Name: Intelligence constructor
// Created: SBO 2007-10-22
// -----------------------------------------------------------------------------
Intelligence::Intelligence( Model& model, const ASN1T_MsgIntelligenceCreation& message )
    : formation_( ResolveDiffusion( model, message.intelligence.diffusion ) )
    , id_       ( message.oid )
    , name_     ( message.intelligence.name )
    , nature_   ( message.intelligence.nature )
    , embarked_ ( message.intelligence.embarked ? true : false )
    , level_    ( message.intelligence.level )
    , diplomacy_( message.intelligence.diplomacy )
    , position_ ( (const char*)message.intelligence.location.data, 15 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Intelligence destructor
// Created: SBO 2007-10-22
// -----------------------------------------------------------------------------
Intelligence::~Intelligence()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Intelligence::Update
// Created: SBO 2007-10-22
// -----------------------------------------------------------------------------
void Intelligence::Update( const ASN1T_MsgIntelligenceCreation& /*message*/ )
{
    FlagUpdate();
}

// -----------------------------------------------------------------------------
// Name: Intelligence::SendFullUpdate
// Created: SBO 2007-10-22
// -----------------------------------------------------------------------------
void Intelligence::SendFullUpdate( ClientPublisher_ABC& /*publisher*/ ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Intelligence::SendCreation
// Created: SBO 2007-10-22
// -----------------------------------------------------------------------------
void Intelligence::SendCreation( ClientPublisher_ABC& publisher ) const
{
    AsnMsgSimToClientIntelligenceCreation message;
    message().oid = id_;
    message().intelligence.name = name_.c_str();
    message().intelligence.nature = nature_.c_str();
    message().intelligence.embarked = embarked_ ? 1 : 0;
    message().intelligence.level = level_;
    message().intelligence.diplomacy = diplomacy_;
    message().intelligence.diffusion.t = T_IntelligenceDiffusion_formation;
    message().intelligence.diffusion.u.formation = formation_.GetID();
    message().intelligence.location = position_.c_str();
    message.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Intelligence::SendDestruction
// Created: SBO 2007-10-22
// -----------------------------------------------------------------------------
void Intelligence::SendDestruction( ClientPublisher_ABC& publisher ) const
{
    AsnMsgSimToClientIntelligenceDestruction message;
    message().oid = id_;
    message.Send( publisher );
}
