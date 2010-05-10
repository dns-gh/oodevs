// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "script_plugin_pch.h"
#include "Actions.h"
#include "ModelAdapter.h"
#include "actions/Action_ABC.h"
#include "actions/ActionFactory.h"
#include "actions/ActionParameterFactory.h"
#include "actions/ActionsModel.h"
#include "clients_kernel/AgentKnowledgeConverter_ABC.h"
#include "clients_kernel/CoordinateConverter.h"
#include "clients_kernel/ObjectKnowledgeConverter_ABC.h"
#include "clients_kernel/Time_ABC.h"
#include "directia/Brain.h"
#include "dispatcher/Agent.h"
#include "dispatcher/Automat.h"
#include "dispatcher/AgentKnowledge.h"
#include "dispatcher/KnowledgeGroup.h"
#include "dispatcher/Model.h"
#include "dispatcher/ObjectKnowledge.h"
#include "dispatcher/PopulationKnowledge.h"
#include "dispatcher/SimulationPublisher_ABC.h"
#include "MT/MT_Logger/MT_Logger_lib.h"
#include "protocol/protocol.h"
#include "protocol/ServerPublisher_ABC.h"
#include "tools/ExerciseConfig.h"
#include <xeumeuleu/xml.h>

using namespace plugins::script;

namespace
{
    struct SimulationTime : public kernel::Time_ABC
    {
        virtual QDateTime GetDateTime() const { return QDateTime(); }
        virtual QDateTime GetInitialDateTime() const { return QDateTime(); }
        virtual unsigned int GetTickDuration() const { return 0; }
    };
}

// -----------------------------------------------------------------------------
// Name: Actions Publisher
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
struct Actions::Publisher : public Publisher_ABC
{
    explicit Publisher( dispatcher::SimulationPublisher_ABC& sim ) : sim_( &sim ) {}

    virtual void Send( const MsgsClientToSim::MsgClientToSim& message )
    {
        sim_->Send( message );
    }
    virtual void Send( const MsgsClientToAuthentication::MsgClientToAuthentication&  ) {}
    virtual void Send( const MsgsClientToReplay::MsgClientToReplay& ){}
    virtual void Send( const MsgsClientToAar::MsgClientToAar& ) {}
    virtual void Send( const MsgsClientToMessenger::MsgClientToMessenger& ) {}
    dispatcher::SimulationPublisher_ABC* sim_;
};

// -----------------------------------------------------------------------------
// Name: Actions::AgentConverter
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
struct Actions::AgentConverter : public kernel::AgentKnowledgeConverter_ABC
{
    //! @name Construction/Destruction
    //@{
    explicit AgentConverter( const dispatcher::Model& model )
        : model_( model )
    {}
    //@}

    //! @name Operations
    //@{
    virtual const kernel::AgentKnowledge_ABC* Find( const kernel::Agent_ABC& base, const kernel::Entity_ABC& owner )
    {
        const kernel::Entity_ABC& group = FindGroup( owner );
        tools::Iterator< const dispatcher::AgentKnowledge& > it = model_.agentKnowledges_.CreateIterator();
        while( it.HasMoreElements() )
        {
            const dispatcher::AgentKnowledge& k = it.NextElement();
            if( & k.knowledgeGroup_ == &group && &k.agent_ == &base )
                return &k;
        }
        return 0;
    }

    virtual const kernel::PopulationKnowledge_ABC* Find( const kernel::Population_ABC& base, const kernel::Entity_ABC& owner )
    {
        const kernel::Entity_ABC& group = FindGroup( owner );
        tools::Iterator< const dispatcher::PopulationKnowledge& > it = model_.populationKnowledges_.CreateIterator();
        while( it.HasMoreElements() )
        {
            const dispatcher::PopulationKnowledge& k = it.NextElement();
            if( & k.knowledgeGroup_ == &group && &k.population_ == &base )
                return &k;
        }
        return 0;
    }
    //@}

