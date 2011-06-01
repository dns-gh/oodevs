// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __EntityResolverFacade_h_
#define __EntityResolverFacade_h_

#include "clients_kernel/EntityResolver_ABC.h"

class Model;

// =============================================================================
/** @class  EntityResolverFacade
    @brief  EntityResolverFacade
*/
// Created: SBO 2010-05-10
// =============================================================================
class EntityResolverFacade : public kernel::EntityResolver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit EntityResolverFacade( Model& model );
    virtual ~EntityResolverFacade();
    //@}

    //! @name Operations
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
    //! @name Copy/Assignment
    //@{
    EntityResolverFacade( const EntityResolverFacade& );            //!< Copy constructor
    EntityResolverFacade& operator=( const EntityResolverFacade& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    Model& model_;
    //@}
};

#endif // __EntityResolverFacade_h_
