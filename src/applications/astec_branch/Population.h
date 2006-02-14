// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Population_h_
#define __Population_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "Types.h"
#include "ASN_Types.h"

#include "PopulationPart_ABC.h"
#include "Agent_ABC.h"
#include "TypePopulation.h"
#include "AgentKnowledge.h"
#include "Resolver.h"

class PopulationConcentration;
class PopulationFlow;
class Team;

class AgentModel;

class AgentKnowledge;


// =============================================================================
// Created: HME 2005-09-29
// =============================================================================
class Population : public Agent_ABC
                 , public Resolver< PopulationConcentration >
                 , public Resolver< PopulationFlow >
{
    friend class GLTool; // to avoid "get" on concentration/flow maps

public:
    //! @name Constructor/Destructor
    //@{
    explicit Population( const ASN1T_MsgPopulationCreation& asnMsg );
    virtual ~Population();
    //@}

    void ReadODB( InputArchive& archive );
    void WriteODB( MT_XXmlOutputArchive& archive );

    //! @name Network
    //@{
	void CreatePopulationFlow         ( const ASN1T_MsgPopulationFluxCreation&             asnMsg );
    void UpdatePopulationFlow         ( const ASN1T_MsgPopulationFluxUpdate&               asnMsg );
    void DeletePopulationFlow         ( const ASN1T_MsgPopulationFluxDestruction&          asnMsg );
    void UpdatePopulation             ( const ASN1T_MsgPopulationUpdate&                   asnMsg );
    void CreatePopulationConcentration( const ASN1T_MsgPopulationConcentrationCreation&    asnMsg );
	void UpdatePopulationConcentration( const ASN1T_MsgPopulationConcentrationUpdate&      asnMsg );
	void DeletePopulationConcentration( const ASN1T_MsgPopulationConcentrationDestruction& asnMsg );
    //@}



    const PopulationConcentration* FindConcentration ( uint nID ) const;
    const PopulationFlow*          FindFlow          ( uint nID ) const;
    virtual AgentKnowledge*        FindAgentKnowledge( uint nId );

    const PopulationConcentration& GetConcentration ( uint nID ) const;
    const PopulationFlow&          GetFlow          ( uint nID ) const;

    //! @name Accessors
    //@{
    virtual unsigned long     GetId() const;
	virtual Team&             GetTeam() const;
	virtual std::string       GetName() const;
	virtual const MT_Vector2D GetPos () const;
    virtual const MT_Vector2D GetAggregatedPos() const;
    virtual const MT_Vector2D GetConflictPos  ( const MT_Vector2D& origin ) const;

	uint                      GetLivingHumans() const;
	uint                      GetDeadHumans  () const;
    const AgentModel&         GetModel       () const;
    const TypePopulation&     GetType        () const;
    bool                      IsEmpty          () const;
    //@}

    
private:
    //! @name Types
    //@{
    typedef std::map< uint, AgentKnowledge* >           T_AgentKnowledgeMap;
    typedef T_AgentKnowledgeMap::iterator                   IT_AgentKnowledgeMap;
    typedef T_AgentKnowledgeMap::const_iterator             CIT_AgentKnowledgeMap;
    //@}

private:
    //! @name Member data
    //@{
    Population( const Population& );
    Population& operator=( const Population& );
	MIL_AgentID               nPopulationID_;
	std::string               strName_;
    const TypePopulation* pType_;
	Team*                 pTeam_;
    T_AgentKnowledgeMap       agentKnowledges_;
    //@}

private:
    static MIL_AgentID        nMaxId_;
};

#include "Population.inl"

#endif // __Population_h_
