// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
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
, bWorkingTimeSet_    ( false )
, workingTime_        ()
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
    if( input.ReadField( "DelaiAvantAvertissement", workingTime_, ADN_XmlInput_Helper::eNothing ) )
        bWorkingTimeSet_ = true;
    input.EndSection();
}


// -----------------------------------------------------------------------------
// Name: WorkTimeModifiersInfo::WriteArchive
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_Log_Data::WorkTimeModifiersInfo::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( ADN_Tools::ComputeWorkingTimeScriptName( nType_ ) );
    if( bWorkingTimeSet_ == true )
        output.WriteField( "DelaiAvantAvertissement", workingTime_.GetData() );
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
