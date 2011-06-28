// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __Model_ABC_h_
#define __Model_ABC_h_

#include <tools/Resolver_ABC.h>

namespace kernel
{
class Agent_ABC;

// =============================================================================
/** @class  Model_ABC
    @brief  Model_ABC
*/
// Created: JSR 2011-06-28
// =============================================================================
class Model_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Model_ABC() {}
    virtual ~Model_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual tools::Resolver_ABC< Team_ABC >& GetTeamResolver() const = 0;
    virtual tools::Resolver_ABC< Formation_ABC >& GetFormationResolver() const = 0;
    virtual tools::Resolver_ABC< Automat_ABC >& GetAutomatResolver() const = 0;
    virtual tools::Resolver_ABC< Agent_ABC >& GetAgentResolver() const = 0;
    virtual tools::Resolver_ABC< KnowledgeGroup_ABC >& GetKnowledgeGroupResolver() const = 0;
    virtual tools::Resolver_ABC< Object_ABC >& GetObjectResolver() const = 0;
    virtual tools::Resolver_ABC< Population_ABC >& GetPopulationResolver() const = 0;
    virtual tools::Resolver_ABC< Inhabitant_ABC >& GetInhabitantResolver() const = 0;
    // $$$$ _RC_ JSR 2011-06-28: Passer TerrainObjectProxy dans clients_kernel pour remplacer ces méthodes
    virtual Object_ABC* FindUrbanObject( unsigned int id ) const = 0;
    virtual Object_ABC& GetUrbanObject( unsigned int id ) const = 0;
    //@}
};

} // end namespace kernel

#endif // __Model_ABC_h_
