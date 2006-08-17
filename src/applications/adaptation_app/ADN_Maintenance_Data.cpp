// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Maintenance_Data.h"

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
// Name: WorkingSchemeInfo::WorkingSchemeInfo
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
ADN_Maintenance_Data::WorkingSchemeInfo::WorkingSchemeInfo( unsigned int nIdx )
: ADN_Ref_ABC         ()
, ADN_DataTreeNode_ABC()
, nIdx_               ( nIdx )
, nWorkTime_          ( 0 )
, warningDelay_       ()
{
}


// -----------------------------------------------------------------------------
// Name: WorkingSchemeInfo::GetNodeName
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
std::string ADN_Maintenance_Data::WorkingSchemeInfo::GetNodeName()
{
    return std::string();
}


// -----------------------------------------------------------------------------
// Name: WorkingSchemeInfo::GetItemName
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
std::string ADN_Maintenance_Data::WorkingSchemeInfo::GetItemName()
{
    return std::string();
}


// -----------------------------------------------------------------------------
// Name: WorkingSchemeInfo::ReadArchive
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_Maintenance_Data::WorkingSchemeInfo::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section( QString( "Regime" ).append( QString::number( nIdx_.GetData() ) ).ascii() );
    input.ReadAttribute( "dureeTravail", nWorkTime_ );
    input.ReadAttribute( "delaiAvantAvertissement", warningDelay_, ADN_XmlInput_Helper::eNothing );
    input.EndSection();
}


// -----------------------------------------------------------------------------
// Name: WorkingSchemeInfo::WriteArchive
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_Maintenance_Data::WorkingSchemeInfo::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( QString( "Regime" ).append( QString::number( nIdx_.GetData() ) ).ascii() );
    output.WriteAttribute( "dureeTravail", nWorkTime_.GetData() );
    if(  ! warningDelay_.GetData().empty()
        && warningDelay_.GetData() != "0s"
        && warningDelay_.GetData() != "0m"
        && warningDelay_.GetData() != "0h" )
        output.WriteAttribute( "delaiAvantAvertissement", warningDelay_.GetData() );
    output.EndSection();
}

// -----------------------------------------------------------------------------
// Name: ADN_Maintenance_Data constructor
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
ADN_Maintenance_Data::ADN_Maintenance_Data()
: ADN_Data_ABC()
{
    for( int n = 0; n < 4; ++n )
        vWorkingSchemes_.AddItem( new WorkingSchemeInfo( n+1 ) );
}


// -----------------------------------------------------------------------------
// Name: ADN_Maintenance_Data destructor
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
ADN_Maintenance_Data::~ADN_Maintenance_Data()
{
    vWorkingSchemes_.Reset();
    vRepairerWarnings_.Reset();
    vHaulerWarnings_.Reset();
}


// -----------------------------------------------------------------------------
// Name: ADN_Maintenance_Data::FilesNeeded
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_Maintenance_Data::FilesNeeded( T_StringList& vFiles ) const
{
    vFiles.push_back( ADN_Workspace::GetWorkspace().GetProject().GetData().GetDataInfos().szMaintenance_.GetData() );
}


// -----------------------------------------------------------------------------
// Name: ADN_Maintenance_Data::Reset
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_Maintenance_Data::Reset()
{
    vRepairerWarnings_.Reset();
    vHaulerWarnings_.Reset();
}


// -----------------------------------------------------------------------------
// Name: ADN_Maintenance_Data::ReadArchive
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_Maintenance_Data::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section( "Maintenance" );
    input.Section( "RegimesTravail" );
    for( IT_WorkingSchemeInfo_Vector it = vWorkingSchemes_.begin(); it != vWorkingSchemes_.end(); ++it )
        (*it)->ReadArchive( input );
    input.EndSection();

    input.Section( "AlertesDisponibiliteMoyens" );
        input.BeginList( "AlertesDisponibiliteReparateurs" );
        while( input.NextListElement() )
        {
            std::auto_ptr< ADN_AvailabilityWarning > pNew( new ADN_AvailabilityWarning() );
            pNew->ReadArchive( input, "AlerteDisponibiliteReparateurs" );
            vRepairerWarnings_.AddItem( pNew.release() );
        }
        vRepairerWarnings_.AddItem( 0 );
        input.EndList();

        input.BeginList( "AlertesDisponibiliteRemorqueurs" );
        while( input.NextListElement() )
        {
            std::auto_ptr< ADN_AvailabilityWarning > pNew( new ADN_AvailabilityWarning() );
            pNew->ReadArchive( input, "AlerteDisponibiliteRemorqueurs" );
            vHaulerWarnings_.AddItem( pNew.release() );
        }
        vHaulerWarnings_.AddItem( 0 );
        input.EndList();
    input.EndSection();

    input.EndSection();
}


// -----------------------------------------------------------------------------
// Name: ADN_Maintenance_Data::WriteArchive
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_Maintenance_Data::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section( "Maintenance" );
    output.Section( "RegimesTravail" );
    for( IT_WorkingSchemeInfo_Vector it = vWorkingSchemes_.begin(); it != vWorkingSchemes_.end(); ++it )
        (*it)->WriteArchive( output );
    output.EndSection();

    output.Section( "AlertesDisponibiliteMoyens" );
        output.BeginList( "AlertesDisponibiliteReparateurs", vRepairerWarnings_.size() );
        for( IT_AvailabilityWarning_Vector it = vRepairerWarnings_.begin(); it != vRepairerWarnings_.end(); ++it )
            (*it)->WriteArchive( output, "AlerteDisponibiliteReparateurs" );
        output.EndList();

        output.BeginList( "AlertesDisponibiliteRemorqueurs", vHaulerWarnings_.size() );
        for( IT_AvailabilityWarning_Vector it = vHaulerWarnings_.begin(); it != vHaulerWarnings_.end(); ++it )
            (*it)->WriteArchive( output, "AlerteDisponibiliteRemorqueurs" );
        output.EndList();
    output.EndSection();

    output.EndSection();
}
