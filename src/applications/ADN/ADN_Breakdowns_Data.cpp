// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-03-16 $
// $Archive: /MVW_v10/Build/SDK/ADN2/src/ADN_Breakdowns_Data.cpp $
// $Author: Nld $
// $Modtime: 27/04/05 10:57 $
// $Revision: 7 $
// $Workfile: ADN_Breakdowns_Data.cpp $
//
// *****************************************************************************

#include "ADN_pch.h"
#include "ADN_Breakdowns_Data.h"

#include "ADN_Workspace.h"
#include "ADN_Equipement_Data.h"
#include "ADN_Project_Data.h"
#include "ADN_OpenFile_Exception.h"
#include "ADN_DataException.h"
#include "ADN_Xml_Exception.h"
#include "ADN_XmlInput_Helper.h"
#include "ADN_SaveFile_Exception.h"
#include "ADN_Tools.h"
#include "ADN_Tr.h"


// -----------------------------------------------------------------------------
// Name: RepairPartInfo::RepairPartInfo
// Created: APE 2005-03-16
// -----------------------------------------------------------------------------
ADN_Breakdowns_Data::RepairPartInfo::RepairPartInfo()
: ADN_Ref_ABC         ()
, ADN_DataTreeNode_ABC()
, nNbr_               ( 0 )
, ptrPart_            ( ADN_Workspace::GetWorkspace().GetEquipements().GetData().GetDotation( eFamilleDotation_Piece ).categories_, 0 )
{
    this->BindExistenceTo( &ptrPart_ );
}


// -----------------------------------------------------------------------------
// Name: RepairPartInfo::GetNodeName
// Created: APE 2005-03-16
// -----------------------------------------------------------------------------
std::string ADN_Breakdowns_Data::RepairPartInfo::GetNodeName()
{
    return std::string();
}


// -----------------------------------------------------------------------------
// Name: RepairPartInfo::GetItemName
// Created: APE 2005-03-16
// -----------------------------------------------------------------------------
std::string ADN_Breakdowns_Data::RepairPartInfo::GetItemName()
{
    return std::string();
}


// -----------------------------------------------------------------------------
// Name: ADN_Breakdowns_Data::CreateCopy
// Created: APE 2005-04-07
// -----------------------------------------------------------------------------
ADN_Breakdowns_Data::RepairPartInfo* ADN_Breakdowns_Data::RepairPartInfo::CreateCopy()
{
    RepairPartInfo* pCopy = new RepairPartInfo();
    pCopy->ptrPart_ = ptrPart_.GetData();
    pCopy->nNbr_ = nNbr_.GetData();

    return pCopy;
}


// -----------------------------------------------------------------------------
// Name: RepairPartInfo::ReadArchive
// Created: APE 2005-03-16
// -----------------------------------------------------------------------------
void ADN_Breakdowns_Data::RepairPartInfo::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section( "Piece" );
    std::string strCategoryName;
    input.ReadAttribute( "categorie", strCategoryName );

    ADN_Equipement_Data::CategoryInfo* pCategory = ADN_Workspace::GetWorkspace().GetEquipements().GetData().GetDotation( eFamilleDotation_Piece ).FindCategory( strCategoryName );
    if( pCategory == 0 )
        input.ThrowError( tr( "Repair part '%1' does not exist." ).arg( strCategoryName.c_str() ).ascii() );

    ptrPart_ = pCategory;

    int nNbr;
    input.Read( nNbr );
    nNbr_ = nNbr;
    input.EndSection(); // Piece
}


// -----------------------------------------------------------------------------
// Name: RepairPartInfo::WriteArchive
// Created: APE 2005-03-16
// -----------------------------------------------------------------------------
void ADN_Breakdowns_Data::RepairPartInfo::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( "Piece" );
    output.WriteAttribute( "categorie", ptrPart_.GetData()->strName_.GetData() );
    output << nNbr_.GetData();
    output.EndSection(); // Piece
}

