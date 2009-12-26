// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "UrbanKnowledge.h"

#include "Automat.h"
#include "ClientPublisher_ABC.h"
#include "Model_ABC.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "Side.h"
#include "UrbanObject.h"


namespace dispatcher
{

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge constructor
// Created: MGD 2009-12-11
// -----------------------------------------------------------------------------
UrbanKnowledge::UrbanKnowledge( const Model_ABC& model, const ASN1T_MsgUrbanKnowledgeCreation& asnMsg )
    : SimpleEntity< kernel::UrbanKnowledge_ABC >( asnMsg.oid )
    , model_                        ( model )
    , team_                         ( model.Sides().Get( asnMsg.team ) )
    , pUrban_                       ( model.UrbanBlocks().Find( asnMsg.real_urban ) )
    , bPerceived_                   ( false )
    , automatPerceptions_           ()    
{
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge destructor
// Created: MGD 2009-12-11
// -----------------------------------------------------------------------------
UrbanKnowledge::~UrbanKnowledge()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge::Update
// Created: MGD 2009-12-11
// -----------------------------------------------------------------------------
void UrbanKnowledge::Update( const ASN1T_MsgUrbanKnowledgeCreation& message )
{
    bool realUrbanChanged = ( message.real_urban && ! pUrban_ )
                          || ( pUrban_ && pUrban_->GetId() != message.real_urban );
    if( realUrbanChanged )
        pUrban_ = model_.UrbanBlocks().Find( message.real_urban );

    ApplyUpdate( message );
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge::Update
// Created: MGD 2009-12-11
// -----------------------------------------------------------------------------
void UrbanKnowledge::Update( const ASN1T_MsgUrbanKnowledgeUpdate& asnMsg )
{  
    if( asnMsg.m.automat_perceptionPresent )
    {
        optionals_.automat_perceptionPresent = 1;
        automatPerceptions_.clear();
        for( unsigned int i = 0; i < asnMsg.automat_perception.n; ++i )
            automatPerceptions_.push_back( &model_.Automats().Get( asnMsg.automat_perception.elem[ i ]) );
    }

    if( asnMsg.m.real_urbanPresent )
        pUrban_ = model_.UrbanBlocks().Find( asnMsg.real_urban );

    if( asnMsg.m.perceivedPresent )
    {
        optionals_.perceivedPresent = 1;
        bPerceived_ = asnMsg.perceived;
    }
    if( asnMsg.m.identification_levelPresent)
    {
        optionals_.identification_levelPresent = 1;
        nIdentificationLevel_ = asnMsg.identification_level;
    }
    if( asnMsg.m.relevancePresent )
    {
        optionals_.relevancePresent = 1;
        nRelevance_ = asnMsg.relevance;
    }
    if( asnMsg.m.progressPresent )
    {
        optionals_.progressPresent = 1;
        nProgress_ = asnMsg.progress; 
    }

}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge::SendCreation
// Created: MGD 2009-12-11
// -----------------------------------------------------------------------------
void UrbanKnowledge::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::UrbanKnowledgeCreation asn;

    asn().oid         = GetId();
    asn().team        = team_.GetId();
    asn().real_urban  = pUrban_ ? pUrban_->GetId() : 0;

    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge::SendFullUpdate
// Created: MGD 2009-12-11
// -----------------------------------------------------------------------------
void UrbanKnowledge::SendFullUpdate( ClientPublisher_ABC& publisher ) const
{
    client::UrbanKnowledgeUpdate asn;

    asn().oid  = GetId();
    asn().team = team_.GetId();

    asn().m.real_urbanPresent = 1;
    asn().real_urban          = pUrban_ ? pUrban_->GetId() : 0;

    if( optionals_.automat_perceptionPresent )
    {
        asn().m.automat_perceptionPresent = 1;
        asn().automat_perception.n = automatPerceptions_.size();
        asn().automat_perception.elem = asn().automat_perception.n > 0 ? new ASN1T_OID[ asn().automat_perception.n ] : 0;
        unsigned int i = 0;
        for( std::vector< const kernel::Automat_ABC* >::const_iterator it = automatPerceptions_.begin(); it != automatPerceptions_.end(); ++it, ++i )
            asn().automat_perception.elem[i] = (*it)->GetId();
    }
    if( optionals_.perceivedPresent )
    {
        asn().m.perceivedPresent = 1;
        asn().perceived = 1;
    }
    if( optionals_.relevancePresent )
    {
        asn().m.relevancePresent = 1; 
        asn().relevance = nRelevance_;
    }
    if( optionals_.identification_levelPresent )
    {
        asn().m.identification_levelPresent = 1;
        asn().identification_level = nIdentificationLevel_;
    }
    if( optionals_.progressPresent )
    {
        asn().m.progressPresent = 1;
        asn().progress = nProgress_;
    }

    asn.Send( publisher );

    if( asn().m.automat_perceptionPresent && asn().automat_perception.n > 0 )
        delete [] asn().automat_perception.elem;
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge::SendDestruction
// Created: MGD 2009-12-11
// -----------------------------------------------------------------------------
void UrbanKnowledge::SendDestruction( ClientPublisher_ABC& publisher ) const
{
    client::UrbanKnowledgeDestruction asn;
    asn().oid  = GetId();
    asn().team = team_.GetId();
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge::Accept
// Created: MGD 2009-12-11
// -----------------------------------------------------------------------------
void UrbanKnowledge::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge::GetRecognizedEntity
// Created: MGD 2009-12-11
// -----------------------------------------------------------------------------
const kernel::Entity_ABC* UrbanKnowledge::GetRecognizedEntity() const
{
    return pUrban_;
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge::GetEntity
// Created: MGD 2009-12-11
// -----------------------------------------------------------------------------
const UrbanObject* UrbanKnowledge::GetEntity() const
{
    return pUrban_;
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge::GetOwner
// Created: MGD 2009-12-11
// -----------------------------------------------------------------------------
const kernel::Team_ABC& UrbanKnowledge::GetOwner() const
{
    return team_;
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge::Display
// Created: MGD 2009-12-11
// -----------------------------------------------------------------------------
void UrbanKnowledge::Display( kernel::Displayer_ABC& ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge::DisplayInList
// Created: MGD 2009-12-11
// -----------------------------------------------------------------------------
void UrbanKnowledge::DisplayInList( kernel::Displayer_ABC& ) const
{
    // NOTHING
}

} // namespace dispatcher
