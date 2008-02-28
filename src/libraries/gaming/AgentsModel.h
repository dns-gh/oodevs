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

#include "game_asn/Simulation.h"
#include "clients_kernel/Resolver.h"

namespace kernel
{
    class Agent_ABC;
    class Automat_ABC;
    class Entity_ABC;
    class Population_ABC;
}

class AgentFactory_ABC;

namespace DIN
{
    class DIN_Input;
};


// =============================================================================
/** @class  AgentsModel
    @brief  AgentsModel
*/
// Created: AGE 2006-02-10
// =============================================================================
class AgentsModel : public kernel::Resolver< kernel::Agent_ABC >
                  , public kernel::Resolver< kernel::Automat_ABC >
                  , public kernel::Resolver< kernel::Population_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit AgentsModel( AgentFactory_ABC& agentFactory );
    virtual ~AgentsModel();
    //@}

    //! @name Operations
    //@{
    void CreateAutomat( const ASN1T_MsgAutomatCreation& asnMsg );
    kernel::Automat_ABC& GetAutomat( unsigned long id );
    kernel::Automat_ABC* FindAutomat( unsigned long id );

    void CreateAgent( const ASN1T_MsgUnitCreation& asnMsg );
    kernel::Agent_ABC& GetAgent( unsigned long id ) const;
    kernel::Agent_ABC* FindAgent( unsigned long id ) const;
    void DestroyAgent( const ASN1T_MsgUnitDestruction& asnMsg );

    kernel::Entity_ABC* FindAllAgent( unsigned long id ) const;

    void CreatePopulation( const ASN1T_MsgPopulationCreation& asnMsg );
    kernel::Population_ABC& GetPopulation( unsigned long id );
    kernel::Population_ABC* FindPopulation( unsigned long id );

    void Purge();
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
