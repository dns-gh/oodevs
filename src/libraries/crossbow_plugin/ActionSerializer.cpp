// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "ActionSerializer.h"
#include "Database_ABC.h"
#include "Row_ABC.h"
#include "dispatcher/AgentKnowledgeConverter.h"
#include "dispatcher/ObjectKnowledgeConverter.h"
#include "dispatcher/ModelAdapter.h"
#include "dispatcher/Agent_ABC.h"
#include "dispatcher/Automat_ABC.h"
#include "actions/ActionFactory.h"
#include "actions/ActionParameterFactory.h"
#include "actions/ParameterFactory_ABC.h"
#include "actions/Action_ABC.h"
#include "actions/Parameter_ABC.h"
#include "clients_kernel/Time_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/StaticModel.h"
#include "clients_kernel/CoordinateConverter.h"
#include <xeumeuleu/xml.hpp>

using namespace plugins::crossbow;

namespace
{
    template< typename Type >
    Type GetField( const Row_ABC& row, const std::string& name )
    {
        return boost::get< Type >( row.GetField( name ) );
    }

    struct SimulationTime : public kernel::Time_ABC
    {
        virtual QDateTime GetDateTime() const { return QDateTime(); }
        virtual QDateTime GetInitialDateTime() const { return QDateTime(); }
        virtual unsigned int GetTickDuration() const { return 0; }
    };
}

// -----------------------------------------------------------------------------
// Name: ActionSerializer constructor
// Created: JCR 2010-12-07
// -----------------------------------------------------------------------------
ActionSerializer::ActionSerializer( const kernel::CoordinateConverter_ABC& converter, dispatcher::Model_ABC& model, const kernel::StaticModel& staticModel )
    : controller_       ( new kernel::Controller() )
    , entities_         ( new dispatcher::ModelAdapter( model ) )
    , converter_        ( converter )
    , time_             ( new SimulationTime() )
    , agentsKnowledges_ ( new dispatcher::AgentKnowledgeConverter( model ) )
    , objectsKnowledges_( new dispatcher::ObjectKnowledgeConverter( model ) )
    , parameters_       ( new actions::ActionParameterFactory( converter_, *entities_, staticModel, *agentsKnowledges_, *objectsKnowledges_, *controller_ ) )
    , factory_          ( new actions::ActionFactory( *controller_, *parameters_, *entities_, staticModel, *time_ ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionSerializer destructor
// Created: JCR 2010-12-07
// -----------------------------------------------------------------------------
ActionSerializer::~ActionSerializer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionSerializer::SerializeCreation
// Created: JCR 2011-01-19
// -----------------------------------------------------------------------------
void ActionSerializer::SerializeCreation( const Row_ABC& row, std::auto_ptr< actions::Action_ABC >& action ) const
{
    try
    {
        const std::string xmlMessage( GetField< std::string >( row, "data" ) );
        xml::xistringstream xis( xmlMessage );

        xis >> xml::start( "actions" )
                >> xml::list( "action", *this, &ActionSerializer::ReadAction, action )
            >> xml::end;


    }
    catch ( std::exception& e )
    {
        throw std::runtime_error( std::string( "Object creation failed:\n" ) + e.what() );
    }
}

// -----------------------------------------------------------------------------
// Name: ActionSerializer::ReadAction
// Created: JCR 2011-01-19
// -----------------------------------------------------------------------------
void ActionSerializer::ReadAction(  xml::xistream& xis, std::auto_ptr< actions::Action_ABC >& action ) const
{
    action.reset( factory_->CreateAction( xis/*, true*/ ) );
}
