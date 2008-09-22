// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "PopulationConcentrationKnowledge.h"
#include "Population.h"
#include "ClientPublisher_ABC.h"
#include "PopulationConcentration.h"
#include "ModelVisitor_ABC.h"
#include "EntityPublisher.h"
#include "clients_kernel/PopulationKnowledge_ABC.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: PopulationConcentrationKnowledge constructor
// Created: NLD 2006-10-03
// -----------------------------------------------------------------------------
PopulationConcentrationKnowledge::PopulationConcentrationKnowledge( const kernel::PopulationKnowledge_ABC& populationKnowledge, const ASN1T_MsgPopulationConcentrationKnowledgeCreation& msg )
    : SimpleEntity< >     ( msg.oid_connaissance_concentration )
    , populationKnowledge_( populationKnowledge )
    , pConcentration_     ( msg.oid_concentration_reelle == 0 ? 0 : &static_cast< const Population* >( populationKnowledge_.GetEntity() )->concentrations_.Get( msg.oid_concentration_reelle ) ) // $$$$ SBO 2008-07-11: 
    , position_           ( msg.position )
    , nNbrAliveHumans_    ( 0 )
    , nNbrDeadHumans_     ( 0 )
    , nAttitude_          ( EnumPopulationAttitude::agressive ) 
    , nRelevance_         ( 0 )
    , bPerceived_         ( false )
{
    optionals_.nb_humains_vivantsPresent = 0;
    optionals_.nb_humains_mortsPresent   = 0;
    optionals_.attitudePresent           = 0;
    optionals_.pertinencePresent         = 0;
    optionals_.est_percuPresent          = 0;
    Attach< EntityPublisher_ABC >( *new EntityPublisher< PopulationConcentrationKnowledge >( *this ) );
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentrationKnowledge destructor
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
PopulationConcentrationKnowledge::~PopulationConcentrationKnowledge()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentrationKnowledge::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationConcentrationKnowledge::Update( const ASN1T_MsgPopulationConcentrationKnowledgeUpdate& msg )
{
    if( msg.m.oid_concentration_reellePresent )
        pConcentration_ = msg.oid_concentration_reelle == 0 ? 0 : &static_cast< const Population* >( populationKnowledge_.GetEntity() )->concentrations_.Get( msg.oid_concentration_reelle ); // $$$$ SBO 2008-07-11: 

    if( msg.m.attitudePresent )
    {
        nAttitude_ = msg.attitude;
        optionals_.attitudePresent = 1;
    }
    if( msg.m.nb_humains_mortsPresent )
    {
        nNbrDeadHumans_ = msg.nb_humains_morts;
        optionals_.nb_humains_mortsPresent = 1;
    }
    if( msg.m.nb_humains_vivantsPresent )
    {
        nNbrAliveHumans_ = msg.nb_humains_vivants;
        optionals_.nb_humains_vivantsPresent = 1;
    }
    if( msg.m.pertinencePresent )
    {
        nRelevance_ = msg.pertinence;
        optionals_.pertinencePresent = 1;
    }
    if( msg.m.est_percuPresent )
    {
        bPerceived_ = msg.est_percu != 0;
        optionals_.est_percuPresent = 1;
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentrationKnowledge::SendCreation
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationConcentrationKnowledge::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::PopulationConcentrationKnowledgeCreation asn;

    asn().oid_connaissance_concentration = GetId();
    asn().oid_connaissance_population    = populationKnowledge_.GetId();
    asn().oid_concentration_reelle       = pConcentration_ ? pConcentration_->GetId() : 0;
    asn().oid_groupe_possesseur          = populationKnowledge_.GetOwner().GetId();
    asn().position = position_;

    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentrationKnowledge::SendFullUpdate
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void PopulationConcentrationKnowledge::SendFullUpdate( ClientPublisher_ABC& publisher ) const
{
    client::PopulationConcentrationKnowledgeUpdate asn;

    asn().oid_connaissance_concentration = GetId();
    asn().oid_connaissance_population    = populationKnowledge_.GetId();
    asn().oid_groupe_possesseur          = populationKnowledge_.GetOwner().GetId();

    asn().m.oid_concentration_reellePresent = 1;
    asn().oid_concentration_reelle          = pConcentration_ ? pConcentration_->GetId() : 0;

    if( optionals_.nb_humains_mortsPresent )
    {
        asn().m.nb_humains_mortsPresent = 1;
        asn().nb_humains_morts = nNbrDeadHumans_;
    }

    if( optionals_.nb_humains_vivantsPresent )
    {
        asn().m.nb_humains_vivantsPresent = 1;
        asn().nb_humains_vivants = nNbrAliveHumans_;
    }

    if( optionals_.attitudePresent )
    {
        asn().m.attitudePresent = 1;
        asn().attitude = nAttitude_;
    }

    if( optionals_.pertinencePresent )
    {
        asn().m.pertinencePresent = 1;
        asn().pertinence = nRelevance_;
    }

    if( optionals_.est_percuPresent )
    {
        asn().m.est_percuPresent = 1;
        asn().est_percu = bPerceived_;
    }

    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentrationKnowledge::SendDestruction
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
void PopulationConcentrationKnowledge::SendDestruction( ClientPublisher_ABC& publisher ) const
{
    client::PopulationConcentrationKnowledgeDestruction asn;
    asn().oid_connaissance_concentration = GetId();
    asn().oid_connaissance_population    = populationKnowledge_.GetId();
    asn().oid_groupe_possesseur          = populationKnowledge_.GetOwner().GetId();
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentrationKnowledge::Accept
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
void PopulationConcentrationKnowledge::Accept( ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}
