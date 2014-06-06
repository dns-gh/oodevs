// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_KnowledgeGroups_Data.h"
#include "ADN_Workspace.h"
#include "ADN_Project_Data.h"
#include "ADN_Tools.h"

// -----------------------------------------------------------------------------
// Name: ADN_KnowledgeGroups_Data::AgentGroupInfo::AgentGroupInfo
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
ADN_KnowledgeGroups_Data::AgentGroupInfo::AgentGroupInfo()
    : ADN_Ref_ABC()
    , maxLifetime_       ( "0s" )
    , rMaxDistance_      ( -1 )
    , bInterpolationTime_( false )
    , interpolationTime_ ( "0s" )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_KnowledgeGroups_Data::AgentGroupInfo::ReadArchive
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
void ADN_KnowledgeGroups_Data::AgentGroupInfo::ReadArchive( xml::xistream& input )
{
    input >> xml::start( "unit-knowledge" )
            >> xml::attribute( "max-lifetime", maxLifetime_ )
            >> xml::optional >> xml::attribute( "max-unit-to-knowledge-distance", rMaxDistance_ )
            >> xml::optional >> xml::attribute( "interpolation-time", interpolationTime_ )
          >> xml::end;
    bInterpolationTime_ = interpolationTime_ != "0s";
}

// -----------------------------------------------------------------------------
// Name: ADN_KnowledgeGroups_Data::AgentGroupInfo::WriteArchive
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
void ADN_KnowledgeGroups_Data::AgentGroupInfo::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "unit-knowledge" )
            << xml::attribute( "max-lifetime", maxLifetime_ );
    if( rMaxDistance_.GetData() > 0 )
        output << xml::attribute( "max-unit-to-knowledge-distance", rMaxDistance_ );
    if( bInterpolationTime_.GetData() )
        output << xml::attribute( "interpolation-time", interpolationTime_ );
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_KnowledgeGroups_Data::PopulationGroupInfo::PopulationGroupInfo
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
ADN_KnowledgeGroups_Data::PopulationGroupInfo::PopulationGroupInfo()
    : ADN_Ref_ABC()
    , maxLifetime_( "0s" )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_KnowledgeGroups_Data::PopulationGroupInfo::ReadArchive
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
void ADN_KnowledgeGroups_Data::PopulationGroupInfo::ReadArchive( xml::xistream& input )
{
    input >> xml::start( "population-knowledge" )
            >> xml::attribute( "max-lifetime", maxLifetime_ )
          >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_KnowledgeGroups_Data::PopulationGroupInfo::WriteArchive
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
void ADN_KnowledgeGroups_Data::PopulationGroupInfo::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "population-knowledge" )
            << xml::attribute( "max-lifetime", maxLifetime_ )
          << xml::end;
}

// -----------------------------------------------------------------------------
// Name: GroupInfo::GroupInfo
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
ADN_KnowledgeGroups_Data::GroupInfo::GroupInfo()
    : agentInfos_        ()
    , populationInfos_   ()
    , communicationDelay_( "0s" ) // LTO
{
    strName_.SetContext( ADN_Workspace::GetWorkspace().GetContext( eKnowledgeGroups, "knowledge-groups" ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_KnowledgeGroups_Data::CreateCopy
// Created: LGY 2010-08-18
// -----------------------------------------------------------------------------
ADN_KnowledgeGroups_Data::GroupInfo* ADN_KnowledgeGroups_Data::GroupInfo::CreateCopy()
{
    GroupInfo* pCopy = new GroupInfo();
    pCopy->communicationDelay_ = communicationDelay_.GetData();
    pCopy->agentInfos_.maxLifetime_ = agentInfos_.maxLifetime_.GetData();
    pCopy->agentInfos_.rMaxDistance_ = agentInfos_.rMaxDistance_.GetData();
    pCopy->agentInfos_.bInterpolationTime_ = agentInfos_.bInterpolationTime_.GetData();
    pCopy->agentInfos_.interpolationTime_ = agentInfos_.interpolationTime_.GetData();
    pCopy->populationInfos_.maxLifetime_ = populationInfos_.maxLifetime_.GetData();
    return pCopy;
}

// -----------------------------------------------------------------------------
// Name: GroupInfo::ReadArchive
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
void ADN_KnowledgeGroups_Data::GroupInfo::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "name", *this )
          >> xml::optional >> xml::attribute( "communication-delay", communicationDelay_ ); // LTO
    agentInfos_.ReadArchive( input );
    populationInfos_.ReadArchive( input );
}

// -----------------------------------------------------------------------------
// Name: GroupInfo::WriteArchive
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
void ADN_KnowledgeGroups_Data::GroupInfo::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "knowledge-group" )
           << xml::attribute( "name", *this );
    if( communicationDelay_ != "0s" ) // LTO
        output << xml::attribute( "communication-delay", communicationDelay_ ); // LTO
    agentInfos_.WriteArchive( output );
    populationInfos_.WriteArchive( output );
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_KnowledgeGroups_Data constructor
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
ADN_KnowledgeGroups_Data::ADN_KnowledgeGroups_Data()
    : ADN_Data_ABC( eKnowledgeGroups )
{
    vGroups_.AddUniquenessChecker( eError, duplicateName_, &ADN_Tools::NameExtractor );
}

// -----------------------------------------------------------------------------
// Name: ADN_KnowledgeGroups_Data destructor
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
ADN_KnowledgeGroups_Data::~ADN_KnowledgeGroups_Data()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_KnowledgeGroups_Data::FilesNeeded
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
void ADN_KnowledgeGroups_Data::FilesNeeded( tools::Path::T_Paths& vFiles ) const
{
    vFiles.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szKnowledgeGroups_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_KnowledgeGroups_Data::ReadArchive
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
void ADN_KnowledgeGroups_Data::ReadArchive( xml::xistream& input )
{
    input >> xml::start( "knowledge-groups" )
            >> xml::list( "knowledge-group", *this, &ADN_KnowledgeGroups_Data::ReadKnowledgeGroup )
          >> xml::end;
    vGroups_.CheckValidity();
}

// -----------------------------------------------------------------------------
// Name: ADN_KnowledgeGroups_Data::ReadKnowledgeGroup
// Created: AGE 2007-08-16
// -----------------------------------------------------------------------------
void ADN_KnowledgeGroups_Data::ReadKnowledgeGroup( xml::xistream& input )
{
    std::unique_ptr<GroupInfo> spNew( new GroupInfo() );
    spNew->ReadArchive( input );
    vGroups_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_KnowledgeGroups_Data::WriteArchive
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
void ADN_KnowledgeGroups_Data::WriteArchive( xml::xostream& output ) const
{
    if( vGroups_.GetErrorStatus() == eError )
        throw MASA_EXCEPTION( GetInvalidDataErrorMsg() );

    output << xml::start( "knowledge-groups" );
    tools::SchemaWriter schemaWriter;
    schemaWriter.WritePhysicalSchema( output, "KnowledgeGroups" );
    for( auto it = vGroups_.begin(); it != vGroups_.end(); ++it )
        (*it)->WriteArchive( output );
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_KnowledgeGroups_Data::GetGroupInfos
// Created: LGY 2010-08-18
// -----------------------------------------------------------------------------
ADN_KnowledgeGroups_Data::T_GroupInfoVector& ADN_KnowledgeGroups_Data::GetGroupInfos()
{
    return vGroups_;
}
