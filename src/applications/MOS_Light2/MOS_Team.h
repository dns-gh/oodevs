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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_Team.h $
// $Author: Ape $
// $Modtime: 16/02/05 15:52 $
// $Revision: 8 $
// $Workfile: MOS_Team.h $
//
// *****************************************************************************

#ifndef __MOS_Team_h_
#define __MOS_Team_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_ASN_Types.h"

class MOS_DynaObjectKnowledge;
class MOS_AgentKnowledge;
class MOS_DynaObject_ABC;
class MOS_Gtia;

// =============================================================================
/** @class  MOS_Team
    @brief  Represents a team.
    @par    Holds the team's knowledge on dynamic objects and a concatenation
            of it's gtia's agent knowledges.     
*/
// Created: AGN 2003-12-22
// =============================================================================
class MOS_Team
{
    MT_COPYNOTALLOWED( MOS_Team );
    friend class MOS_GLTool;

public:
    //! @name Types
    //@{
    typedef std::pair< std::string, ASN1T_EnumDiplomatie> T_Diplomaty;
    typedef std::vector< T_Diplomaty >           T_DiplomatyVector;
    typedef T_DiplomatyVector::iterator          IT_DiplomatyVector;
    typedef T_DiplomatyVector::const_iterator    CIT_DiplomatyVector;

    typedef std::map< uint, MOS_Gtia* > T_GtiaMap;
    typedef T_GtiaMap::iterator         IT_GtiaMap;
    typedef T_GtiaMap::const_iterator   CIT_GtiaMap;

    typedef std::map< uint, MOS_DynaObjectKnowledge* >   T_ObjectKnowledgeMap;
    typedef T_ObjectKnowledgeMap::iterator               IT_ObjectKnowledgeMap;
    typedef T_ObjectKnowledgeMap::const_iterator         CIT_ObjectKnowledgeMap;
    typedef T_ObjectKnowledgeMap::const_reverse_iterator RCIT_ObjectKnowledgeMap;

    typedef std::map< uint, MOS_AgentKnowledge* >       T_AgentKnowledgeMap;
    typedef T_AgentKnowledgeMap::iterator               IT_AgentKnowledgeMap;
    typedef T_AgentKnowledgeMap::const_iterator         CIT_AgentKnowledgeMap;
    typedef T_AgentKnowledgeMap::const_reverse_iterator RCIT_AgentKnowledgeMap;
    //@}
    //@}

public:
    //! @name Constructors/Destructor
    //@{
     MOS_Team();
     MOS_Team( uint nID, DIN::DIN_Input& input );
    ~MOS_Team();
    //@}

    void ReadODB( MT_InputArchive_ABC& archive, bool bReadGtias );
    void WriteODB( MT_OutputArchive_ABC& archive );

    void RegisterGtia   ( MOS_Gtia& gtia );
    void RemoveGtia     ( MOS_Gtia& gtia );
    void DeleteAllGtias ();
    MOS_Gtia* FindGtia( uint nID );
    const T_GtiaMap& GetGtias() const;


    //! @name Accessors
    //@{
    uint               GetID  () const;
    const std::string& GetName() const;
    uint               GetIdx() const;

    void               SetIdx( uint nIdx );

    void ChangeDiplomaty( MOS_Team& team, ASN1T_EnumDiplomatie nRelationship );


    const T_ObjectKnowledgeMap& GetObjectKnowledges() const;
    MOS_DynaObjectKnowledge*    FindObjectKnowledge( int nId );
    MOS_DynaObjectKnowledge*    FindKnowledgeOnObject( const MOS_DynaObject_ABC& object );
    //@}

    //! @name Network events
    //@{
    void OnReceiveMsgObjectKnowledgeCreation   ( const ASN1T_MsgObjectKnowledgeCreation&    asnMsg );
    void OnReceiveMsgObjectKnowledgeUpdate     ( const ASN1T_MsgObjectKnowledgeUpdate&      asnMsg );
    void OnReceiveMsgObjectKnowledgeDestruction( const ASN1T_MsgObjectKnowledgeDestruction& asnMsg );
    //@}

    //! @name Knowledges
    //@{
    void RegisterAgentKnowledge( MOS_AgentKnowledge& knowledge );
    void RemoveAgentKnowledge( MOS_AgentKnowledge& knowledge );
    void DestroyAllObjectKnowledges();
    const T_AgentKnowledgeMap&  GetAgentKnowledges() const;
    //@}

private:
    std::string         strName_;
    uint32              nID_;

    T_GtiaMap            gtias_;
    T_DiplomatyVector    diplomaty_;
    T_ObjectKnowledgeMap objectKnowledges_;
    T_AgentKnowledgeMap  agentKnowledges_;    // Concatenation of the gtias' agent knowledges for easy browsing.

    uint nIdx_;
};


#ifdef MOS_USE_INLINE
#   include "MOS_Team.inl"
#endif

#endif // __MOS_Team_h_
