// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MOS_Population_h_
#define __MOS_Population_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_Types.h"
#include "MOS_ASN_Types.h"

#include "MOS_PopulationPart_ABC.h"

#include "MOS_Agent_ABC.h"
#include "MOS_TypePopulation.h"
#include "MOS_AgentKnowledge.h"

class MOS_PopulationConcentration;
class MOS_PopulationFlow;
class MOS_Team;

class MOS_AgentModel;

class MOS_AgentKnowledge;


// =============================================================================
// Created: HME 2005-09-29
// =============================================================================
class MOS_Population : public MOS_Agent_ABC
{
    friend class MOS_GLTool; // to avoid "get" on concentration/flow maps

public:
	typedef std::map< uint, MOS_PopulationConcentration* >  T_ConcentrationMap;
	typedef T_ConcentrationMap::iterator                    IT_ConcentrationMap;
	typedef T_ConcentrationMap::const_iterator              CIT_ConcentrationMap;
    typedef T_ConcentrationMap::const_reverse_iterator      RCIT_ConcentrationMap;

	typedef std::map< uint , MOS_PopulationFlow* >          T_FlowMap;
	typedef T_FlowMap::iterator                             IT_FlowMap;
	typedef T_FlowMap::const_iterator                       CIT_FlowMap;
    typedef T_FlowMap::const_reverse_iterator               RCIT_FlowMap;

private:
    typedef std::map< uint, MOS_AgentKnowledge* >           T_AgentKnowledgeMap;
    typedef T_AgentKnowledgeMap::iterator                   IT_AgentKnowledgeMap;
    typedef T_AgentKnowledgeMap::const_iterator             CIT_AgentKnowledgeMap;

public:
    //! @name Constructor/Destructor
    //@{
     MOS_Population();
     MOS_Population( const ASN1T_MsgPopulationCreation& asnMsg );
     MOS_Population( MT_Vector2D point, E_PopulationAttitude attitude, int persons, MOS_Team& team , std::string name, MOS_TypePopulation* type );
    ~MOS_Population();
    //@}

    void ReadODB( MOS_InputArchive& archive );
    void WriteODB( MT_XXmlOutputArchive& archive );

    //! @name Network
    //@{
	void CreatePopulationFlow         ( const ASN1T_MsgPopulationFluxCreation&             asnMsg );
	void CreatePopulationConcentration( const ASN1T_MsgPopulationConcentrationCreation&    asnMsg );
    void CreatePopulationConcentration( MT_Vector2D point, E_PopulationAttitude attitude , int persons );
    void UpdatePopulation             ( const ASN1T_MsgPopulationUpdate&                   asnMsg );
    void UpdatePopulationFlow         ( const ASN1T_MsgPopulationFluxUpdate&               asnMsg );
	void UpdatePopulationConcentration( const ASN1T_MsgPopulationConcentrationUpdate&      asnMsg );
	void DeletePopulationFlow         ( const ASN1T_MsgPopulationFluxDestruction&          asnMsg );
	void DeletePopulationConcentration( const ASN1T_MsgPopulationConcentrationDestruction& asnMsg );
    //@}

    const MOS_PopulationConcentration* FindConcentration ( uint nID ) const;
    const MOS_PopulationFlow*          FindFlow          ( uint nID ) const;
    virtual MOS_AgentKnowledge*        FindAgentKnowledge( uint nId );

    //! @name Accessors
    //@{
	virtual const MIL_AgentID          GetID  () const;
	virtual MOS_Team&                  GetTeam() const;
	virtual const std::string          GetName() const;
	virtual const MT_Vector2D&         GetPos () const;

	uint                               GetLivingHumans() const;
	uint                               GetDeadHumans  () const;
    const MOS_AgentModel&              GetModel       () const;
    const MOS_TypePopulation&          GetType        () const;

    // for selection management
    const T_ConcentrationMap&          GetConcentrations() const;
    const T_FlowMap&                   GetFlows         () const;
    bool                               IsEmpty          () const;
    //@}

private:
	MIL_AgentID               nPopulationID_;
	std::string               strName_;
    const MOS_TypePopulation* pType_;
	MOS_Team*                 pTeam_;

    T_ConcentrationMap        concentrationMap_;
	T_FlowMap                 flowMap_;

    T_AgentKnowledgeMap       agentKnowledges_;

private:
    static MIL_AgentID        nMaxId_;
};

#include "MOS_Population.inl"

#endif // __MOS_Population_h_