// -----------------------------------------------------------------------------
// Name: BreakdownInfo::BreakdownInfo
// Created: APE 2005-03-16
// -----------------------------------------------------------------------------
ADN_Breakdowns_Data::BreakdownInfo::BreakdownInfo()
: ADN_Ref_ABC          ()
, ADN_DataTreeNode_ABC ()
, nId_                 ( ADN_Workspace::GetWorkspace().GetBreakdowns().GetData().GetNextId() )
, repairTime_          ( "0s" )
, repairTimeVariance_  ( "0s" )
{
}


// -----------------------------------------------------------------------------
// Name: BreakdownInfo::~BreakdownInfo
// Created: APE 2005-03-17
// -----------------------------------------------------------------------------
ADN_Breakdowns_Data::BreakdownInfo::~BreakdownInfo()
{
    MT_DELETEOWNED( vRepairParts_ );
}


// -----------------------------------------------------------------------------
// Name: BreakdownInfo::GetNodeName
// Created: APE 2005-03-16
// -----------------------------------------------------------------------------
std::string ADN_Breakdowns_Data::BreakdownInfo::GetNodeName()
{
    return std::string();
}


// -----------------------------------------------------------------------------
// Name: BreakdownInfo::GetItemName
// Created: APE 2005-03-16
// -----------------------------------------------------------------------------
std::string ADN_Breakdowns_Data::BreakdownInfo::GetItemName()
{
    return std::string();
}


// -----------------------------------------------------------------------------
// Name: BreakdownInfo::CreateCopy
// Created: APE 2005-03-18
// -----------------------------------------------------------------------------
ADN_Breakdowns_Data::BreakdownInfo* ADN_Breakdowns_Data::BreakdownInfo::CreateCopy()
{
    BreakdownInfo* pCopy = new BreakdownInfo();
    pCopy->strName_ = tr( "New breakdown" ).ascii();
    pCopy->nType_ = nType_.GetData();
    pCopy->nNTI_  = nNTI_.GetData();
    pCopy->repairTime_ = repairTime_.GetData();
    pCopy->repairTimeVariance_ = repairTimeVariance_.GetData();

    for( IT_RepairPartInfoVector it = vRepairParts_.begin(); it != vRepairParts_.end(); ++it )
        pCopy->vRepairParts_.AddItem( (*it)->CreateCopy() );

    return pCopy;
}


// -----------------------------------------------------------------------------
// Name: BreakdownInfo::ReadArchive
// Created: APE 2005-03-16
// -----------------------------------------------------------------------------
void ADN_Breakdowns_Data::BreakdownInfo::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section( "Panne" );

    input.ReadAttribute( "nom", strName_ );
    input.ReadAttribute( "type", nType_, ADN_Tr::ConvertToBreakdownType, ADN_XmlInput_Helper::eThrow );

    input.Section( "Reparation" );
    input.ReadAttribute( "tempsMoyen", repairTime_ );
    input.ReadAttribute( "variance", repairTimeVariance_ );
    input.EndSection(); // Reparation

    input.BeginList( "Pieces" );
    while( input.NextListElement() )
    {
        std::auto_ptr<RepairPartInfo> spNew( new RepairPartInfo() );
        spNew->ReadArchive( input );
        vRepairParts_.AddItem( spNew.release() );
    }
    input.EndList(); // Pieces

    input.EndSection(); // Panne
}


// -----------------------------------------------------------------------------
// Name: BreakdownInfo::WriteArchive
// Created: APE 2005-03-16
// -----------------------------------------------------------------------------
void ADN_Breakdowns_Data::BreakdownInfo::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( "Panne" );

    output.WriteAttribute( "nom", strName_.GetData() );
    output.WriteAttribute( "type", ADN_Tr::ConvertFromBreakdownType( nType_.GetData() ) );
    output.WriteField( "MosID", nId_.GetData() );

    output.Section( "Reparation" );
    output.WriteAttribute( "tempsMoyen", repairTime_.GetData() );
    output.WriteAttribute( "variance", repairTimeVariance_.GetData() );
    output.EndSection(); // Reparation

    output.BeginList( "Pieces", vRepairParts_.size() );
    for( IT_RepairPartInfoVector it = vRepairParts_.begin(); it != vRepairParts_.end(); ++it )
        (*it)->WriteArchive( output );
    output.EndList(); // Pieces

    output.EndSection(); // Panne
}

