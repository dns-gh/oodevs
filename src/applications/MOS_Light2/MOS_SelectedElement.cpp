// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-05-05 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_SelectedElement.cpp $
// $Author: Age $
// $Modtime: 5/04/05 18:25 $
// $Revision: 4 $
// $Workfile: MOS_SelectedElement.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MOS_SelectedElement.h"
#include "MOS_Agent.h"
#include "MOS_Gtia.h"
#include "MOS_Team.h"
#include "MOS_Population.h"
#include "MOS_PopulationConcentration.h"
#include "MOS_PopulationFlow.h"
#include "MOS_Object_ABC.h"
#include "MOS_AgentKnowledge.h"
#include "MOS_ObjectKnowledge.h"
#include "MOS_PopulationKnowledge.h"
#include "MOS_PopulationConcentrationKnowledge.h"
#include "MOS_PopulationFlowKnowledge.h"

// -----------------------------------------------------------------------------
// Name: MOS_SelectedElement constructor
// Created: APE 2004-05-05
// -----------------------------------------------------------------------------
MOS_SelectedElement::MOS_SelectedElement()
{
    Init();
}


// -----------------------------------------------------------------------------
// Name: MOS_SelectedElement constructor
// Created: APE 2004-05-05
// -----------------------------------------------------------------------------
MOS_SelectedElement::MOS_SelectedElement( MOS_Agent& agent )
{
    Init();
    pAgent_ = &agent;
}

// -----------------------------------------------------------------------------
// Name: MOS_SelectedElement constructor
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
MOS_SelectedElement::MOS_SelectedElement( MOS_Population& population )
{
	Init();
	pPopulation_ = &population;
}

// -----------------------------------------------------------------------------
// Name: MOS_SelectedElement constructor
// Created: SBO 2005-10-26
// -----------------------------------------------------------------------------
MOS_SelectedElement::MOS_SelectedElement( MOS_PopulationConcentration& populationConcentration )
{
    Init();
    pPopulationConcentration_ = &populationConcentration;
}
    
// -----------------------------------------------------------------------------
// Name: MOS_SelectedElement constructor
// Created: SBO 2005-10-26
// -----------------------------------------------------------------------------
MOS_SelectedElement::MOS_SelectedElement( MOS_PopulationFlow& populationFlow )
{
    Init();
    pPopulationFlow_ = &populationFlow;
}

// -----------------------------------------------------------------------------
// Name: MOS_SelectedElement constructor
// Created: APE 2004-05-05
// -----------------------------------------------------------------------------
MOS_SelectedElement::MOS_SelectedElement( MOS_Gtia& gtia )
{
    Init();
    pGtia_ = &gtia;
}


// -----------------------------------------------------------------------------
// Name: MOS_SelectedElement constructor
// Created: APE 2004-05-05
// -----------------------------------------------------------------------------
MOS_SelectedElement::MOS_SelectedElement( MOS_Team& team )
{
    Init();
    pTeam_ = &team;
}


// -----------------------------------------------------------------------------
// Name: MOS_SelectedElement constructor
// Created: APE 2004-05-05
// -----------------------------------------------------------------------------
MOS_SelectedElement::MOS_SelectedElement( MOS_Object_ABC& object )
{
    Init();
    pObject_ = &object;
}


// -----------------------------------------------------------------------------
// Name: MOS_SelectedElement constructor
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
MOS_SelectedElement::MOS_SelectedElement( MOS_AgentKnowledge& agentKnowledge )
{
    Init();
    pAgentKnowledge_ = &agentKnowledge;
}


// -----------------------------------------------------------------------------
// Name: MOS_SelectedElement constructor
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
MOS_SelectedElement::MOS_SelectedElement( MOS_ObjectKnowledge& objectKnowledge )
{
    Init();
    pObjectKnowledge_ = &objectKnowledge;
}

// -----------------------------------------------------------------------------
// Name: MOS_SelectedElement constructor
// Created: SBO 2005-10-19
// -----------------------------------------------------------------------------
MOS_SelectedElement::MOS_SelectedElement( MOS_PopulationKnowledge& populationKnowledge )
{
    Init();
    pPopulationKnowledge_ = &populationKnowledge;
}

// -----------------------------------------------------------------------------
// Name: MOS_SelectedElement constructor
// Created: SBO 2005-10-26
// -----------------------------------------------------------------------------
MOS_SelectedElement::MOS_SelectedElement( MOS_PopulationConcentrationKnowledge& populationConcentrationKnowledge )
{
    Init();
    pPopulationConcentrationKnowledge_ = &populationConcentrationKnowledge;
}
    
// -----------------------------------------------------------------------------
// Name: MOS_SelectedElement constructor
// Created: SBO 2005-10-26
// -----------------------------------------------------------------------------
MOS_SelectedElement::MOS_SelectedElement( MOS_PopulationFlowKnowledge& populationFlowKnowledge )
{
    Init();
    pPopulationFlowKnowledge_ = &populationFlowKnowledge;
}

