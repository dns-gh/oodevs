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
// $Archive: /MVW_v10/Build/SDK/Light2/src/Team.h $
// $Author: Ape $
// $Modtime: 16/02/05 15:52 $
// $Revision: 8 $
// $Workfile: Team.h $
//
// *****************************************************************************

#ifndef __Team_h_
#define __Team_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "ASN_Types.h"

class ObjectKnowledge;
class AgentKnowledge;
class Object_ABC;
class Gtia;

// =============================================================================
/** @class  Team
    @brief  Represents a team.
    @par    Holds the team's knowledge on dynamic objects and a concatenation
            of it's gtia's agent knowledges.     
*/
// Created: AGN 2003-12-22
// =============================================================================
class Team
{
    MT_COPYNOTALLOWED( Team );
    friend class GLTool;

public:
    //! @name Types
    //@{
    typedef std::pair< std::string, ASN1T_EnumDiplomatie> T_Diplomaty;
    typedef std::vector< T_Diplomaty >           T_DiplomatyVector;
    typedef T_DiplomatyVector::iterator          IT_DiplomatyVector;
    typedef T_DiplomatyVector::const_iterator    CIT_DiplomatyVector;

    typedef std::map< uint, Gtia* > T_GtiaMap;
    typedef T_GtiaMap::iterator         IT_GtiaMap;
    typedef T_GtiaMap::const_iterator   CIT_GtiaMap;

    typedef std::map< uint, ObjectKnowledge* >   T_ObjectKnowledgeMap;
    typedef T_ObjectKnowledgeMap::iterator               IT_ObjectKnowledgeMap;
    typedef T_ObjectKnowledgeMap::const_iterator         CIT_ObjectKnowledgeMap;
    typedef T_ObjectKnowledgeMap::const_reverse_iterator RCIT_ObjectKnowledgeMap;

    typedef std::map< uint, AgentKnowledge* >       T_AgentKnowledgeMap;
    typedef T_AgentKnowledgeMap::iterator               IT_AgentKnowledgeMap;
    typedef T_AgentKnowledgeMap::const_iterator         CIT_AgentKnowledgeMap;
    typedef T_AgentKnowledgeMap::const_reverse_iterator RCIT_AgentKnowledgeMap;
    //@}
    //@}

public:
    //! @name Constructors/Destructor
    //@{
     Team();
     Team( uint nID, DIN::DIN_Input& input );
    ~Team();
    //@}

    void ReadODB( InputArchive& archive, bool bReadGtias );
    void WriteODB( MT_OutputArchive_ABC& archive );

    void CreateGtia     ( uint32 nId );
    void RegisterGtia   ( Gtia& gtia );
    void RemoveGtia     ( Gtia& gtia );
    void DeleteAllGtias ();
    Gtia* FindGtia( uint nID );
    const T_GtiaMap& GetGtias() const;


    //! @name Accessors
    //@{
    uint               GetID  () const;
    const std::string& GetName() const;
    uint               GetIdx() const;

    void               SetIdx( uint nIdx );

    void ChangeDiplomaty( Team& team, ASN1T_EnumDiplomatie nRelationship );


    const T_ObjectKnowledgeMap& GetObjectKnowledges() const;
    ObjectKnowledge*    FindObjectKnowledge( int nId );
    ObjectKnowledge*    FindKnowledgeOnObject( const Object_ABC& object );
    //@}

    //! @name Network events
    //@{
    bool OnReceiveMsgObjectKnowledgeCreation   ( const ASN1T_MsgObjectKnowledgeCreation&    asnMsg );
    void OnReceiveMsgObjectKnowledgeUpdate     ( const ASN1T_MsgObjectKnowledgeUpdate&      asnMsg );
    void OnReceiveMsgObjectKnowledgeDestruction( const ASN1T_MsgObjectKnowledgeDestruction& asnMsg );
    //@}

    //! @name Knowledges
    //@{
    void RegisterAgentKnowledge( AgentKnowledge& knowledge );
    void RemoveAgentKnowledge( AgentKnowledge& knowledge );
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

#   include "Team.inl"

#endif // __Team_h_
