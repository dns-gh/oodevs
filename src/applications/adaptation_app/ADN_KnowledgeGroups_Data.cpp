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
: ADN_Ref_ABC   ()
, maxLifetime_        ( "0s" )
, rMaxDistance_       ( -1 )
, bInterpolationTime_ ( false )
, interpolationTime_  ( "0s" )
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
            >> xml::optional() >> xml::attribute( "max-unit-to-knowledge-distance", rMaxDistance_ )
            >> xml::optional() >> xml::attribute( "interpolation-time", interpolationTime_ )
          >> xml::end();
    bInterpolationTime_ = interpolationTime_ != "0s";
}

// -----------------------------------------------------------------------------
// Name: ADN_KnowledgeGroups_Data::AgentGroupInfo::WriteArchive
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
void ADN_KnowledgeGroups_Data::AgentGroupInfo::WriteArchive( xml::xostream& output )
{
    output << xml::start( "unit-knowledge" )
            << xml::attribute( "max-lifetime", maxLifetime_ );
    if( rMaxDistance_.GetData() > 0 )
        output << xml::attribute( "max-unit-to-knowledge-distance", rMaxDistance_ );
    if( bInterpolationTime_.GetData() )
        output << xml::attribute( "interpolation-time", interpolationTime_ );
    output << xml::end();
}

// -----------------------------------------------------------------------------
// Name: ADN_KnowledgeGroups_Data::PopulationGroupInfo::PopulationGroupInfo
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
ADN_KnowledgeGroups_Data::PopulationGroupInfo::PopulationGroupInfo()
: ADN_Ref_ABC   ()
, maxLifetime_        ( "0s" )
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
          >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: ADN_KnowledgeGroups_Data::PopulationGroupInfo::WriteArchive
// Created: SBO 2005-10-24
// -----------------------------------------------------------------------------
void ADN_KnowledgeGroups_Data::PopulationGroupInfo::WriteArchive( xml::xostream& output )
{
    output << xml::start( "population-knowledge" )
            << xml::attribute( "max-lifetime", maxLifetime_ )
          << xml::end();
}

// -----------------------------------------------------------------------------
// Name: GroupInfo::GroupInfo
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
ADN_KnowledgeGroups_Data::GroupInfo::GroupInfo()
: ADN_Ref_ABC         ()
, ADN_DataTreeNode_ABC()
, agentInfos_         ()
, populationInfos_    ()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GroupInfo::GetNodeName
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
std::string ADN_KnowledgeGroups_Data::GroupInfo::GetNodeName()
{
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: GroupInfo::GetItemName
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
std::string ADN_KnowledgeGroups_Data::GroupInfo::GetItemName()
{
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: GroupInfo::ReadArchive
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
void ADN_KnowledgeGroups_Data::GroupInfo::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "name", strName_ );
    agentInfos_     .ReadArchive( input );
    populationInfos_.ReadArchive( input );
}

// -----------------------------------------------------------------------------
// Name: GroupInfo::WriteArchive
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
void ADN_KnowledgeGroups_Data::GroupInfo::WriteArchive( xml::xostream& output )
{
    output << xml::start( "knowledge-group" )
            << xml::attribute( "name", strName_ );
    agentInfos_     .WriteArchive( output );
    populationInfos_.WriteArchive( output );
    output << xml::end();
}

// -----------------------------------------------------------------------------
// Name: ADN_KnowledgeGroups_Data constructor
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
ADN_KnowledgeGroups_Data::ADN_KnowledgeGroups_Data()
: ADN_Data_ABC()
{
}

// -----------------------------------------------------------------------------
// Name: ADN_KnowledgeGroups_Data destructor
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
ADN_KnowledgeGroups_Data::~ADN_KnowledgeGroups_Data()
{
}

// -----------------------------------------------------------------------------
// Name: ADN_KnowledgeGroups_Data::FilesNeeded
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
void ADN_KnowledgeGroups_Data::FilesNeeded( T_StringList& vFiles ) const
{
    vFiles.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szKnowledgeGroups_.GetData() );
}

// -----------------------------------------------------------------------------
// Name: ADN_KnowledgeGroups_Data::Reset
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
void ADN_KnowledgeGroups_Data::Reset()
{
    vGroups_.Reset();
}

// -----------------------------------------------------------------------------
// Name: ADN_KnowledgeGroups_Data::ReadArchive
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
void ADN_KnowledgeGroups_Data::ReadArchive( xml::xistream& input )
{
    input >> xml::start( "knowledge-groups" )
            >> xml::list( "knowledge-group", *this, &ADN_KnowledgeGroups_Data::ReadKnowledgeGroup )
          >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: ADN_KnowledgeGroups_Data::ReadKnowledgeGroup
// Created: AGE 2007-08-16
// -----------------------------------------------------------------------------
void ADN_KnowledgeGroups_Data::ReadKnowledgeGroup( xml::xistream& input )
{
    std::auto_ptr<GroupInfo> spNew( new GroupInfo() );
    spNew->ReadArchive( input );
    vGroups_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_KnowledgeGroups_Data::WriteArchive
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
void ADN_KnowledgeGroups_Data::WriteArchive( xml::xostream& output )
{
    output << xml::start( "knowledge-groups" );
    for( IT_GroupInfoVector it = vGroups_.begin(); it != vGroups_.end(); ++it )
        (*it)->WriteArchive( output );
    output << xml::end();
}
