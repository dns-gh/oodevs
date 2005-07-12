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
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Automata_Data.cpp $
// $Author: Ape $
// $Modtime: 7/06/05 9:51 $
// $Revision: 15 $
// $Workfile: ADN_Automata_Data.cpp $
//
// *****************************************************************************

#include "ADN_pch.h"
#include "ADN_Automata_Data.h"

#include "ADN_Workspace.h"
#include "ADN_Project_Data.h"
#include "ADN_OpenFile_Exception.h"
#include "ADN_DataException.h"
#include "ADN_Xml_Exception.h"
#include "ADN_XmlInput_Helper.h"
#include "ADN_SaveFile_Exception.h"

#include "ADN_Tr.h"


// -----------------------------------------------------------------------------
// Name: UnitInfos::UnitInfos
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
ADN_Automata_Data::UnitInfos::UnitInfos()
: ADN_Ref_ABC()
, ADN_DataTreeNode_ABC()
, ptrUnit_( ADN_Workspace::GetWorkspace().GetUnits().GetData().GetUnitsInfos(), 0 )
, strNbrRegExp_( "1" )
{
    BindExistenceTo( &ptrUnit_ );
}


// -----------------------------------------------------------------------------
// Name: UnitInfos::GetNodeName
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
std::string ADN_Automata_Data::UnitInfos::GetNodeName()
{
    return std::string();
}


// -----------------------------------------------------------------------------
// Name: UnitInfos::GetItemName
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
std::string ADN_Automata_Data::UnitInfos::GetItemName()
{
    return std::string();
}


// -----------------------------------------------------------------------------
// Name: UnitInfos::CreateCopy
// Created: APE 2005-02-14
// -----------------------------------------------------------------------------
ADN_Automata_Data::UnitInfos* ADN_Automata_Data::UnitInfos::CreateCopy()
{
    UnitInfos* pCopy = new UnitInfos();
    pCopy->ptrUnit_ = ptrUnit_.GetData();
    pCopy->strNbrRegExp_ = strNbrRegExp_.GetData();
    return pCopy;
}


// -----------------------------------------------------------------------------
// Name: UnitInfos::ReadArchive
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
void ADN_Automata_Data::UnitInfos::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section( "Pion" );

    std::string strName;
    input.ReadAttribute( "nom", strName );
    ADN_Units_Data::UnitInfos* pUnit = ADN_Workspace::GetWorkspace().GetUnits().GetData().FindUnit( strName );
    assert( pUnit != 0 );
    ptrUnit_ = pUnit;

    std::string strNbrRegExp;
    input.Read( strNbrRegExp );
    strNbrRegExp_ = strNbrRegExp;

    input.EndSection(); // Pion
}


// -----------------------------------------------------------------------------
// Name: UnitInfos::WriteArchive
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
void ADN_Automata_Data::UnitInfos::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( "Pion" );
    output.WriteAttribute( "nom", ptrUnit_.GetData()->strName_.GetData() );
    output << strNbrRegExp_.GetData();
    output.EndSection(); // Pion
}


// -----------------------------------------------------------------------------
// Name: AutomatonInfos::AutomatonInfos
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
ADN_Automata_Data::AutomatonInfos::AutomatonInfos()
: ADN_Ref_ABC()
, ADN_DataTreeNode_ABC()
, ptrUnit_( ADN_Workspace::GetWorkspace().GetUnits().GetData().GetUnitsInfos(), 0 )
, ptrModel_( ADN_Workspace::GetWorkspace().GetModels().GetData().GetAutomataModelsInfos(), 0 )
, bStrengthRatioFeedbackTime_( false )
, rStrengthRatioFeedbackTime_( 0.0 )
{
    this->BindExistenceTo( &ptrUnit_ );
    this->BindExistenceTo( &ptrModel_ );
}


// -----------------------------------------------------------------------------
// Name: AutomatonInfos::~AutomatonInfos
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
ADN_Automata_Data::AutomatonInfos::~AutomatonInfos()
{
    vSubUnits_.Reset();
}


