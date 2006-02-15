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
#include "Entity_ABC.h"

class ObjectKnowledge;
class AgentKnowledge;
class Object_ABC;
class KnowledgeGroup;

// =============================================================================
/** @class  Team
    @brief  Represents a team.
    @par    Holds the team's knowledge on dynamic objects and a concatenation
            of it's gtia's agent knowledges.     
*/
// Created: AGN 2003-12-22
// =============================================================================
class Team : public Entity_ABC
{
    MT_COPYNOTALLOWED( Team );
    friend class GLTool;

public:
    //! @name Types
    //@{
    typedef std::map< uint, KnowledgeGroup* > T_KnowledgeGroupMap;
    typedef T_KnowledgeGroupMap::iterator         IT_KnowledgeGroupMap;
    typedef T_KnowledgeGroupMap::const_iterator   CIT_KnowledgeGroupMap;

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

    KnowledgeGroup* CreateKnowledgeGroup     ( uint32 nId );
    void RegisterKnowledgeGroup   ( KnowledgeGroup& gtia );
    void RemoveKnowledgeGroup     ( KnowledgeGroup& gtia );
    void DeleteAllKnowledgeGroups ();
    KnowledgeGroup* FindKnowledgeGroup( uint nID );
    const T_KnowledgeGroupMap& GetKnowledgeGroups() const;


    //! @name Accessors
    //@{
    uint               GetID  () const;
    const std::string& GetName() const;
    uint               GetIdx() const;

    void               SetIdx( uint nIdx );

    const T_ObjectKnowledgeMap& GetObjectKnowledges() const;
    ObjectKnowledge*    FindObjectKnowledge( int nId );
    ObjectKnowledge*    FindKnowledgeOnObject( const Object_ABC& object );
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

    T_KnowledgeGroupMap            gtias_;
//    T_DiplomatyVector    diplomaty_;
    T_ObjectKnowledgeMap objectKnowledges_;
    T_AgentKnowledgeMap  agentKnowledges_;    // Concatenation of the gtias' agent knowledges for easy browsing.

    uint nIdx_;
};

#   include "Team.inl"

#endif // __Team_h_
