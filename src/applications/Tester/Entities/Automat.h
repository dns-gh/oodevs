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
#include "Testable_Entity.h"

namespace TEST
{
    class KnowledgeGroup;
    class Team;
    class Pawn;
    class AutomatType;
    class Workspace;

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
class Automat : public Testable_Entity
{

public:
    //! @name Constructors/Destructor
    //@{
             Automat( const Workspace& workspace, const ASN1T_MsgAutomateCreation& asnMsg );
    virtual ~Automat();
    //@}

    //! @name Accessors
    //@{
    virtual       T_EntityId      GetId            () const;
    virtual const Position&       GetPosition      () const;
    virtual const Automat&        GetAutomat       () const;

            const AutomatType&    GetType          () const;
	        const Team&           GetTeam          () const;
	        const KnowledgeGroup& GetKnowledgeGroup() const;
	        const Pawn&	          GetPc            () const;
    //@}

    //! @name Operations
    //@{
    void OnKnowledgeGroupChanged( KnowledgeGroup& knowledgeGroup );
    //@}

    //! @name Pawn management
    //@{
    void AttachPawn( Pawn& pawn );
	void DetachPawn( Pawn& pawn );
    //@}

    //! @name Mission Scheduling
    //@{
    void ScheduleAllMissions ( Scheduler& scheduler );
    void ScheduleMission     ( Scheduler& scheduler, const std::string& strMissionName );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Automat( const Automat& );            //!< Copy constructor
    Automat& operator=( const Automat& ); //!< Assignement operator
    //@}

private:
    //! @name types
    //@{
    typedef std::set< Pawn* >         T_PawnSet;
    typedef T_PawnSet::const_iterator CIT_PawnSet;
    //@}

private:
    //! @name Member data
    //@{
    T_EntityId         nId_;
	Team*              pTeam_;
    KnowledgeGroup*    pKnowledgeGroup_;
	Pawn*			   pPc_;
    const AutomatType* pType_;
    T_PawnSet          childPawns_;
    //@}
};

} // end namespace TEST

#include "Automat.inl"

#endif // __Automat_h_
