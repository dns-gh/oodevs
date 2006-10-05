// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-08 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifndef __PopulationKnowledge_h_
#define __PopulationKnowledge_h_

#include "Types.h"
#include "Entities/KnowledgeGroup.h"

namespace TEST {

    class Population;
    class EntityManager;

// =============================================================================
// Created: SBO 2005-08-08
// =============================================================================
class PopulationKnowledge
{

public:
    //! @name Constructors/Destructor
    //@{
             PopulationKnowledge( const EntityManager&                        entityManager, 
                                  const ASN1T_MsgPopulationKnowledgeCreation& asnMsg, 
                                  const KnowledgeGroup&                       knowledgeGroup );
    virtual ~PopulationKnowledge();
    //@}

    //! @name Accessors
    //@{
    T_EntityId        GetId            () const;
    const Population* GetRealPopulation() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PopulationKnowledge( const PopulationKnowledge& );            //!< Copy constructor
    PopulationKnowledge& operator=( const PopulationKnowledge& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    T_EntityId            nId_;
    const KnowledgeGroup& owner_;
    Population*           pRealPopulation_;
    //@}
};

} // end namespace TEST

#include "PopulationKnowledge.inl"

#endif // __PopulationKnowledge_h_
