// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-12 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/KnowledgeGroup.h $
// $Author: Sbo $
// $Modtime: 21/07/05 17:04 $
// $Revision: 5 $
// $Workfile: KnowledgeGroup.h $
//
// *****************************************************************************

#ifndef __KnowledgeGroup_h_
#define __KnowledgeGroup_h_

#include "Types.h"
#include "Messages/ASN_Messages.h"

namespace TEST
{
    class Team;
    class PawnKnowledge;

// =============================================================================
/** @class  KnowledgeGroup
    @brief  KnowledgeGroup
    @par    Using example
    @code
    KnowledgeGroup;
    @endcode
*/
// Created: SBO 2005-05-12
// =============================================================================
class KnowledgeGroup
{

public:
    //! @name Constructors/Destructor
    //@{
             KnowledgeGroup( T_EntityId nSimId, DIN::DIN_Input& input );
    virtual ~KnowledgeGroup();
    //@}

    //! @name Operations
    //@{
    static void             Initialize();
    static void             Terminate ();
    static KnowledgeGroup*  Find      ( T_EntityId nId );
    static void             Register  ( KnowledgeGroup& knowledgeGroup );
    static void             Unregister( KnowledgeGroup& knowledgeGroup );
    //@}

    //! @name Accessors
    //@{
    T_EntityId GetId() const;
    //@}

    //! @name Message handlers
    //@{
    void OnReceiveMsgUnitKnowledgeCreation   ( const ASN1T_MsgUnitKnowledgeCreation&    asnMsg );
    void OnReceiveMsgUnitKnowledgeUpdate     ( const ASN1T_MsgUnitKnowledgeUpdate&      asnMsg );
    void OnReceiveMsgUnitKnowledgeDestruction( const ASN1T_MsgUnitKnowledgeDestruction& asnMsg );
    //@}

    //! @name Test Parameters
    //@{
    T_IdVector& GetTestParam_Targets() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    KnowledgeGroup( const KnowledgeGroup& );            //!< Copy constructor
    KnowledgeGroup& operator=( const KnowledgeGroup& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::map< T_EntityId, KnowledgeGroup* >	T_KnowledgeGroupMap;
	typedef T_KnowledgeGroupMap::const_iterator		CIT_KnowledgeGroupMap;

    typedef std::map< T_EntityId, PawnKnowledge* > T_PawnKnowledgeMap;
    typedef T_PawnKnowledgeMap::const_iterator     CIT_PawnKnowledgeMap;
    //@}

private:
    //! @name Member data
    //@{
    T_EntityId         nId_;
    Team*              pTeam_;
    T_PawnKnowledgeMap knownPawns_;

    // global KnowledgeGroup list
    static T_KnowledgeGroupMap  knowledgeGroups_;
    //@}
};

} // end namespace TEST

#include "KnowledgeGroup.inl"

#endif // __KnowledgeGroup_h_
