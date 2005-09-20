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
#include "MOS_Object_ABC.h"
#include "MOS_AgentKnowledge.h"
#include "MOS_ObjectKnowledge.h"

// -----------------------------------------------------------------------------
// Name: MOS_SelectedElement constructor
// Created: APE 2004-05-05
// -----------------------------------------------------------------------------
MOS_SelectedElement::MOS_SelectedElement()
{
    this->Init();
}


// -----------------------------------------------------------------------------
// Name: MOS_SelectedElement constructor
/** @param  agent 
*/
// Created: APE 2004-05-05
// -----------------------------------------------------------------------------
MOS_SelectedElement::MOS_SelectedElement( MOS_Agent& agent )
{
    this->Init();
    pAgent_ = &agent;
}


// -----------------------------------------------------------------------------
// Name: MOS_SelectedElement constructor
/** @param  gtia 
*/
// Created: APE 2004-05-05
// -----------------------------------------------------------------------------
MOS_SelectedElement::MOS_SelectedElement( MOS_Gtia& gtia )
{
    this->Init();
    pGtia_ = &gtia;
}


// -----------------------------------------------------------------------------
// Name: MOS_SelectedElement constructor
/** @param  team 
*/
// Created: APE 2004-05-05
// -----------------------------------------------------------------------------
MOS_SelectedElement::MOS_SelectedElement( MOS_Team& team )
{
    this->Init();
    pTeam_ = &team;
}


// -----------------------------------------------------------------------------
// Name: MOS_SelectedElement constructor
/** @param  object 
*/
// Created: APE 2004-05-05
// -----------------------------------------------------------------------------
MOS_SelectedElement::MOS_SelectedElement( MOS_Object_ABC& object )
{
    this->Init();
    pObject_ = &object;
}


// -----------------------------------------------------------------------------
// Name: MOS_SelectedElement constructor
/** @param  agentKnowledge 
*/
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
MOS_SelectedElement::MOS_SelectedElement( MOS_AgentKnowledge& agentKnowledge )
{
    this->Init();
    pAgentKnowledge_ = &agentKnowledge;
}


// -----------------------------------------------------------------------------
// Name: MOS_SelectedElement constructor
/** @param  ObjectKnowledge 
*/
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
MOS_SelectedElement::MOS_SelectedElement( MOS_ObjectKnowledge& ObjectKnowledge )
{
    this->Init();
    pObjectKnowledge_ = &ObjectKnowledge;
}


// -----------------------------------------------------------------------------
// Name: MOS_SelectedElement constructor
/** @param  line 
    @param  nLinePoint 
*/
// Created: APE 2004-05-05
// -----------------------------------------------------------------------------
MOS_SelectedElement::MOS_SelectedElement( MOS_TacticalLine_ABC& line, int nLinePoint )
{
    this->Init();
    pLine_ = &line;
    nLinePoint_ = nLinePoint;
}


// -----------------------------------------------------------------------------
// Name: MOS_SelectedElement constructor
/** @param  rc 
*/
// Created: APE 2004-05-12
// -----------------------------------------------------------------------------
MOS_SelectedElement::MOS_SelectedElement( MOS_RC& rc )
{
    this->Init();
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
    pAgent_               = 0;
    pGtia_                = 0;
    pTeam_                = 0;
    pObject_          = 0;
    pAgentKnowledge_      = 0;
    pObjectKnowledge_ = 0;
    pRC_                  = 0;
    pLine_                = 0;
    nLinePoint_           = -1;
}

// -----------------------------------------------------------------------------
// Name: MOS_SelectedElement::operator==
// Created: AGE 2005-04-05
// -----------------------------------------------------------------------------
bool MOS_SelectedElement::operator==( const MOS_SelectedElement& rhs ) const
{
    return pAgent_               == rhs.pAgent_
        && pGtia_                == rhs.pGtia_
        && pTeam_                == rhs.pTeam_
        && pObject_          == rhs.pObject_
        && pAgentKnowledge_      == rhs.pAgentKnowledge_
        && pObjectKnowledge_ == rhs.pObjectKnowledge_
        && pRC_                  == rhs.pRC_
        && pLine_                == rhs.pLine_
        && nLinePoint_           == rhs.nLinePoint_;
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
        str << " agent : "            << pAgent_->GetAgentID()          << "; ";
    if( pGtia_ )
        str << " gtia : "             << pGtia_->GetID()                << "; ";
    if( pTeam_ )
        str << " team : "             << pTeam_->GetID()                << "; ";
    if( pObject_ )
        str << " object : "           << pObject_->GetID()          << "; ";
    if( pAgentKnowledge_ )
        str << " agent knowledge : "  << pAgentKnowledge_->GetID()      << "; ";
    if( pObjectKnowledge_ )
        str << " object knowledge : " << pObjectKnowledge_->GetID() << "; ";
    MT_LOG_INFO_MSG( str.str() );
}
