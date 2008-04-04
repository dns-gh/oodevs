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

// -----------------------------------------------------------------------------
// Name: Intelligence constructor
// Created: SBO 2007-10-22
// -----------------------------------------------------------------------------
Intelligence::Intelligence( Model& model, const ASN1T_MsgIntelligenceCreation& message )
    : formation_( model.GetFormations().Get( message.intelligence.formation ) )
    , id_       ( message.oid )
    , name_     ( message.intelligence.name )
    , nature_   ( message.intelligence.nature )
    , embarked_ ( message.intelligence.embarked ? true : false )
    , level_    ( message.intelligence.level )
    , diplomacy_( message.intelligence.diplomacy )
    , position_ ( message.intelligence.location )
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
// Created: SBO 2007-10-23
// -----------------------------------------------------------------------------
void Intelligence::Update( const ASN1T_MsgIntelligenceUpdate& message )
{
    if( message.m.namePresent )
        name_ = message.name;
    if( message.m.naturePresent )
        nature_ = message.nature;
    if( message.m.embarkedPresent )
        embarked_ = message.embarked ? true : false;
    if( message.m.levelPresent )
        level_ = message.level;
    if( message.m.diplomacyPresent )
        diplomacy_ = message.diplomacy;
    if( message.m.locationPresent )
        position_ = message.location;
}

// -----------------------------------------------------------------------------
// Name: Intelligence::SendFullUpdate
// Created: SBO 2007-10-22
// -----------------------------------------------------------------------------
void Intelligence::SendFullUpdate( ClientPublisher_ABC& publisher ) const
{
    AsnMsgSimToClientIntelligenceUpdate message;
    message().oid       = id_;
    message().formation = formation_.GetID();
    message().m.namePresent      = 1; message().name      = name_.c_str();
    message().m.naturePresent    = 1; message().nature    = nature_.c_str();
    message().m.embarkedPresent  = 1; message().embarked  = embarked_ ? 1 : 0;
    message().m.levelPresent     = 1; message().level     = level_;
    message().m.diplomacyPresent = 1; message().diplomacy = diplomacy_;
    message().m.locationPresent  = 1; message().location  = position_;
    message.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Intelligence::SendCreation
// Created: SBO 2007-10-22
// -----------------------------------------------------------------------------
void Intelligence::SendCreation( ClientPublisher_ABC& publisher ) const
{
    AsnMsgSimToClientIntelligenceCreation message;
    message().oid = id_;
    message().intelligence.name      = name_.c_str();
    message().intelligence.nature    = nature_.c_str();
    message().intelligence.embarked  = embarked_ ? 1 : 0;
    message().intelligence.level     = level_;
    message().intelligence.diplomacy = diplomacy_;
    message().intelligence.formation = formation_.GetID();
    message().intelligence.location  = position_;
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
