// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __EntityResolverFacade_h_
#define __EntityResolverFacade_h_

#include "EntityResolver_ABC.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
class Model_ABC;

// =============================================================================
/** @class  EntityResolverFacade
    @brief  EntityResolverFacade
*/
// Created: SBO 2010-05-10
// =============================================================================
class EntityResolverFacade : public EntityResolver_ABC
                           , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit EntityResolverFacade( Model_ABC& model );
    virtual ~EntityResolverFacade();
    //@}

    //! @name Operations
    //@{
    virtual Agent_ABC* FindAgent( unsigned int id ) const;
    virtual Agent_ABC& GetAgent( unsigned int id ) const;
    virtual Automat_ABC* FindAutomat( unsigned int id ) const;
    virtual Automat_ABC& GetAutomat( unsigned int id ) const;
    virtual Inhabitant_ABC* FindInhabitant( unsigned int id ) const;
    virtual Inhabitant_ABC& GetInhabitant( unsigned int id ) const;
    virtual KnowledgeGroup_ABC* FindKnowledgeGroup( unsigned int id ) const;
    virtual KnowledgeGroup_ABC& GetKnowledgeGroup( unsigned int id ) const;
    virtual Object_ABC* FindObject( unsigned int id ) const;
    virtual Object_ABC& GetObject( unsigned int id ) const;
    virtual Population_ABC* FindPopulation( unsigned int id ) const;
    virtual Population_ABC& GetPopulation( unsigned int id ) const;
    virtual Formation_ABC* FindFormation( unsigned int id ) const;
    virtual Formation_ABC& GetFormation( unsigned int id ) const;
    virtual Team_ABC* FindTeam( unsigned int id ) const;
    virtual Team_ABC& GetTeam( unsigned int id ) const;
    virtual UrbanObject_ABC* FindUrbanObject( unsigned int id ) const;
    virtual UrbanObject_ABC& GetUrbanObject( unsigned int id ) const;

    virtual Entity_ABC* FindEntity( unsigned int id ) const;
    //@}

private:
    //! @name Member data
    //@{
    Model_ABC& model_;
    //@}
};

}

#endif // __EntityResolverFacade_h_
