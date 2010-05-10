// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ModelAdapter_h_
#define __ModelAdapter_h_

#include "clients_kernel/EntityResolver_ABC.h"

namespace dispatcher
{
    class Model;
}

namespace plugins
{
namespace script
{

// =============================================================================
/** @class  ModelAdapter
    @brief  ModelAdapter
*/
// Created: SBO 2010-05-10
// =============================================================================
class ModelAdapter : public kernel::EntityResolver_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ModelAdapter( const dispatcher::Model& model );
    virtual ~ModelAdapter();
    //@}

    //! @name Operations
    //@{
    virtual kernel::Agent_ABC* FindAgent( unsigned int id ) const;
    virtual kernel::Agent_ABC& GetAgent( unsigned int id ) const;
    virtual kernel::Automat_ABC* FindAutomat( unsigned int id ) const;
    virtual kernel::Automat_ABC& GetAutomat( unsigned int id ) const;
    virtual kernel::KnowledgeGroup_ABC* FindKnowledgeGroup( unsigned int id ) const;
    virtual kernel::KnowledgeGroup_ABC& GetKnowledgeGroup( unsigned int id ) const;
    virtual kernel::Object_ABC* FindObject( unsigned int id ) const;
    virtual kernel::Object_ABC& GetObject( unsigned int id ) const;
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
    ModelAdapter( const ModelAdapter& );            //!< Copy constructor
    ModelAdapter& operator=( const ModelAdapter& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const dispatcher::Model& model_;
    //@}
};

}
}

#endif // __ModelAdapter_h_
