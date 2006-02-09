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
// $Archive: /MVW_v10/Build/SDK/Light2/src/SelectedElement.cpp $
// $Author: Age $
// $Modtime: 5/04/05 18:25 $
// $Revision: 4 $
// $Workfile: SelectedElement.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "SelectedElement.h"
#include "Agent.h"
#include "Gtia.h"
#include "Team.h"
#include "Population.h"
#include "PopulationConcentration.h"
#include "PopulationFlow.h"
#include "Object_ABC.h"
#include "AgentKnowledge.h"
#include "ObjectKnowledge.h"
#include "PopulationKnowledge.h"
#include "PopulationConcentrationKnowledge.h"
#include "PopulationFlowKnowledge.h"

// -----------------------------------------------------------------------------
// Name: SelectedElement constructor
// Created: APE 2004-05-05
// -----------------------------------------------------------------------------
SelectedElement::SelectedElement()
{
    Init();
}


// -----------------------------------------------------------------------------
// Name: SelectedElement constructor
// Created: APE 2004-05-05
// -----------------------------------------------------------------------------
SelectedElement::SelectedElement( Agent& agent )
{
    Init();
    pAgent_ = &agent;
}

// -----------------------------------------------------------------------------
// Name: SelectedElement constructor
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
SelectedElement::SelectedElement( Population& population )
{
	Init();
	pPopulation_ = &population;
}

// -----------------------------------------------------------------------------
// Name: SelectedElement constructor
// Created: SBO 2005-10-26
// -----------------------------------------------------------------------------
SelectedElement::SelectedElement( PopulationConcentration& populationConcentration )
{
    Init();
    pPopulationConcentration_ = &populationConcentration;
    pPopulation_              = const_cast< Population* >( &populationConcentration.GetPopulation() );
}
    
// -----------------------------------------------------------------------------
// Name: SelectedElement constructor
// Created: SBO 2005-10-26
// -----------------------------------------------------------------------------
SelectedElement::SelectedElement( PopulationFlow& populationFlow )
{
    Init();
    pPopulationFlow_ = &populationFlow;
    pPopulation_     = const_cast< Population* >( &populationFlow.GetPopulation() );
}

// -----------------------------------------------------------------------------
// Name: SelectedElement constructor
// Created: APE 2004-05-05
// -----------------------------------------------------------------------------
SelectedElement::SelectedElement( Gtia& gtia )
{
    Init();
    pGtia_ = &gtia;
}


// -----------------------------------------------------------------------------
// Name: SelectedElement constructor
// Created: APE 2004-05-05
// -----------------------------------------------------------------------------
SelectedElement::SelectedElement( Team& team )
{
    Init();
    pTeam_ = &team;
}


// -----------------------------------------------------------------------------
// Name: SelectedElement constructor
// Created: APE 2004-05-05
// -----------------------------------------------------------------------------
SelectedElement::SelectedElement( Object_ABC& object )
{
    Init();
    pObject_ = &object;
}


// -----------------------------------------------------------------------------
// Name: SelectedElement constructor
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
SelectedElement::SelectedElement( AgentKnowledge& agentKnowledge )
{
    Init();
    pAgentKnowledge_ = &agentKnowledge;
}


// -----------------------------------------------------------------------------
// Name: SelectedElement constructor
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
SelectedElement::SelectedElement( ObjectKnowledge& objectKnowledge )
{
    Init();
    pObjectKnowledge_ = &objectKnowledge;
}

// -----------------------------------------------------------------------------
// Name: SelectedElement constructor
// Created: SBO 2005-10-19
// -----------------------------------------------------------------------------
SelectedElement::SelectedElement( PopulationKnowledge& populationKnowledge )
{
    Init();
    pPopulationKnowledge_ = &populationKnowledge;
}

// -----------------------------------------------------------------------------
// Name: SelectedElement constructor
// Created: SBO 2005-10-26
// -----------------------------------------------------------------------------
SelectedElement::SelectedElement( PopulationConcentrationKnowledge& populationConcentrationKnowledge )
{
    Init();
    pPopulationConcentrationKnowledge_ = &populationConcentrationKnowledge;
}
    
// -----------------------------------------------------------------------------
// Name: SelectedElement constructor
// Created: SBO 2005-10-26
// -----------------------------------------------------------------------------
SelectedElement::SelectedElement( PopulationFlowKnowledge& populationFlowKnowledge )
{
    Init();
    pPopulationFlowKnowledge_ = &populationFlowKnowledge;
}

// -----------------------------------------------------------------------------
// Name: SelectedElement constructor
// Created: APE 2004-05-05
// -----------------------------------------------------------------------------
SelectedElement::SelectedElement( TacticalLine_ABC& line, int nLinePoint )
{
    Init();
    pLine_ = &line;
    nLinePoint_ = nLinePoint;
}


// -----------------------------------------------------------------------------
// Name: SelectedElement constructor
// Created: APE 2004-05-12
// -----------------------------------------------------------------------------
SelectedElement::SelectedElement( RC& rc )
{
    Init();
    pRC_ = &rc;
}


// -----------------------------------------------------------------------------
// Name: SelectedElement destructor
// Created: APE 2004-05-05
// -----------------------------------------------------------------------------
SelectedElement::~SelectedElement()
{
}


// -----------------------------------------------------------------------------
// Name: SelectedElement::Init
// Created: APE 2004-05-12
// -----------------------------------------------------------------------------
void SelectedElement::Init()
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
// Name: SelectedElement::operator==
// Created: AGE 2005-04-05
// -----------------------------------------------------------------------------
bool SelectedElement::operator==( const SelectedElement& rhs ) const
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
// Name: SelectedElement::Dump
// Created: AGE 2005-04-05
// -----------------------------------------------------------------------------
void SelectedElement::Dump()
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
