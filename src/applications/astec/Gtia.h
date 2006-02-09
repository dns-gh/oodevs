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
// $Archive: /MVW_v10/Build/SDK/Light2/src/Gtia.h $
// $Author: Ape $
// $Modtime: 23/11/04 18:22 $
// $Revision: 8 $
// $Workfile: Gtia.h $
//
// *****************************************************************************

#ifndef __Gtia_h_
#define __Gtia_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "ASN_Types.h"
#include "IDManager.h"

class Team;
class Agent;
class AgentKnowledge;
class Population;
class PopulationKnowledge;


// =============================================================================
/** @class  Gtia
    @brief  Represents a gtia.
    @par    Holds the gtia's knowledge on other agents.
*/
// Created: AGN 2003-12-22
// =============================================================================
class Gtia
{
    MT_COPYNOTALLOWED( Gtia );
    friend class GLTool;

public:
    //! @name Types
    //@{
    typedef std::map< uint, AgentKnowledge* >      T_AgentKnowledgeMap;
    typedef T_AgentKnowledgeMap::iterator              IT_AgentKnowledgeMap;
    typedef T_AgentKnowledgeMap::const_iterator        CIT_AgentKnowledgeMap;

    typedef std::map< uint, PopulationKnowledge* > T_PopulationKnowledgeMap;
    typedef T_PopulationKnowledgeMap::iterator         IT_PopulationKnowledgeMap;
    typedef T_PopulationKnowledgeMap::const_iterator   CIT_PopulationKnowledgeMap;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             Gtia( bool bGenerateId = false );
             Gtia( uint32 nId );
    virtual ~Gtia();
    //@}

    //! @name ODB operations
    //@{
    void ReadODB ( InputArchive&     archive );
    void WriteODB( MT_OutputArchive_ABC& archive );
    //@}

    //! @name Accessors & Modifiers
    //@{
    uint      GetID() const;
    Team& GetTeam() const;

    const T_AgentKnowledgeMap& GetAgentKnowledges     () const;
    AgentKnowledge*        FindAgentKnowledge     ( int nId );
    AgentKnowledge*        FindKnowledgeOnAgent   ( const Agent& agent );

    const T_PopulationKnowledgeMap& GetPopulationKnowledges() const;
    PopulationKnowledge*        FindPopulationKnowledge( int nId );
    PopulationKnowledge*        FindKnowledgeOnPopulation( const Population& population );
    //@}
    
    //! @name Modifiers
    //@{
    void      SetTeam( Team& team );
    void      SetType( const std::string& strType );
    //@}

    //! @name Network events
    //@{
    bool OnReceiveMsgUnitKnowledgeCreation         ( const ASN1T_MsgUnitKnowledgeCreation&          asnMsg );
    void OnReceiveMsgUnitKnowledgeUpdate           ( const ASN1T_MsgUnitKnowledgeUpdate&            asnMsg );
    void OnReceiveMsgUnitKnowledgeDestruction      ( const ASN1T_MsgUnitKnowledgeDestruction&       asnMsg );
    bool OnReceiveMsgPopulationKnowledgeCreation   ( const ASN1T_MsgPopulationKnowledgeCreation&    asnMsg );
    void OnReceiveMsgPopulationKnowledgeUpdate     ( const ASN1T_MsgPopulationKnowledgeUpdate&      asnMsg );
    void OnReceiveMsgPopulationKnowledgeDestruction( const ASN1T_MsgPopulationKnowledgeDestruction& asnMsg );
    //@}

private:
    uint32                   nID_;
    Team*                pTeam_;
    std::string              strType_;

    T_AgentKnowledgeMap      agentKnowledges_;
    T_PopulationKnowledgeMap populationKnowledges_;

private:
    static IDManager     idManager_;
};


#   include "Gtia.inl"

#endif // __Gtia_h_
