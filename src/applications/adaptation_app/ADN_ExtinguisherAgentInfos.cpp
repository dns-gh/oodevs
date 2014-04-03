// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_ExtinguisherAgentInfos.h"
#include "ADN_WorkspaceElement.h"
#include "protocol/Protocol.h"

// -----------------------------------------------------------------------------
// Name: ADN_ExtinguisherAgentInfos constructor
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
ADN_ExtinguisherAgentInfos::ADN_ExtinguisherAgentInfos( ADN_Resources_Data::CategoryInfo* agent )
    : ADN_CrossedRef( ADN_Workspace::GetWorkspace().GetResources().GetData().GetResource( sword::dotation_type_extinguisher_agent ).categories_, agent, true )
    , heatDecreaseRate_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_ExtinguisherAgentInfos destructor
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
ADN_ExtinguisherAgentInfos::~ADN_ExtinguisherAgentInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_ExtinguisherAgentInfos::CopyFrom
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
void ADN_ExtinguisherAgentInfos::CopyFrom( ADN_ExtinguisherAgentInfos& agent )
{
    heatDecreaseRate_ = agent.heatDecreaseRate_.GetData();
    SetCrossedElement( agent.GetCrossedElement() );
}

// -----------------------------------------------------------------------------
// Name: ADN_ExtinguisherAgentInfos::ReadArchive
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
void ADN_ExtinguisherAgentInfos::ReadArchive( xml::xistream& xis )
{
    xis >> xml::attribute( "heat-decrease-rate", heatDecreaseRate_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_ExtinguisherAgentInfos::WriteArchive
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
void ADN_ExtinguisherAgentInfos::WriteArchive( xml::xostream& xos, const std::string& tag ) const
{
    xos << xml::start( tag )
            << xml::attribute( "agent", *this )
            << xml::attribute( "heat-decrease-rate", heatDecreaseRate_ )
        << xml::end;
}
