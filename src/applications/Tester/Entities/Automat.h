// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-11 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Automat.h $
// $Author: Sbo $
// $Modtime: 21/07/05 17:03 $
// $Revision: 5 $
// $Workfile: Automat.h $
//
// *****************************************************************************

#ifndef __Automat_h_
#define __Automat_h_

#include "Types.h"
#include "Messages/ASN_Messages.h"

namespace TEST
{
    class EntityManager;
    class KnowledgeGroup;
    class Team;
    class Pawn;

// =============================================================================
/** @class  Automat
    @brief  Automat
    @par    Using example
    @code
    Automat;
    @endcode
*/
// Created: SBO 2005-05-11
// =============================================================================
class Automat
{
    friend class Pawn;

public:
    //! @name Constructors/Destructor
    //@{
                    Automat( const ASN1T_MsgAutomateCreation& asnMsg );
    virtual        ~Automat();
    //@}

    //! @name Operations
    //@{
    static       void     Initialize         ();
    static       void     Terminate          ();
    static       Automat* Find               ( T_EntityId nId );
    static       void     Register           ( Automat& automat );
    static       void     Unregister         ( Automat& automat );
    //@}

    //! @name Accessors
    //@{
          T_EntityId      GetId              () const;
	const Team&           GetTeam            () const;
	const KnowledgeGroup& GetKnowledgeGroup  () const;
	const Pawn&	          GetPc              () const;
    //@}

    //! @name Operations
    //@{
          void            OnKnowledgeGroupChanged( KnowledgeGroup& knowledgeGroup );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Automat( const Automat& );            //!< Copy constructor
    Automat& operator=( const Automat& ); //!< Assignement operator
    //@}

    //! @name Pawn management
    //@{
    void AttachPawn ( Pawn& pawn );
	void DetachPawn ( Pawn& pawn );
    //@}

private:
    //! @name types
    //@{
	typedef std::map< T_EntityId, Automat* > T_AutomatMap;
	typedef T_AutomatMap::const_iterator	 CIT_AutomatMap;

    typedef std::set< Pawn* >                T_PawnSet;
    typedef T_PawnSet::const_iterator        CIT_PawnSet;
    //@}

private:
    //! @name Member data
    //@{
    T_EntityId           nId_;
	Team*                pTeam_;
	KnowledgeGroup*      pKnowledgeGroup_;
	T_PawnSet            childPawns_;
	Pawn*			     pPc_;

    // global automat list
    static T_AutomatMap  automats_;
    //@}
};

} // end namespace TEST

#include "Automat.inl"

#endif // __Automat_h_