// -----------------------------------------------------------------------------
// Name: AutomatonInfos::GetNodeName
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
std::string ADN_Automata_Data::AutomatonInfos::GetNodeName()
{
    return std::string();
}


// -----------------------------------------------------------------------------
// Name: AutomatonInfos::GetItemName
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
std::string ADN_Automata_Data::AutomatonInfos::GetItemName()
{
    return std::string();
}


// -----------------------------------------------------------------------------
// Name: AutomatonInfos::CreateCopy
// Created: APE 2005-02-14
// -----------------------------------------------------------------------------
ADN_Automata_Data::AutomatonInfos* ADN_Automata_Data::AutomatonInfos::CreateCopy()
{
    AutomatonInfos* pCopy = new AutomatonInfos();

    pCopy->nAgentType_ = nAgentType_.GetData();
    pCopy->ptrModel_ = ptrModel_.GetData();
    pCopy->ptrUnit_ = ptrUnit_.GetData();
    pCopy->bStrengthRatioFeedbackTime_ = bStrengthRatioFeedbackTime_.GetData();
    pCopy->rStrengthRatioFeedbackTime_ = rStrengthRatioFeedbackTime_.GetData();

    for( IT_UnitInfosVector it = vSubUnits_.begin(); it != vSubUnits_.end(); ++it )
        pCopy->vSubUnits_.AddItem( (*it)->CreateCopy() );

    return pCopy;
}

// -----------------------------------------------------------------------------
// Name: AutomatonInfos::ReadArchive
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
void ADN_Automata_Data::AutomatonInfos::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section( "Unite" );
    input.ReadAttribute( "nom", strName_ );

    std::string strType;
    input.ReadAttribute( "type", strType );
    nAgentType_ = ADN_Tr::ConvertToAgentTypeAutomate( strType );

    input.Section( "Automate" );

    std::string strModel;
    input.ReadField( "ModeleDecisionnel", strModel );
    ADN_Models_Data::ModelInfos* pModel = ADN_Workspace::GetWorkspace().GetModels().GetData().FindAutomataModel( strModel );
    assert( pModel != 0 );
    ptrModel_ = pModel;

    input.BeginList( "Constitution" );
    while( input.NextListElement() )
    {
        std::auto_ptr<UnitInfos> spNew( new UnitInfos() );
        spNew->ReadArchive( input );
        vSubUnits_.AddItem( spNew.release() );
    }
    input.EndList(); // Constitution

    if( input.Section( "RapportDeForce", ADN_XmlInput_Helper::eNothing ) )
    {
        bStrengthRatioFeedbackTime_ = true;
        input.ReadTimeField( "TempsDeRemontee", rStrengthRatioFeedbackTime_ );
        input.EndSection(); // RapportDeForce
    }

    input.EndSection(); // Automate
    input.Section( "PionPC" );

    std::string strUnit;
    input.ReadAttribute( "type", strUnit );
    ADN_Units_Data::UnitInfos* pUnit = ADN_Workspace::GetWorkspace().GetUnits().GetData().FindUnit( strUnit );
    assert( pUnit != 0 );
    ptrUnit_ = pUnit;

    input.EndSection(); // PionPC
    input.EndSection(); // Unite
}