    //! @name Useless stuff
    //@{
    virtual const kernel::AgentKnowledge_ABC*      FindAgent( unsigned long , const kernel::Entity_ABC& ) { return 0; }
    virtual const kernel::PopulationKnowledge_ABC* FindPopulation( unsigned long , const kernel::Entity_ABC& ) { return 0; }
    virtual const kernel::AgentKnowledge_ABC*      Find( const kernel::AgentKnowledge_ABC& , const kernel::Entity_ABC& ) { return 0; };
    virtual const kernel::PopulationKnowledge_ABC* Find( const kernel::PopulationKnowledge_ABC& , const kernel::Entity_ABC& ) { return 0; };
    //@}

private:
    //! @name Helpers
    //@{
    AgentConverter& operator=( const AgentConverter& );
    const kernel::Entity_ABC& FindGroup( const kernel::Entity_ABC& owner )
    {
        if( const dispatcher::Automat* automat = dynamic_cast< const dispatcher::Automat* >( &owner ) )
            return automat->GetKnowledgeGroup();
        else if( const dispatcher::Agent* agent = dynamic_cast< const dispatcher::Agent* >( &owner ) )
            return agent->automat_->GetKnowledgeGroup();
        throw std::runtime_error( __FUNCTION__ );
    }
    //@}

    //! @name Member data
    //@{
    const dispatcher::Model& model_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: Actions::ObjectConverter
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
struct Actions::ObjectConverter : public kernel::ObjectKnowledgeConverter_ABC
{
    explicit ObjectConverter( const dispatcher::Model& model )
        : model_( model ) {}
    //! @name Operations
    //@{
    virtual const kernel::ObjectKnowledge_ABC* Find( const kernel::Object_ABC& base,  const kernel::Team_ABC& owner ) const
    {
        tools::Iterator< const dispatcher::ObjectKnowledge& > it = model_.objectKnowledges_.CreateIterator();
        while( it.HasMoreElements() )
        {
            const dispatcher::ObjectKnowledge& k = it.NextElement();
            if( & k.team_ == &owner && k.pObject_ == &base )
                return &k;
        }
        return 0;
    }
    //@}

    //! @name Useless stuff
    //@{
    virtual const kernel::ObjectKnowledge_ABC* Find( unsigned long , const kernel::Team_ABC& ) const { return 0; }
    virtual const kernel::ObjectKnowledge_ABC* Find( const kernel::ObjectKnowledge_ABC& , const kernel::Team_ABC& ) const { return 0; }
    //@}

private:
    //! @name Assignment
    //@{
    ObjectConverter& operator=( const ObjectConverter& );
    //@}

    //! @name Member data
    //@{
    const dispatcher::Model& model_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: Actions constructor
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
Actions::Actions( kernel::Controller& controller, const tools::ExerciseConfig& config, const dispatcher::Model& model, const kernel::StaticModel& staticModel, dispatcher::SimulationPublisher_ABC& sim )
    : entities_( new ModelAdapter( model ) )
    , publisher_( new Publisher( sim ) )
    , converter_( new kernel::CoordinateConverter( config ) )
    , time_( new SimulationTime() )
    , agentsKnowledges_( new AgentConverter( model ) )
    , objectsKnowledges_( new ObjectConverter( model ) )
    , parameters_( new actions::ActionParameterFactory( *converter_, *entities_, staticModel, *agentsKnowledges_, *objectsKnowledges_, controller ) )
    , factory_( new actions::ActionFactory( controller, *parameters_, *entities_, staticModel, *time_ ) )
    , file_   ( config.BuildExerciseChildFile( "scripts/resources/orders.ord" ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Actions destructor
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
Actions::~Actions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Actions::RegisterIn
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
void Actions::RegisterIn( directia::Brain& brain )
{
    brain.RegisterObject( "actions", this );
    brain.RegisterFunction( "IssueOrder", &Actions::IssueOrder );
    brain.RegisterFunction( "IssueXmlOrder", &Actions::IssueXmlOrder );
}

// -----------------------------------------------------------------------------
// Name: Actions::IssueOrder
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
void Actions::IssueOrder( const std::string& name )
{
    try
    {
        actions::ActionsModel model( *factory_, *publisher_ );
        model.Load( file_ );
        tools::Iterator< const actions::Action_ABC& > it = model.CreateIterator();
        while( it.HasMoreElements() )
        {
            const actions::Action_ABC& action = it.NextElement();
            if( action.GetName() == name.c_str() )
                action.Publish( *publisher_ );
        }
    }
    catch( std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Error in script: " << e.what() )
    }
}

// -----------------------------------------------------------------------------
// Name: Actions::IssueXmlOrder
// Created: AGE 2008-07-17
// -----------------------------------------------------------------------------
void Actions::IssueXmlOrder( const std::string& name )
{
    try
    {
        xml::xistringstream xis( name );
        xis >> xml::start( "action" );
        std::auto_ptr< actions::Action_ABC > action( factory_->CreateAction( xis ) );
        if( action.get() )
             action->Publish( *publisher_ );
    }
    catch( std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Error in script: " << e.what() )
    }
}
