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

#include <tools/Resolver.h>

namespace sword
{
    class AutomatCreation;
    class AutomatDestruction;
    class CrowdCreation;
    class CrowdDestruction;
    class PopulationCreation;
    class UnitCreation;
    class UnitDestruction;
}

namespace kernel
{
    class Agent_ABC;
    class Automat_ABC;
    class Entity_ABC;
    class Population_ABC;
    class PopulationPart_ABC;
    class Inhabitant_ABC;
}

class AgentFactory_ABC;

class PopulationPartResolver : public tools::Resolver_ABC< kernel::PopulationPart_ABC >
{
public:
    virtual kernel::PopulationPart_ABC* Find( const unsigned long& id ) const;
    virtual kernel::PopulationPart_ABC& Get ( const unsigned long& id ) const;
    virtual tools::Iterator< const kernel::PopulationPart_ABC& > CreateIterator() const;
    virtual kernel::PopulationPart_ABC* FindPopulationPart( const unsigned long& id ) const = 0;
};

// =============================================================================
/** @class  AgentsModel
    @brief  AgentsModel
*/
// Created: AGE 2006-02-10
// =============================================================================
class AgentsModel : public tools::Resolver< kernel::Agent_ABC >
                  , public tools::Resolver< kernel::Automat_ABC >
                  , public tools::Resolver< kernel::Population_ABC >
                  , public tools::Resolver< kernel::Inhabitant_ABC >
                  , public PopulationPartResolver
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit AgentsModel( AgentFactory_ABC& agentFactory );
    virtual ~AgentsModel();
    //@}

    //! @name Operations
    //@{
    void CreateAutomat( const sword::AutomatCreation& message );
    kernel::Automat_ABC& GetAutomat( unsigned long id );
    kernel::Automat_ABC* FindAutomat( unsigned long id );
    void DestroyAutomat( const sword::AutomatDestruction& message );

    void CreateAgent( const sword::UnitCreation& message );
    kernel::Agent_ABC& GetAgent( unsigned long id ) const;
    kernel::Agent_ABC* FindAgent( unsigned long id ) const;
    void DestroyAgent( const sword::UnitDestruction& message );

    kernel::Entity_ABC* FindAllAgent( unsigned long id ) const;

    void CreatePopulation( const sword::CrowdCreation& message );
    kernel::Population_ABC& GetPopulation( unsigned long id );
    kernel::Population_ABC* FindPopulation( unsigned long id );
    void DestroyCrowd( const sword::CrowdDestruction& message );

    virtual kernel::PopulationPart_ABC* FindPopulationPart( const unsigned long& id ) const;

    void CreateInhabitant( const sword::PopulationCreation& message );
    kernel::Inhabitant_ABC& GetInhabitant( unsigned long id );
    kernel::Inhabitant_ABC* FindInhabitant( unsigned long id );

    void Purge();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AgentsModel( const AgentsModel& );            //!< Copy constructor
    AgentsModel& operator=( const AgentsModel& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    AgentFactory_ABC& agentFactory_;
    //@}
};

#endif // __AgentsModel_h_
