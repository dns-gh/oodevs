// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __Model_h_
#define __Model_h_

#include "client_proxy/SwordMessageHandler_ABC.h"
#include "clients_kernel/EntityResolver_ABC.h"
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

namespace kernel
{
    class StaticModel;
}

namespace sword
{
    class UnitCreation;
    class UnitKnowledgeCreation;
    class AutomatCreation;
}

namespace mission_tester
{
    class Agent;
    class Automat;
    class Scheduler_ABC;
    class Listener_ABC;

// =============================================================================
/** @class  Model
    @brief  Model
*/
// Created: PHC 2011-03-28
// =============================================================================
class Model : private boost::noncopyable
            , public SwordMessageHandler_ABC
            , public kernel::EntityResolver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Model( const kernel::StaticModel& staticModel, Scheduler_ABC& scheduler );
    virtual ~Model();
    //@}

    //! @name Operations
    //@{
    virtual void OnReceiveMessage( const sword::SimToClient& message );
    virtual void OnReceiveMessage( const sword::MessengerToClient& message );
    virtual void OnReceiveMessage( const sword::AuthenticationToClient& message );
    virtual void OnReceiveMessage( const sword::DispatcherToClient& message );

    void Register( Listener_ABC& listener );
    //@}

    //! @name Resolvers
    //@{
    virtual kernel::Agent_ABC* FindAgent( unsigned int id ) const;
    virtual kernel::Agent_ABC& GetAgent( unsigned int id ) const;
    virtual kernel::Automat_ABC* FindAutomat( unsigned int id ) const;
    virtual kernel::Automat_ABC& GetAutomat( unsigned int id ) const;
    virtual kernel::Inhabitant_ABC* FindInhabitant( unsigned int id ) const;
    virtual kernel::Inhabitant_ABC& GetInhabitant( unsigned int id ) const;
    virtual kernel::KnowledgeGroup_ABC* FindKnowledgeGroup( unsigned int id ) const;
    virtual kernel::KnowledgeGroup_ABC& GetKnowledgeGroup( unsigned int id ) const;
    virtual kernel::Object_ABC* FindObject( unsigned int id ) const;
    virtual kernel::Object_ABC& GetObject( unsigned int id ) const;
    virtual kernel::Object_ABC* FindUrbanObject( unsigned int id ) const;
    virtual kernel::Object_ABC& GetUrbanObject( unsigned int id ) const;
    virtual kernel::Population_ABC* FindPopulation( unsigned int id ) const;
    virtual kernel::Population_ABC& GetPopulation( unsigned int id ) const;
    virtual kernel::Formation_ABC* FindFormation( unsigned int id ) const;
    virtual kernel::Formation_ABC& GetFormation( unsigned int id ) const;
    virtual kernel::Team_ABC* FindTeam( unsigned int id ) const;
    virtual kernel::Team_ABC& GetTeam( unsigned int id ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void CreateAgent( const sword::UnitCreation& message );
    void CreateAgentKnowledge( const sword::UnitKnowledgeCreation& message );
    void CreateAutomat( const sword::AutomatCreation& message );
    //@}

    //! @name Types
    //@{
    typedef std::map< unsigned long, boost::shared_ptr< Agent > > T_Agents;
    typedef std::map< unsigned long, boost::shared_ptr< Automat > > T_Automats;
    //@}

private:
    //! @name Member data
    //@{
    std::vector< Listener_ABC* > listeners_;
    const kernel::StaticModel& staticModel_;
    T_Agents agents_;
    T_Automats automats_;
    Scheduler_ABC& scheduler_;
    //@}
};

}

#endif // __Model_h_
