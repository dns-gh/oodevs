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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_Gtia.h $
// $Author: Ape $
// $Modtime: 23/11/04 18:22 $
// $Revision: 8 $
// $Workfile: MOS_Gtia.h $
//
// *****************************************************************************

#ifndef __MOS_Gtia_h_
#define __MOS_Gtia_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_ASN_Types.h"
#include "MOS_IDManager.h"

class MOS_AgentKnowledge;
class MOS_Agent;
class MOS_Team;


// =============================================================================
/** @class  MOS_Gtia
    @brief  Represents a gtia.
    @par    Holds the gtia's knowledge on other agents.
*/
// Created: AGN 2003-12-22
// =============================================================================
class MOS_Gtia
{
    MT_COPYNOTALLOWED( MOS_Gtia );
    friend class MOS_GLTool;

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
    MOS_Gtia( bool bGenerateId = false );
    ~MOS_Gtia();
    //@}

    //! @name Init
    //@{
    void Initialize( DIN::DIN_Input& input );
    //@}

    //! @name Accessors & Modifiers
    //@{
    void SetTeam( MOS_Team& team );
    MOS_Team& GetTeam() const;

    void SetType( const std::string& strType );

    uint GetID() const;

    const T_AgentKnowledgeMap& GetAgentKnowledges() const;
    MOS_AgentKnowledge*        FindAgentKnowledge( int nId );
    MOS_AgentKnowledge*        FindKnowledgeOnAgent( const MOS_Agent& agent );
    //@}

    //! @name Network events
    //@{
    void OnReceiveMsgUnitKnowledgeCreation   ( const ASN1T_MsgUnitKnowledgeCreation&    asnMsg );
    void OnReceiveMsgUnitKnowledgeUpdate     ( const ASN1T_MsgUnitKnowledgeUpdate&      asnMsg );
    void OnReceiveMsgUnitKnowledgeDestruction( const ASN1T_MsgUnitKnowledgeDestruction& asnMsg );
    //@}

    void ReadODB( MT_InputArchive_ABC& archive );
    void WriteODB( MT_OutputArchive_ABC& archive );

private:
    uint32              nID_;
    T_AgentKnowledgeMap agentKnowledges_;
    MOS_Team* pTeam_;

    std::string strType_;


private:
    static MOS_IDManager idManager_;
};


#ifdef MOS_USE_INLINE
#   include "MOS_Gtia.inl"
#endif


#endif // __MOS_Gtia_h_
