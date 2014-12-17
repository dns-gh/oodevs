#include "simulation_kernel_pch.h"
#include "DEC_EntityFunctions.h"
#include "Decision/Brain.h"
#include "Decision/DEC_Decision_ABC.h"
#include "Entities/MIL_Entity_ABC.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"

void DEC_EntityFunctions::Register( sword::Brain& brain )
{
    brain.RegisterFunction( "DEC_IsAgent", &DEC_EntityFunctions::IsAgent );
    brain.RegisterFunction( "DEC_IsAutomaton", &DEC_EntityFunctions::IsAutomaton );
    brain.RegisterFunction( "DEC_IsCrowd", &DEC_EntityFunctions::IsCrowd ); 

    brain.RegisterFunction( "_DEC_DecisionalState", &DEC_EntityFunctions::DecisionalState );
}

bool DEC_EntityFunctions::IsAgent( const DEC_Decision_ABC* actor )
{
    return actor->GetKind() == DEC_Decision_ABC::ePion;
}

bool DEC_EntityFunctions::IsAutomaton( const DEC_Decision_ABC* actor )
{
    return actor->GetKind() == DEC_Decision_ABC::eAutomate;
}

bool DEC_EntityFunctions::IsCrowd( const DEC_Decision_ABC* actor )
{
    return actor->GetKind() == DEC_Decision_ABC::ePopulation;
}

void DEC_EntityFunctions::DecisionalState( const DEC_Decision_ABC* actor, const std::string& key, const std::string& value )
{
    if( actor )
    {
        client::DecisionalState msg;
        const auto id = actor->GetEntity().GetID();
        switch( actor->GetKind() )
        {
        case DEC_Decision_ABC::ePion:
            msg().mutable_source()->mutable_unit()->set_id( id );
        case DEC_Decision_ABC::eAutomate:
            msg().mutable_source()->mutable_automat()->set_id( id );
        case DEC_Decision_ABC::ePopulation:
            msg().mutable_source()->mutable_crowd()->set_id( id );
        }
        msg().set_key( key.c_str() );
        msg().set_value( value.c_str() );
        msg.Send( NET_Publisher_ABC::Publisher() );
    }
}
