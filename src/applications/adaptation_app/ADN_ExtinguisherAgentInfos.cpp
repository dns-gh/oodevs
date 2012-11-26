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
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: ADN_ExtinguisherAgentInfos constructor
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
ADN_ExtinguisherAgentInfos::ADN_ExtinguisherAgentInfos( ADN_Resources_Data::CategoryInfo* agent )
    : ptrAgent_        ( ADN_Workspace::GetWorkspace().GetResources().GetData().GetResource( eDotationFamily_AgentExtincteur ).categories_, agent )
    , heatDecreaseRate_( 0 )
{
    BindExistenceTo( &ptrAgent_ );
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
    ptrAgent_ = agent.ptrAgent_.GetData();
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
void ADN_ExtinguisherAgentInfos::WriteArchive( xml::xostream& xos, const std::string& tag )
{
    xos << xml::start( tag )
            << xml::attribute( "agent", ptrAgent_.GetData()->strName_ )
            << xml::attribute( "heat-decrease-rate", heatDecreaseRate_ )
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_ExtinguisherAgentInfos::operator<<
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
xml::xostream& ADN_ExtinguisherAgentInfos::operator<<( xml::xostream& xos )
{
    WriteArchive( xos );
    return xos;
}