// -----------------------------------------------------------------------------
// Name: MOS_SelectedElement constructor
// Created: APE 2004-05-05
// -----------------------------------------------------------------------------
MOS_SelectedElement::MOS_SelectedElement( MOS_TacticalLine_ABC& line, int nLinePoint )
{
    Init();
    pLine_ = &line;
    nLinePoint_ = nLinePoint;
}


// -----------------------------------------------------------------------------
// Name: MOS_SelectedElement constructor
// Created: APE 2004-05-12
// -----------------------------------------------------------------------------
MOS_SelectedElement::MOS_SelectedElement( MOS_RC& rc )
{
    Init();
    pRC_ = &rc;
}


// -----------------------------------------------------------------------------
// Name: MOS_SelectedElement destructor
// Created: APE 2004-05-05
// -----------------------------------------------------------------------------
MOS_SelectedElement::~MOS_SelectedElement()
{
}


// -----------------------------------------------------------------------------
// Name: MOS_SelectedElement::Init
// Created: APE 2004-05-12
// -----------------------------------------------------------------------------
void MOS_SelectedElement::Init()
{
    pAgent_                            = 0;
    pGtia_                             = 0;
    pTeam_                             = 0;
    pObject_                           = 0;
    pAgentKnowledge_                   = 0;
    pObjectKnowledge_                  = 0;
    pRC_                               = 0;
    pLine_                             = 0;
    nLinePoint_                        = -1;
	pPopulation_                       = 0;
    pPopulationConcentration_          = 0;
    pPopulationFlow_                   = 0;
    pPopulationKnowledge_              = 0;
    pPopulationConcentrationKnowledge_ = 0;
    pPopulationFlowKnowledge_          = 0;
}

// -----------------------------------------------------------------------------
// Name: MOS_SelectedElement::operator==
// Created: AGE 2005-04-05
// -----------------------------------------------------------------------------
bool MOS_SelectedElement::operator==( const MOS_SelectedElement& rhs ) const
{
    return pAgent_                            == rhs.pAgent_
		&& pPopulation_                       == rhs.pPopulation_
        && pPopulationConcentration_          == rhs.pPopulationConcentration_
        && pPopulationFlow_                   == rhs.pPopulationFlow_
        && pGtia_                             == rhs.pGtia_
        && pTeam_                             == rhs.pTeam_
        && pObject_                           == rhs.pObject_
        && pAgentKnowledge_                   == rhs.pAgentKnowledge_
        && pObjectKnowledge_                  == rhs.pObjectKnowledge_
        && pPopulationKnowledge_              == rhs.pPopulationKnowledge_
        && pPopulationConcentrationKnowledge_ == rhs.pPopulationConcentrationKnowledge_
        && pPopulationFlowKnowledge_          == rhs.pPopulationFlowKnowledge_
        && pRC_                               == rhs.pRC_
        && pLine_                             == rhs.pLine_
        && nLinePoint_                        == rhs.nLinePoint_;
}

// -----------------------------------------------------------------------------
// Name: MOS_SelectedElement::Dump
// Created: AGE 2005-04-05
// -----------------------------------------------------------------------------
void MOS_SelectedElement::Dump()
{
    std::stringstream str;
    str << "Selected : ";
    if( pAgent_ )
        str << " agent : " << pAgent_->GetID() << "; ";
	if( pPopulation_ )
        str << " population : " << pPopulation_->GetID() << "; ";
    if( pPopulationConcentration_ )
        str << " population concentration : " << pPopulationConcentration_->GetID() << "; ";
    if( pPopulationFlow_ )
        str << " population flow : " << pPopulationFlow_->GetID() << "; ";
    if( pPopulation_ )
        str << " population : " << pPopulation_->GetID() << "; ";
    if( pGtia_ )
        str << " gtia : " << pGtia_->GetID() << "; ";
    if( pTeam_ )
        str << " team : " << pTeam_->GetID() << "; ";
    if( pObject_ )
        str << " object : " << pObject_->GetID() << "; ";
    if( pAgentKnowledge_ )
        str << " agent knowledge : " << pAgentKnowledge_->GetID() << "; ";
    if( pObjectKnowledge_ )
        str << " object knowledge : " << pObjectKnowledge_->GetID() << "; ";
    if( pPopulationKnowledge_ )
        str << " population knowledge : " << pPopulationKnowledge_->GetID() << "; ";
    if( pPopulationConcentrationKnowledge_ )
        str << " population concentration knowledge : " << pPopulationConcentrationKnowledge_->GetID() << "; ";
    if( pPopulationFlowKnowledge_ )
        str << " population flow knowledge : " << pPopulationFlowKnowledge_->GetID() << "; ";
    MT_LOG_INFO_MSG( str.str() );
}
