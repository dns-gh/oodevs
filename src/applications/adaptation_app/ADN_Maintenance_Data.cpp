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
#include "ADN_Tools.h"
#include "ADN_Enums.h"
#include "ENT/ENT_Tr.h"

// -----------------------------------------------------------------------------
// Name: WorkingSchemeInfo::WorkingSchemeInfo
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
ADN_Maintenance_Data::WorkingSchemeInfo::WorkingSchemeInfo( unsigned int nIdx )
    : nIdx_               ( nIdx )
    , nWorkTime_          ( 0 )
    , warningDelay_       ( "0s" )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: WorkingSchemeInfo::ReadArchive
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_Maintenance_Data::WorkingSchemeInfo::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "working-time", nWorkTime_ )
          >> xml::optional >> xml::attribute( "time-before-warning", warningDelay_ );
}

// -----------------------------------------------------------------------------
// Name: WorkingSchemeInfo::WriteArchive
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_Maintenance_Data::WorkingSchemeInfo::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "working-scheme" )
        << xml::attribute( "type", QString( "Regime" ).append( QString::number( nIdx_.GetData() ) ).toStdString() )
        << xml::attribute( "working-time", nWorkTime_ );
    if( !ADN_Tools::IsNullDelay( warningDelay_.GetData() ) )
        output << xml::attribute( "time-before-warning", warningDelay_ );
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Maintenance_Data constructor
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
ADN_Maintenance_Data::ADN_Maintenance_Data()
    : ADN_Data_ABC( eLogistic, eMaintenance )
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
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Maintenance_Data::FilesNeeded
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_Maintenance_Data::FilesNeeded( tools::Path::T_Paths& vFiles ) const
{
    vFiles.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szMaintenance_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Maintenance_Data::ReadWorkingScheme
// Created: AGE 2007-08-17
// -----------------------------------------------------------------------------
void ADN_Maintenance_Data::ReadWorkingScheme( xml::xistream& input )
{
    std::string type;
    input >> xml::attribute( "type", type );
    const unsigned nType = *type.rbegin() - '1';
    vWorkingSchemes_.at( nType )->ReadArchive( input );
}

// -----------------------------------------------------------------------------
// Name: ADN_Maintenance_Data::ReadAvailabilityAlert
// Created: AGE 2007-08-17
// -----------------------------------------------------------------------------
void ADN_Maintenance_Data::ReadAvailabilityAlert( xml::xistream& input )
{
    std::auto_ptr< ADN_AvailabilityWarning > pNew( new ADN_AvailabilityWarning() );
    pNew->ReadArchive( input );
    std::string resource;
    input >> xml::attribute( "resource", resource );
    if( resource == "AlerteDisponibiliteReparateurs" )
        vRepairerWarnings_.AddItem( pNew.release() );
    else if( resource == "AlerteDisponibiliteRemorqueurs" )
        vHaulerWarnings_.AddItem( pNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Maintenance_Data::ReadArchive
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_Maintenance_Data::ReadArchive( xml::xistream& input )
{
    input >> xml::start( "maintenance" )
            >> xml::start( "working-schemes" )
                >> xml::list( "working-scheme", *this, &ADN_Maintenance_Data::ReadWorkingScheme )
            >> xml::end
            >> xml::start( "resource-availability-alerts" )
                >> xml::list( "resource-availability-alert", *this, &ADN_Maintenance_Data::ReadAvailabilityAlert )
            >> xml::end
          >> xml::end;
    vRepairerWarnings_.AddItem( 0 );
    vHaulerWarnings_.AddItem( 0 );
}

// -----------------------------------------------------------------------------
// Name: ADN_Maintenance_Data::WriteArchive
// Created: APE 2005-03-14
// -----------------------------------------------------------------------------
void ADN_Maintenance_Data::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "maintenance" );
    tools::SchemaWriter schemaWriter;
    schemaWriter.WritePhysicalSchema( output, "Maintenance" );
    output << xml::start( "working-schemes" );
    for( auto it = vWorkingSchemes_.begin(); it != vWorkingSchemes_.end(); ++it )
        (*it)->WriteArchive( output );
    output << xml::end
        << xml::start( "resource-availability-alerts" );
    for( auto it = vRepairerWarnings_.begin(); it != vRepairerWarnings_.end(); ++it )
        (*it)->WriteArchive( output, "resource", "AlerteDisponibiliteReparateurs" );
    for( auto it = vHaulerWarnings_.begin(); it != vHaulerWarnings_.end(); ++it )
        (*it)->WriteArchive( output, "resource", "AlerteDisponibiliteRemorqueurs" );
    output << xml::end
        << xml::end;
}
