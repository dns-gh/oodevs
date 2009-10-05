#ifndef __MockMIL_Mission_ABC_h_
#define __MockMIL_Mission_ABC_h_

#include "simulation_orders/MIL_ParameterType_ABC.h"
#include "Entities/Orders/MIL_Mission_ABC.h"
#include "Entities/Orders/MIL_MissionParameterVisitor_ABC.h"
#include "StubMIL_MissionParameter_ABC.h"

class StubMIL_Mission_ABC 
    : public MIL_Mission_ABC
{
public:
    StubMIL_Mission_ABC( const MIL_MissionType_ABC& type, const DEC_KnowledgeResolver_ABC& knowledgeResolver, const std::string& param ="" )
        : MIL_Mission_ABC( type, knowledgeResolver )
        , param_( param )
    {}

    virtual void Start               () {}
    virtual bool IsFragOrderAvailable( const MIL_FragOrderType& fragOrderType ) const 
    {
        return false;
    }

    virtual void Visit( MIL_MissionParameterVisitor_ABC& parameterVisitor ) const
    {
        const MIL_ParameterType_ABC* pType = MIL_ParameterType_ABC::Find( "string" );
        if( !pType )
        {
            MIL_ParameterType_ABC::Initialize();
            pType = MIL_ParameterType_ABC::Find( "string" );
        }
        StubMIL_MissionParameter_ABC param( param_ );
        parameterVisitor.Accept( "parameter", *pType, param );
    }

private:
    std::string param_;

};

#endif