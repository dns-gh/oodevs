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

#ifndef __PawnKnowledge_h_
#define __PawnKnowledge_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "Types.h"
#include "Entities/KnowledgeGroup.h"

namespace TEST {

    class Pawn;
    class EntityManager;

// =============================================================================
/** @class  PawnKnowledge
    @brief  PawnKnowledge
    @par    Using example
    @code
    PawnKnowledge;
    @endcode
*/
// Created: SBO 2005-08-08
// =============================================================================
class PawnKnowledge
{

public:
    //! @name Constructors/Destructor
    //@{
             PawnKnowledge( const EntityManager&                  entityManager, 
                            const ASN1T_MsgUnitKnowledgeCreation& asnMsg, 
                            const KnowledgeGroup&                 knowledgeGroup );
    virtual ~PawnKnowledge();
    //@}

    //! @name Accessors
    //@{
    T_EntityId  GetId      () const;
    const Pawn* GetRealPawn() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PawnKnowledge( const PawnKnowledge& );            //!< Copy constructor
    PawnKnowledge& operator=( const PawnKnowledge& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    T_EntityId            nId_;
    const KnowledgeGroup& owner_;
    Pawn*                 pRealPawn_;
    //@}
};

} // end namespace TEST

#include "PawnKnowledge.inl"

#endif // __PawnKnowledge_h_
