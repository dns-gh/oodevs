#include "simulation_kernel_pch.h"
#include "DEC_EntityFunctions.h"
#include "DEC_Decision_ABC.h"
#include "Entities/MIL_Entity_ABC.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"

void DEC_EntityFunctions::DecisionalState( const DEC_Decision_ABC* agent, const std::string& key, const std::string& value )
{
    if( agent )
    {
        client::DecisionalState msg;
        const auto id = agent->GetEntity().GetID();
        switch( agent->GetKind() )
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
