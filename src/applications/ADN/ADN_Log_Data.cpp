// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-03-14 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Log_Data.cpp $
// $Author: Ape $
// $Modtime: 7/04/05 11:21 $
// $Revision: 4 $
// $Workfile: ADN_Log_Data.cpp $
//
// *****************************************************************************

#include "ADN_pch.h"
#include "ADN_Log_Data.h"

#include "ADN_Workspace.h"
#include "ADN_Project_Data.h"
#include "ADN_XmlInput_Helper.h"
#include "ADN_Xml_Exception.h"
#include "ADN_Tools.h"
#include "ADN_OpenFile_Exception.h"
#include "ADN_SaveFile_Exception.h"
#include "ADN_Enums.h"
#include "ENT/ENT_Tr.h"


// -----------------------------------------------------------------------------
// Name: WorkTimeModifiersInfo::WorkTimeModifiersInfo
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
ADN_Log_Data::WorkTimeModifiersInfo::WorkTimeModifiersInfo( E_TempsBordee nType )
: ADN_Ref_ABC         ()
, ADN_DataTreeNode_ABC()
, nType_              ( nType )
, rRepairModifier_    ( 0.0 )
, rSortModifier_      ( 0.0 )
, rTreatModifier_     ( 0.0 )
{
}


// -----------------------------------------------------------------------------
// Name: WorkTimeModifiersInfo::GetNodeName
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
std::string ADN_Log_Data::WorkTimeModifiersInfo::GetNodeName()
{
    return std::string();
}


// -----------------------------------------------------------------------------
// Name: WorkTimeModifiersInfo::GetItemName
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
std::string ADN_Log_Data::WorkTimeModifiersInfo::GetItemName()
{
    return std::string();
}


// -----------------------------------------------------------------------------
// Name: WorkTimeModifiersInfo::ReadArchive
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_Log_Data::WorkTimeModifiersInfo::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section( ADN_Tools::ComputeWorkingTimeScriptName( nType_ ) );
    input.ReadField( "CoefTempsReparation", rRepairModifier_ );
    input.ReadField( "CoefTempsSoin", rTreatModifier_ );
    input.ReadField( "CoefTempsTri", rSortModifier_ );
    input.EndSection();
}


// -----------------------------------------------------------------------------
// Name: WorkTimeModifiersInfo::WriteArchive
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_Log_Data::WorkTimeModifiersInfo::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( ADN_Tools::ComputeWorkingTimeScriptName( nType_ ) );
    output.WriteField( "CoefTempsReparation", rRepairModifier_.GetData() );
    output.WriteField( "CoefTempsSoin", rTreatModifier_.GetData() );
    output.WriteField( "CoefTempsTri", rSortModifier_.GetData() );
    output.EndSection();
}


// -----------------------------------------------------------------------------
// Name: ADN_Log_Data constructor
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
ADN_Log_Data::ADN_Log_Data()
: ADN_Data_ABC()
{
    for( int n = 0; n < eNbrTempsBordee; ++n )
        vWorkTimeModifiers_.AddItem( new WorkTimeModifiersInfo( (E_TempsBordee)n ) );
}


// -----------------------------------------------------------------------------
// Name: ADN_Log_Data destructor
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
ADN_Log_Data::~ADN_Log_Data()
{
    vWorkTimeModifiers_.Reset();
}


// -----------------------------------------------------------------------------
// Name: ADN_Log_Data::FilesNeeded
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_Log_Data::FilesNeeded( T_StringList& vFiles ) const
{
    vFiles.push_back( ADN_Workspace::GetWorkspace().GetProject().GetData().GetDataInfos().szLog_.GetData() );
}


// -----------------------------------------------------------------------------
// Name: ADN_Log_Data::Reset
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_Log_Data::Reset()
{
}


// -----------------------------------------------------------------------------
// Name: ADN_Log_Data::ReadArchive
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_Log_Data::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section( "Logistique" );
    input.Section( "TempsDeBordee" );
    for( IT_WorkTimeModifiersInfo_Vector it = vWorkTimeModifiers_.begin(); it != vWorkTimeModifiers_.end(); ++it )
        (*it)->ReadArchive( input );
    input.EndSection(); // TempsDeBordee
    input.EndSection(); // Logistique
}


// -----------------------------------------------------------------------------
// Name: ADN_Log_Data::WriteArchive
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_Log_Data::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( "Logistique" );
    output.Section( "TempsDeBordee" );
    for( IT_WorkTimeModifiersInfo_Vector it = vWorkTimeModifiers_.begin(); it != vWorkTimeModifiers_.end(); ++it )
        (*it)->WriteArchive( output );
    output.EndSection(); // TempsDeBordee
    output.EndSection(); // Logistique
}
