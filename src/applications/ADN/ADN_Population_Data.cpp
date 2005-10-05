// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-12-02 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Population_Data.cpp $
// $Author: Ape $
// $Modtime: 7/06/05 9:51 $
// $Revision: 15 $
// $Workfile: ADN_Population_Data.cpp $
//
// *****************************************************************************

#include "ADN_pch.h"
#include "ADN_Population_Data.h"

#include "ADN_Workspace.h"
#include "ADN_Project_Data.h"
#include "ADN_OpenFile_Exception.h"
#include "ADN_DataException.h"
#include "ADN_Xml_Exception.h"
#include "ADN_XmlInput_Helper.h"
#include "ADN_SaveFile_Exception.h"

#include "ADN_Tr.h"


// -----------------------------------------------------------------------------
// Name: PopulationInfos::PopulationInfos
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
ADN_Population_Data::PopulationInfos::PopulationInfos()
: ADN_Ref_ABC            ()
, ADN_DataTreeNode_ABC   ()
, ptrModel_              ( ADN_Workspace::GetWorkspace().GetModels().GetData().GetPopulationModelsInfos(), 0 )
, rConcentrationDensity_ ( 0. )
, rMoveDensity_          ( 0. )
, rMoveSpeed_            ( 0. )
{
    this->BindExistenceTo( &ptrModel_ );
}


// -----------------------------------------------------------------------------
// Name: PopulationInfos::~PopulationInfos
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
ADN_Population_Data::PopulationInfos::~PopulationInfos()
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: PopulationInfos::GetNodeName
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
std::string ADN_Population_Data::PopulationInfos::GetNodeName()
{
    return std::string();
}


// -----------------------------------------------------------------------------
// Name: PopulationInfos::GetItemName
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
std::string ADN_Population_Data::PopulationInfos::GetItemName()
{
    return std::string();
}


// -----------------------------------------------------------------------------
// Name: PopulationInfos::CreateCopy
// Created: APE 2005-02-14
// -----------------------------------------------------------------------------
ADN_Population_Data::PopulationInfos* ADN_Population_Data::PopulationInfos::CreateCopy()
{
    PopulationInfos* pCopy = new PopulationInfos();

    pCopy->ptrModel_              = ptrModel_.GetData();
    pCopy->rConcentrationDensity_ = rConcentrationDensity_.GetData();
    pCopy->rMoveDensity_          = rMoveDensity_.GetData();
    pCopy->rMoveSpeed_            = rMoveSpeed_.GetData();

    return pCopy;
}

// -----------------------------------------------------------------------------
// Name: PopulationInfos::ReadArchive
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
void ADN_Population_Data::PopulationInfos::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section( "Population" );
    input.ReadAttribute( "nom", strName_ );

    std::string strModel;
    input.ReadField( "ModeleDecisionnel", strModel );
    ADN_Models_Data::ModelInfos* pModel = ADN_Workspace::GetWorkspace().GetModels().GetData().FindPopulationModel( strModel );
    assert( pModel != 0 );
    ptrModel_ = pModel;

    input.ReadField( "DensiteConcentration"      , rConcentrationDensity_ );
    input.ReadField( "DensiteNominaleDeplacement", rMoveDensity_          );
    input.ReadField( "VitesseDeplacement"        , rMoveSpeed_            );

    input.EndSection(); // Population
}


// -----------------------------------------------------------------------------
// Name: PopulationInfos::WriteArchive
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
void ADN_Population_Data::PopulationInfos::WriteArchive( MT_OutputArchive_ABC& output, int nMosId )
{
    output.Section( "Population" );
    output.WriteAttribute( "nom", strName_.GetData() );

    output.WriteField( "MosID", nMosId );
    output.WriteField( "ModeleDecisionnel", ptrModel_.GetData()->strName_.GetData() );

    output.WriteField( "DensiteConcentration"      , rConcentrationDensity_.GetData() );
    output.WriteField( "DensiteNominaleDeplacement", rMoveDensity_.GetData()          );
    output.WriteField( "VitesseDeplacement"        , rMoveSpeed_.GetData()            );

    output.EndSection(); // Population
}


// -----------------------------------------------------------------------------
// Name: ADN_Population_Data constructor
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
ADN_Population_Data::ADN_Population_Data()
: ADN_Data_ABC()
{
}


// -----------------------------------------------------------------------------
// Name: ADN_Population_Data destructor
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
ADN_Population_Data::~ADN_Population_Data()
{
    this->Reset();
}


// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::FilesNeeded
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
void ADN_Population_Data::FilesNeeded( T_StringList& vFiles ) const
{
    vFiles.push_back( ADN_Workspace::GetWorkspace().GetProject().GetData().GetDataInfos().szPopulation_.GetData() );
}


// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::Reset
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
void ADN_Population_Data::Reset()
{
    vPopulation_.Reset();
}


// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::ReadArchive
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
void ADN_Population_Data::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.BeginList( "Populations" );
    while( input.NextListElement() )
    {
        std::auto_ptr<PopulationInfos> spNew( new PopulationInfos() );
        spNew->ReadArchive( input );
        vPopulation_.AddItem( spNew.release() );
    }
    input.EndList(); // Populations
}


// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::WriteArchive
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
void ADN_Population_Data::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.BeginList( "Populations", vPopulation_.size() );
    int n = 0;
    for( IT_PopulationInfosVector it = vPopulation_.begin(); it != vPopulation_.end(); ++it, ++n )
    {
        (*it)->WriteArchive( output, n );
    }
    output.EndList(); // Automates
}
