// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __kernel_EntityResolver_ABC_h_
#define __kernel_EntityResolver_ABC_h_

namespace kernel
{
    class Agent_ABC;
    class Automat_ABC;
    class Entity_ABC;
    class Formation_ABC;
    class Inhabitant_ABC;
    class KnowledgeGroup_ABC;
    class Object_ABC;
    class Population_ABC;
    class Team_ABC;
    class UrbanObject_ABC;

// =============================================================================
/** @class  EntityResolver_ABC
    @brief  EntityResolver_ABC
*/
// Created: SBO 2010-05-07
// =============================================================================
class EntityResolver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             EntityResolver_ABC() {}
    virtual ~EntityResolver_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual kernel::Agent_ABC* FindAgent( unsigned int id ) const = 0;
    virtual kernel::Agent_ABC& GetAgent( unsigned int id ) const = 0;
    virtual kernel::Automat_ABC* FindAutomat( unsigned int id ) const = 0;
    virtual kernel::Automat_ABC& GetAutomat( unsigned int id ) const = 0;
    virtual kernel::Inhabitant_ABC* FindInhabitant( unsigned int id ) const = 0;
    virtual kernel::Inhabitant_ABC& GetInhabitant( unsigned int id ) const = 0;
    virtual kernel::KnowledgeGroup_ABC* FindKnowledgeGroup( unsigned int id ) const = 0;
    virtual kernel::KnowledgeGroup_ABC& GetKnowledgeGroup( unsigned int id ) const = 0;
    virtual kernel::Object_ABC* FindObject( unsigned int id ) const = 0;
    virtual kernel::Object_ABC& GetObject( unsigned int id ) const = 0;
    virtual kernel::Population_ABC* FindPopulation( unsigned int id ) const = 0;
    virtual kernel::Population_ABC& GetPopulation( unsigned int id ) const = 0;
    virtual kernel::Formation_ABC* FindFormation( unsigned int id ) const = 0;
    virtual kernel::Formation_ABC& GetFormation( unsigned int id ) const = 0;
    virtual kernel::Team_ABC* FindTeam( unsigned int id ) const = 0;
    virtual kernel::Team_ABC& GetTeam( unsigned int id ) const = 0;
    virtual kernel::UrbanObject_ABC* FindUrbanObject( unsigned int id ) const = 0;
    virtual kernel::UrbanObject_ABC& GetUrbanObject( unsigned int id ) const = 0;

    virtual kernel::Entity_ABC* FindEntity( unsigned int id ) const = 0;
    //@}
};

}

#endif // __kernel_EntityResolver_ABC_h_