// -----------------------------------------------------------------------------
// Name: ADN_Breakdowns_Data constructor
// Created: APE 2005-03-17
// -----------------------------------------------------------------------------
ADN_Breakdowns_Data::ADN_Breakdowns_Data()
: ADN_Data_ABC ()
, nNextId_     ( 1 )
, vBreakdowns_ ()
{
}


// -----------------------------------------------------------------------------
// Name: ADN_Breakdowns_Data destructor
// Created: APE 2005-03-17
// -----------------------------------------------------------------------------
ADN_Breakdowns_Data::~ADN_Breakdowns_Data()
{
}


// -----------------------------------------------------------------------------
// Name: ADN_Breakdowns_Data::FilesNeeded
// Created: APE 2005-03-17
// -----------------------------------------------------------------------------
void ADN_Breakdowns_Data::FilesNeeded( T_StringList& vFiles ) const
{
    vFiles.push_back( ADN_Workspace::GetWorkspace().GetProject().GetData().GetDataInfos().szBreakdowns_.GetData() );
}


// -----------------------------------------------------------------------------
// Name: ADN_Breakdowns_Data::GetNextId
// Created: APE 2005-03-18
// -----------------------------------------------------------------------------
int ADN_Breakdowns_Data::GetNextId()
{
    return nNextId_++;
}


// -----------------------------------------------------------------------------
// Name: ADN_Breakdowns_Data::Reset
// Created: APE 2005-03-17
// -----------------------------------------------------------------------------
void ADN_Breakdowns_Data::Reset()
{
    nNextId_ = 1;
    vBreakdowns_.Reset();
}


// -----------------------------------------------------------------------------
// Name: ADN_Breakdowns_Data::FindBreakdown
// Created: APE 2005-04-27
// -----------------------------------------------------------------------------
ADN_Breakdowns_Data::BreakdownInfo* ADN_Breakdowns_Data::FindBreakdown( const std::string& strName )
{
    IT_BreakdownInfoVector it = std::find_if( vBreakdowns_.begin(), vBreakdowns_.end(), ADN_Tools::NameCmp<BreakdownInfo>( strName ) );
    if( it == vBreakdowns_.end() )
        return 0;
    return *it;
}

// -----------------------------------------------------------------------------
// Name: ADN_Breakdowns_Data::ReadArchive
// Created: APE 2005-03-17
// -----------------------------------------------------------------------------
void ADN_Breakdowns_Data::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section( "Pannes" );
    input.ReadField( "TempsDiagnostique", strAverageDiagnosticTime_ );

    input.Section( "Types" );
    
    for( uint i = 0; i < eNbrBreakdownNTI; ++i )
    {
        input.BeginList( ADN_Tr::ConvertFromBreakdownNTI( ( E_BreakdownNTI )i ) );
        while( input.NextListElement() )
        {
            std::auto_ptr<BreakdownInfo> spNew( new BreakdownInfo() );
            spNew->ReadArchive( input );
            spNew->nNTI_ = ( E_BreakdownNTI )i;
            vBreakdowns_.AddItem( spNew.release() );
        }
        input.EndList();
    }

    input.EndSection(); // Types

    input.EndSection(); // Pannes
}


// -----------------------------------------------------------------------------
// Name: ADN_Breakdowns_Data::WriteArchive
// Created: APE 2005-03-17
// -----------------------------------------------------------------------------
void ADN_Breakdowns_Data::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( "Pannes" );
    output.WriteField( "TempsDiagnostique", strAverageDiagnosticTime_.GetData() );

    output.Section( "Types" );

    for( uint i = 0; i < eNbrBreakdownNTI; ++i )
    {
        output.BeginList( ADN_Tr::ConvertFromBreakdownNTI( ( E_BreakdownNTI )i ), vBreakdowns_.size() );
        for( IT_BreakdownInfoVector it = vBreakdowns_.begin(); it != vBreakdowns_.end(); ++it )
            if( (*it)->nNTI_ == ( E_BreakdownNTI )i )
                (*it)->WriteArchive( output );
        output.EndList();
    }

    output.EndSection(); // Types

    output.EndSection(); // Pannes
}



