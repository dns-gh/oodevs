#ifndef __MockMIL_Mission_ABC_h_
#define __MockMIL_Mission_ABC_h_

#include "Entities/Orders/MIL_ParameterType_ABC.h"
#include "Entities/Orders/MIL_Mission_ABC.h"
#include "Entities/Orders/MIL_MissionParameterVisitor_ABC.h"
#include "Entities/Orders/MIL_OrderTypeParameter.h"
#include "StubMIL_MissionParameter_ABC.h"

class ActionManager;

class StubMIL_Mission_ABC : public MIL_Mission_ABC
{
public:
    StubMIL_Mission_ABC( const MIL_MissionType_ABC& type, const DEC_KnowledgeResolver_ABC& knowledgeResolver, const std::string& param ="" )
        : MIL_Mission_ABC( type, knowledgeResolver, 0, boost::shared_ptr< MIL_Mission_ABC >() )
        , param_( param )
    {}

    virtual void Start( boost::shared_ptr< MIL_Mission_ABC > self, ActionManager& ){};
    virtual void Stop( boost::shared_ptr< MIL_Mission_ABC > self ){};
    virtual bool IsFragOrderAvailable( const MIL_FragOrderType& /*fragOrderType*/ ) const
    {
        return false;
    }
    virtual void Send( ActionManager& ) const {};
    virtual void Visit( MIL_MissionParameterVisitor_ABC& parameterVisitor ) const
    {
        const MIL_ParameterType_ABC* pType = MIL_ParameterType_ABC::Find( "string" );
        if( !pType )
        {
            MIL_ParameterType_ABC::Initialize();
            pType = MIL_ParameterType_ABC::Find( "string" );
        }
        xml::xistringstream xisParam("<parameter dia-name='pointAReconnaitre_' name='Point a reconnaitre' optional='false' type='string'/>");
        xisParam >> xml::start( "parameter" );
        MIL_OrderTypeParameter orderType( xisParam );
        xisParam >> xml::end;
        StubMIL_MissionParameter_ABC param( param_ );
        parameterVisitor.Accept( "parameter", orderType, param );
    }

private:
    std::string param_;

};
#endif
