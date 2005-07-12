// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: NLD 2004-03-18 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Gtia.h $
// $Author: Nld $
// $Modtime: 14/02/05 11:26 $
// $Revision: 3 $
// $Workfile: MOS_Gtia.h $
//
// *****************************************************************************

#ifndef __MOS_Gtia_h_
#define __MOS_Gtia_h_

#include "MOS_Types.h"
#include "MOS_ASN_Types.h"

class MOS_AgentKnowledge;
class MOS_KnowledgeList_View;
class MOS_Team;

// =============================================================================

// Created: NLD 2004-03-18
// =============================================================================
class MOS_Gtia
{
    MT_COPYNOTALLOWED( MOS_Gtia );

public:
    //! @name Types
    //@{
    typedef std::map< uint, MOS_AgentKnowledge* >   T_AgentKnowledgeMap;
    typedef T_AgentKnowledgeMap::iterator           IT_AgentKnowledgeMap;
    typedef T_AgentKnowledgeMap::const_iterator     CIT_AgentKnowledgeMap;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    MOS_Gtia( uint nID, DIN::DIN_Input& input );
    ~MOS_Gtia();
    //@}

   //! @name Accessors
    //@{
    uint                       GetID  () const;
    const MOS_Team&            GetTeam() const;
    const T_AgentKnowledgeMap& GetAgentKnowledges() const;
    //@}

    //! @name Network events
    //@{
    void OnReceiveMsgUnitKnowledgeCreation   ( const ASN1T_MsgUnitKnowledgeCreation&    asnMsg );
    void OnReceiveMsgUnitKnowledgeUpdate     ( const ASN1T_MsgUnitKnowledgeUpdate&      asnMsg );
    void OnReceiveMsgUnitKnowledgeDestruction( const ASN1T_MsgUnitKnowledgeDestruction& asnMsg );
    //@}

    //! @name Knowledges
    //@{
    void SetAgentKnowledgeListView( MOS_KnowledgeList_View* pListView );
    //@}

    //! @name Display
    //@{
    bool ShouldDrawKnowledges() const;
    void NotifyShouldDrawKnowledges( bool bDraw );
    void Draw();
    //@}

private:
    uint32              nID_;
    T_AgentKnowledgeMap agentKnowledges_;

    MOS_KnowledgeList_View* pKnowledgeListView_;

    bool bShouldDrawKnowledges_;
    const MOS_Team* pArmy_;
};

#include "MOS_Gtia.inl"

#endif // __MOS_Gtia_h_