// -----------------------------------------------------------------------------
// Name: AutomatonInfos::WriteArchive
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
void ADN_Automata_Data::AutomatonInfos::WriteArchive( MT_OutputArchive_ABC& output, int nMosId )
{
    output.Section( "Unite" );
    output.WriteAttribute( "nom", strName_.GetData() );
    output.WriteAttribute( "type", ADN_Tr::ConvertFromAgentTypeAutomate( nAgentType_.GetData() ) );

    output.WriteField( "MosID", nMosId );

    output.Section( "Automate" );
    output.WriteField( "ModeleDecisionnel", ptrModel_.GetData()->strName_.GetData() );

    output.BeginList( "Constitution", vSubUnits_.size() );
    for( IT_UnitInfosVector it = vSubUnits_.begin(); it != vSubUnits_.end(); ++it )
    {
        (*it)->WriteArchive( output );
    }
    output.EndList(); // Constitution

    if( bStrengthRatioFeedbackTime_.GetData() )
    {
        output.Section( "RapportDeForce" );
        output.WriteField( "TempsDeRemontee", ADN_Tools::SecondToString( rStrengthRatioFeedbackTime_.GetData() ) );
        output.EndSection(); // RapportDeForce
    }

    output.EndSection(); // Automate

    output.Section( "PionPC" );
    output.WriteAttribute( "type", ptrUnit_.GetData()->strName_.GetData() );
    output.EndSection(); // PionPC

    output.EndSection(); // Unite
}


// -----------------------------------------------------------------------------
// Name: ADN_Automata_Data constructor
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
ADN_Automata_Data::ADN_Automata_Data()
: ADN_Data_ABC()
{
}


// -----------------------------------------------------------------------------
// Name: ADN_Automata_Data destructor
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
ADN_Automata_Data::~ADN_Automata_Data()
{
    this->Reset();
}


// -----------------------------------------------------------------------------
// Name: ADN_Automata_Data::FilesNeeded
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
void ADN_Automata_Data::FilesNeeded( T_StringList& vFiles ) const
{
    vFiles.push_back( ADN_Workspace::GetWorkspace().GetProject().GetData().GetDataInfos().szAutomata_.GetData() );
}


// -----------------------------------------------------------------------------
// Name: ADN_Automata_Data::Reset
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
void ADN_Automata_Data::Reset()
{
    vAutomata_.Reset();
}


// -----------------------------------------------------------------------------
// Name: ADN_Automata_Data::ReadArchive
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
void ADN_Automata_Data::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.BeginList( "Automates" );
    while( input.NextListElement() )
    {
        std::auto_ptr<AutomatonInfos> spNew( new AutomatonInfos() );
        spNew->ReadArchive( input );
        vAutomata_.AddItem( spNew.release() );
    }
    input.EndList(); // Automates
}


// -----------------------------------------------------------------------------
// Name: ADN_Automata_Data::WriteArchive
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
void ADN_Automata_Data::WriteArchive( MT_OutputArchive_ABC& output )
{
    int nMosBaseId = ADN_Workspace::GetWorkspace().GetUnits().GetData().GetUnitsInfos().size() + 1;
    output.BeginList( "Automates", vAutomata_.size() );
    int n = 0;
    for( IT_AutomatonInfosVector it = vAutomata_.begin(); it != vAutomata_.end(); ++it, ++n )
    {
        (*it)->WriteArchive( output, nMosBaseId + n );
    }
    output.EndList(); // Automates
}


// -----------------------------------------------------------------------------
// Name: ADN_Automata_Data::GetAutomataThatUse
// Created: APE 2005-04-25
// -----------------------------------------------------------------------------
std::string ADN_Automata_Data::GetAutomataThatUse( ADN_Units_Data::UnitInfos& unit )
{
    std::string strResult;
    for( IT_AutomatonInfosVector it = vAutomata_.begin(); it != vAutomata_.end(); ++it )
    {
        AutomatonInfos* pAutomaton = *it;
        if( pAutomaton->ptrUnit_ == &unit )
        {
            if( strResult != "" )
                strResult += "<br>";
            strResult += pAutomaton->strName_.GetData();
        }

        for( IT_UnitInfosVector it2 = pAutomaton->vSubUnits_.begin(); it2 != pAutomaton->vSubUnits_.end(); ++it2 )
        {
            if( (*it2)->ptrUnit_.GetData() == &unit  )
            {
                if( strResult != "" )
                    strResult += "<br>";
                strResult += pAutomaton->strName_.GetData();
                break;
            }
        }
    }
    return strResult;
}
