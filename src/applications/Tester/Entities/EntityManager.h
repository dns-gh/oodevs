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
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/EntityManager.h $
// $Author: Sbo $
// $Modtime: 5/07/05 18:01 $
// $Revision: 10 $
// $Workfile: EntityManager.h $
//
// *****************************************************************************

#ifndef __EntityManager_h_
#define __EntityManager_h_

#include "Types.h"

namespace TEST
{
    class Automat;
    class Pawn;
    class Team;
    class KnowledgeGroup;
    class Object;
    class Scheduler;
    class Testable_Entity;

// =============================================================================
/** @class  EntityManager
    @brief  EntityManager
    @par    Using example
    @code
    EntityManager;
    @endcode
*/
// Created: SBO 2005-05-11
// =============================================================================
class EntityManager
{
	MT_COPYNOTALLOWED( EntityManager );

public:
    //! @name Constructors/Destructor
    //@{
             EntityManager();
    virtual ~EntityManager();
    //@}

    //! @name Operations
    //@{
    void            Register          ( Automat&        automat        );
    void            Register          ( Pawn&           pawn           );
    void            Register          ( Team&           team           );
    void            Register          ( KnowledgeGroup& knowledgeGroup );
    void            Register          ( Object&         object         );
    void            Unregister        ( Object&         object         );
    Automat*        FindAutomat       ( T_EntityId      nId            ) const;
    Pawn*           FindPawn          ( T_EntityId      nId            ) const;
    Team*           FindTeam          ( T_EntityId      nId            ) const;
    KnowledgeGroup* FindKnowledgeGroup( T_EntityId      nId            ) const;
    Object*         FindObject        ( T_EntityId      nId            ) const;
    //@}

    //! @name Scheduling
    //@{
    void ScheduleAllPawnMissions   ( Scheduler& scheduler, bool bParallel = false ) const;
    void ScheduleAllAutomatMissions( Scheduler& scheduler, bool bParallel = false ) const;
    //@}

    //! @name Test parameters
    //@{
    T_IdVector& GetTestParam_AgentList   ( uint nNbr, const Testable_Entity& target ) const;
    T_IdVector& GetTestParam_AutomateList( uint nNbr, const Testable_Entity& target ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< T_EntityId, Automat* > T_AutomatMap;
	typedef T_AutomatMap::const_iterator     CIT_AutomatMap;

    typedef std::map< T_EntityId, Pawn* >    T_PawnMap;
	typedef T_PawnMap::const_iterator        CIT_PawnMap;

	typedef std::map< T_EntityId, Team* >    T_TeamMap;
	typedef T_TeamMap::const_iterator        CIT_TeamMap;

    typedef std::map< T_EntityId, KnowledgeGroup* >	T_KnowledgeGroupMap;
	typedef T_KnowledgeGroupMap::const_iterator		CIT_KnowledgeGroupMap;

    typedef std::map< T_EntityId, Object* >  T_ObjectMap;
	typedef T_ObjectMap::const_iterator	     CIT_ObjectMap;
    //@}

private:
    //! @name Member data
    //@{
    T_AutomatMap         automats_;
    T_PawnMap            pawns_;
    T_TeamMap            teams_;
    T_KnowledgeGroupMap  knowledgeGroups_;
    T_ObjectMap          objects_;
    //@}
};


} // end namespace TEST

#endif // __EntityManager_h_
