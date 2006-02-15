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
// $Archive: /MVW_v10/Build/SDK/Light2/src/KnowledgeGroup.h $
// $Author: Ape $
// $Modtime: 23/11/04 18:22 $
// $Revision: 8 $
// $Workfile: KnowledgeGroup.h $
//
// *****************************************************************************

#ifndef __KnowledgeGroup_h_
#define __KnowledgeGroup_h_

#include "ASN_Types.h"
#include "IDManager.h"

class Team;
class Agent;
class AgentKnowledge;
class Population;
class PopulationKnowledge;


// =============================================================================
/** @class  KnowledgeGroup
    @brief  Represents a gtia.
    @par    Holds the gtia's knowledge on other agents.
*/
// Created: AGN 2003-12-22
// =============================================================================
class KnowledgeGroup
{
    MT_COPYNOTALLOWED( KnowledgeGroup );
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
             KnowledgeGroup( bool bGenerateId = false );
             KnowledgeGroup( uint32 nId );
    virtual ~KnowledgeGroup();
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
    bool Update( const ASN1T_MsgUnitKnowledgeCreation&          asnMsg );
    void Update( const ASN1T_MsgUnitKnowledgeUpdate&            asnMsg );
    void Update( const ASN1T_MsgUnitKnowledgeDestruction&       asnMsg );

    bool Update( const ASN1T_MsgPopulationKnowledgeCreation&    asnMsg );
    void Update( const ASN1T_MsgPopulationKnowledgeUpdate&      asnMsg );
    void Update( const ASN1T_MsgPopulationKnowledgeDestruction& asnMsg );
    void Update( const ASN1T_MsgPopulationConcentrationKnowledgeCreation& asnMsg );
    void Update( const ASN1T_MsgPopulationConcentrationKnowledgeUpdate& asnMsg );
    void Update( const ASN1T_MsgPopulationConcentrationKnowledgeDestruction& asnMsg );
    void Update( const ASN1T_MsgPopulationFluxKnowledgeCreation& asnMsg );
    void Update( const ASN1T_MsgPopulationFluxKnowledgeUpdate& asnMsg );
    void Update( const ASN1T_MsgPopulationFluxKnowledgeDestruction& asnMsg );
    //@}

    //! @name Helpers
    //@{
    template< typename T >
    void UpdatePopulationKnowledge( const T& message );
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


#   include "KnowledgeGroup.inl"

#endif // __KnowledgeGroup_h_
