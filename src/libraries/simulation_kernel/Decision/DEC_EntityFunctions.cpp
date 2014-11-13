#include "simulation_kernel_pch.h"
#include "DEC_EntityFunctions.h"
#include "DEC_Decision_ABC.h"
#include "Entities/MIL_Entity_ABC.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"

void DEC_EntityFunctions::DecisionalState( const DEC_Decision_ABC* agent, const std::string& key, const std::string& value )
{
    client::DecisionalState msg;
    msg().mutable_source()->mutable_crowd()->set_id( agent->GetEntity().GetID() );
    msg().set_key( key.c_str() );
    msg().set_value( value.c_str() );
    msg.Send( NET_Publisher_ABC::Publisher() );
}
 