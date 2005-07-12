// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-03-18 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_KnowledgeGroups_Data.cpp $
// $Author: Ape $
// $Modtime: 22/04/05 15:55 $
// $Revision: 2 $
// $Workfile: ADN_KnowledgeGroups_Data.cpp $
//
// *****************************************************************************

#include "ADN_pch.h"
#include "ADN_KnowledgeGroups_Data.h"

#include "ADN_Workspace.h"
#include "ADN_Project_Data.h"
#include "ADN_XmlInput_Helper.h"
#include "ADN_Tools.h"


// -----------------------------------------------------------------------------
// Name: GroupInfo::GroupInfo
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
ADN_KnowledgeGroups_Data::GroupInfo::GroupInfo()
: ADN_Ref_ABC         ()
, ADN_DataTreeNode_ABC()
, rMaxLifetime_       ( 0.0 )
, rMaxDistance_       ( 0.0 )
, bInterpolationTime_ ( false )
, rInterpolationTime_ ( 0.0 )
{
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
void ADN_KnowledgeGroups_Data::GroupInfo::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section( "GroupeConnaissance" );
    input.ReadAttribute( "nom", strName_ );

    input.Section( "Connaissance" );
    input.ReadTimeField( "DureeDeVieMax", rMaxLifetime_ );
    input.ReadField( "DistanceMaxEntreUniteReelleEtUniteConnue", rMaxDistance_ );
    if( input.ReadTimeField( "TempsInterpolation", rInterpolationTime_, ADN_XmlInput_Helper::eNothing ) )
        bInterpolationTime_ = true;
    input.EndSection(); // Connaissance

    input.EndSection(); // GroupeConnaissance
}


// -----------------------------------------------------------------------------
// Name: GroupInfo::WriteArchive
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
void ADN_KnowledgeGroups_Data::GroupInfo::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( "GroupeConnaissance" );
    output.WriteAttribute( "nom", strName_.GetData() );

    output.Section( "Connaissance" );
    output.WriteField( "DureeDeVieMax", ADN_Tools::SecondToString( rMaxLifetime_.GetData() ) );
    output.WriteField( "DistanceMaxEntreUniteReelleEtUniteConnue", rMaxDistance_.GetData() );
    if( bInterpolationTime_.GetData() )
        output.WriteField( "TempsInterpolation", ADN_Tools::SecondToString( rInterpolationTime_.GetData() ) );
    output.EndSection(); // Connaissance

    output.EndSection(); // GroupeConnaissance
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
    vFiles.push_back( ADN_Workspace::GetWorkspace().GetProject().GetData().GetDataInfos().szKnowledgeGroups_.GetData() );
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
void ADN_KnowledgeGroups_Data::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.BeginList( "GroupesConnaissance" );
    while( input.NextListElement() )
    {
        std::auto_ptr<GroupInfo> spNew( new GroupInfo() );
        spNew->ReadArchive( input );
        vGroups_.AddItem( spNew.release() );
    }
    input.EndList(); // GroupesConnaissance
}


// -----------------------------------------------------------------------------
// Name: ADN_KnowledgeGroups_Data::WriteArchive
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
void ADN_KnowledgeGroups_Data::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.BeginList( "GroupesConnaissance", vGroups_.size() );
    for( IT_GroupInfoVector it = vGroups_.begin(); it != vGroups_.end(); ++it )
        (*it)->WriteArchive( output );
    output.EndList(); // GroupesConnaissance
}
