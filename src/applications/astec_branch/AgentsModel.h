// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentsModel_h_
#define __AgentsModel_h_

#include "ASN_Types.h"
#include "Resolver.h"

class Agent;
class Agent_ABC;
class AgentFactory_ABC;
class ObjectFactory_ABC;
class Team;
class LogMaintenanceConsign;
class LogSupplyConsign;
class LogMedicalConsign;
class KnowledgeGroup;
class Object_ABC;
class App;
class Population;
class TacticalLine_ABC;

namespace DIN {
    class DIN_Input;
};


// =============================================================================
/** @class  AgentsModel
    @brief  AgentsModel
*/
// Created: AGE 2006-02-10
// =============================================================================
class AgentsModel : public Resolver< Agent >
                  , public Resolver< Population >
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentsModel( AgentFactory_ABC& agentFactory );
    virtual ~AgentsModel();
    //@}

    //! @name Operations
    //@{
    void CreateAgent( const ASN1T_MsgAutomateCreation& asnMsg );
    void CreateAgent( const ASN1T_MsgPionCreation& asnMsg );
    Agent& GetAgent( unsigned long id ) const;
    Agent* FindAgent( unsigned long id ) const;

//    template< typename Functor >
//    void ApplyOnAgents( const Functor& f ) {
//        Resolver< Agent >::Apply( f );
//    }

    Agent_ABC* FindAllAgent( unsigned long id ) const;

    void CreatePopulation( const ASN1T_MsgPopulationCreation& asnMsg );
    Population& GetPopulation( unsigned long id );
    Population* FindPopulation( unsigned long id );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AgentsModel( const AgentsModel& );            //!< Copy constructor
    AgentsModel& operator=( const AgentsModel& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    AgentFactory_ABC& agentFactory_;
    //@}
};

#endif // __AgentsModel_h_
