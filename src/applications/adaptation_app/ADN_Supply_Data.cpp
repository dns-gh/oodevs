// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Supply_Data.h"

#include "ADN_ConsistencyChecker.h"
#include "ADN_Missions_Data.h"
#include "ADN_Workspace.h"
#include "ADN_Supply_Gui.h"
#include "ADN_Logistic_GUI.h"
#include "ADN_Project_Data.h"
#include "ADN_Tools.h"
#include "ADN_Tr.h"
#include "ADN_WorkspaceElement.h"

#include "ENT/ENT_Tr.h"

// -----------------------------------------------------------------------------
// Name: ConvoyInfo::ConvoyInfo
// Created: APE 2005-03-22
// -----------------------------------------------------------------------------
template< typename T >
ADN_Supply_Data::ConvoyInfo< T >::ConvoyInfo( typename T::BaseType value )
    : nNbrTrucks_( 1 )
    , value_( value )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ConvoyInfo::ReadArchive
// Created: APE 2005-03-22
// -----------------------------------------------------------------------------
template< typename T >
void ADN_Supply_Data::ConvoyInfo< T >::ReadArchive( const std::string& value, xml::xistream& input )
{
    input >> xml::attribute( "truck-count", nNbrTrucks_ )
          >> xml::attribute( value, value_ );
}

// -----------------------------------------------------------------------------
// Name: ConvoyInfo::WriteArchive
// Created: APE 2005-03-22
// -----------------------------------------------------------------------------
template< typename T >
void ADN_Supply_Data::ConvoyInfo< T >::WriteArchive( const std::string& section, const std::string& attribute, xml::xostream& output )
{
    output << xml::start( section )
            << xml::attribute( "truck-count", nNbrTrucks_ )
            << xml::attribute( attribute, value_ )
           << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Supply_Data::SupplyDataInfos::SupplyDataInfos
// Created: SBO 2009-06-03
// -----------------------------------------------------------------------------
ADN_Supply_Data::SupplyDataInfos::SupplyDataInfos()
    : ptrUnit_( ADN_Workspace::GetWorkspace().GetUnits().GetData().GetUnitsInfos(), 0 )
    , ptrSupplyMission_( ADN_Workspace::GetWorkspace().GetMissions().GetData().GetMissions( eMissionType_Pawn ), 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Supply_Data::SupplyDataInfos::~SupplyDataInfos
// Created: SBO 2009-06-03
// -----------------------------------------------------------------------------
ADN_Supply_Data::SupplyDataInfos::~SupplyDataInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Supply_Data::SupplyDataInfos::ReadArchive
// Created: SBO 2009-06-03
// -----------------------------------------------------------------------------
void ADN_Supply_Data::SupplyDataInfos::ReadArchive( xml::xistream& input )
{
    std::string strUnit, supplyMission;
    std::string dotationSupplyConvoyType( "virtual" );
    std::string stockSupplyConvoyType( "real" );
    input >> xml::start( "supply" )
            >> xml::optional >> xml::start( "convoys" )
              >> xml::attribute( "unit-type", strUnit )
              >> xml::attribute( "mission", supplyMission )
              >> xml::optional >> xml::start( "type" )
                >> xml::start( "dotation-supply" )
                    >> xml::attribute( "type", dotationSupplyConvoyType )
                >> xml::end
                >> xml::start( "stock-supply" )
                    >> xml::attribute( "type", stockSupplyConvoyType )
                >> xml::end
              >> xml::end
              >> xml::start( "constitution-times" )
                >> xml::list( "unit-time", *this, &ADN_Supply_Data::SupplyDataInfos::ReadConstitutionTime )
              >> xml::end
              >> xml::start( "loading-times" )
                >> xml::list( "unit-time", *this, &ADN_Supply_Data::SupplyDataInfos::ReadLoadingTime )
              >> xml::end
              >> xml::start( "unloading-times" )
                >> xml::list( "unit-time", *this, &ADN_Supply_Data::SupplyDataInfos::ReadUnloadingTime )
              >> xml::end
              >> xml::start( "speed-modifiers" )
                >> xml::list( "speed-modifier", *this, &ADN_Supply_Data::SupplyDataInfos::ReadSpeedModifier )
              >> xml::end
            >> xml::end;
    vConvoySetupInfos_.AddItem( 0 );
    vConvoyLoadingInfos_.AddItem( 0 );
    vConvoyUnloadingInfos_.AddItem( 0 );
    vConvoySpeedModificatorInfos_.AddItem( 0 );

    dotationSupplyConvoyType_ = ADN_Tr::ConvertToSupplyConvoyType( dotationSupplyConvoyType );
    if( dotationSupplyConvoyType_ == E_SupplyConvoyType( -1 ) )
        throw MASA_EXCEPTION( tools::translate( "Breakdown_Data", "Logistic supply system - Invalid supply convoy type '%1'" ).arg( dotationSupplyConvoyType.c_str() ).toStdString() );
    stockSupplyConvoyType_ = ADN_Tr::ConvertToSupplyConvoyType( stockSupplyConvoyType );
    if( stockSupplyConvoyType_ == E_SupplyConvoyType( -1 ) )
        throw MASA_EXCEPTION( tools::translate( "Breakdown_Data", "Logistic supply system - Invalid supply convoy type '%1'" ).arg( stockSupplyConvoyType.c_str() ).toStdString() );

    ADN_Units_Data::UnitInfos* pUnit = ADN_Workspace::GetWorkspace().GetUnits().GetData().FindUnit( strUnit );
    if( !strUnit.empty() && pUnit == 0 )
        throw MASA_EXCEPTION( tools::translate( "Supply_Data",  "Logistic supply system - Invalid unit '%1'" ).arg( strUnit.c_str() ).toStdString() );
    ptrUnit_ = pUnit;

    ADN_Missions_Data::T_Mission_Vector& missions = ADN_Workspace::GetWorkspace().GetMissions().GetData().GetMissions( eMissionType_Pawn );
    ADN_Missions_ABC* mission = ADN_Workspace::GetWorkspace().GetMissions().GetData().FindMission( missions, supplyMission );
    if( !supplyMission.empty() && mission == 0 )
        throw MASA_EXCEPTION( tools::translate( "Supply_Data",  "Logistic supply system - Invalid mission '%1'" ).arg( supplyMission.c_str() ).toStdString() );
    ptrSupplyMission_ = mission;

    input >> xml::start( "resource-availability-alerts" )
            >> xml::list( "resource-availability-alert", *this, &ADN_Supply_Data::SupplyDataInfos::ReadResourceAvailability )
          >> xml::end
        >> xml::end;

    vVectorWarnings_.AddItem( 0 );
}

// -----------------------------------------------------------------------------
// Name: ADN_Supply_Data::SupplyDataInfos::ReadResourceAvailability
// Created: AGE 2007-08-16
// -----------------------------------------------------------------------------
void ADN_Supply_Data::SupplyDataInfos::ReadResourceAvailability( xml::xistream& input )
{
    std::auto_ptr< ADN_AvailabilityWarning > pNew( new ADN_AvailabilityWarning() );
    pNew->ReadArchive( input );
    vVectorWarnings_.AddItem( pNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Supply_Data::SupplyDataInfos::ReadConstitutionTime
// Created: AGE 2007-08-16
// -----------------------------------------------------------------------------
void ADN_Supply_Data::SupplyDataInfos::ReadConstitutionTime( xml::xistream& input )
{
    std::auto_ptr< ConvoyInfo< ADN_Type_Time > > spNew( new ConvoyInfo< ADN_Type_Time >( "1s" ) );
    spNew->ReadArchive( "time", input );
    vConvoySetupInfos_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Supply_Data::SupplyDataInfos::ReadLoadingTime
// Created: AGE 2007-08-16
// -----------------------------------------------------------------------------
void ADN_Supply_Data::SupplyDataInfos::ReadLoadingTime( xml::xistream& input )
{
    std::auto_ptr< ConvoyInfo< ADN_Type_Time > > spNew( new ConvoyInfo< ADN_Type_Time >( "1s" ) );
    spNew->ReadArchive( "time", input );
    vConvoyLoadingInfos_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Supply_Data::SupplyDataInfos::ReadUnloadingTime
// Created: AGE 2007-08-16
// -----------------------------------------------------------------------------
void ADN_Supply_Data::SupplyDataInfos::ReadUnloadingTime( xml::xistream& input )
{
    std::auto_ptr< ConvoyInfo< ADN_Type_Time > > spNew( new ConvoyInfo< ADN_Type_Time >( "1s" ) );
    spNew->ReadArchive( "time", input );
    vConvoyUnloadingInfos_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Supply_Data::SupplyDataInfos::ReadSpeedModifier
// Created: AGE 2007-08-16
// -----------------------------------------------------------------------------
void ADN_Supply_Data::SupplyDataInfos::ReadSpeedModifier( xml::xistream& input )
{
    std::auto_ptr< ConvoyInfo< ADN_Type_Double > > spNew( new ConvoyInfo< ADN_Type_Double >( 1 ) );
    spNew->ReadArchive( "value", input );
    vConvoySpeedModificatorInfos_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Supply_Data::SupplyDataInfos::CheckDatabaseValidity
// Created: PHC 2011-01-19
// -----------------------------------------------------------------------------
void ADN_Supply_Data::SupplyDataInfos::CheckDatabaseValidity( ADN_ConsistencyChecker& checker ) const
{
    if( ptrUnit_.GetData() == 0 || ptrUnit_.GetData()->eTypeId_.GetData() != eAgentTypePionLOGConvoi )
        checker.AddError( eMissingConvoy, "", eLogistic, 1 );
    if( ptrSupplyMission_.GetData() == 0 )
        checker.AddError( eMissingConvoyMission, "", eLogistic, 1 );
}

// -----------------------------------------------------------------------------
// Name: ADN_Supply_Data::SupplyDataInfos::WriteArchive
// Created: APE 2005-03-22
// -----------------------------------------------------------------------------
void ADN_Supply_Data::SupplyDataInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "supply" );
    tools::SchemaWriter schemaWriter;
    schemaWriter.WritePhysicalSchema( output, "Supply" );
    if( ptrUnit_.GetData() != 0 && ptrUnit_.GetData()->eTypeId_.GetData() == eAgentTypePionLOGConvoi )
    {
        output  << xml::start( "convoys" )
                    << xml::attribute( "unit-type", ptrUnit_ )
                    << xml::attribute( "mission", ptrSupplyMission_ );
        {
            output << xml::start( "type" )
                        << xml::start( "dotation-supply" )
                            << xml::attribute( "type", dotationSupplyConvoyType_.Convert() )
                        << xml::end
                        << xml::start( "stock-supply" )
                            << xml::attribute( "type", stockSupplyConvoyType_.Convert() )
                        << xml::end
                   << xml::end;
        }
        {
            output << xml::start( "constitution-times" );
            for( auto it = vConvoySetupInfos_.begin(); it != vConvoySetupInfos_.end(); ++it )
                (*it)->WriteArchive( "unit-time", "time", output );
            output << xml::end;
        }
        {
            output << xml::start( "loading-times" );
            for( auto it = vConvoyLoadingInfos_.begin(); it != vConvoyLoadingInfos_.end(); ++it )
                (*it)->WriteArchive( "unit-time", "time", output );
            output << xml::end;
        }
        {
            output << xml::start( "unloading-times" );
            for( auto it = vConvoyUnloadingInfos_.begin(); it != vConvoyUnloadingInfos_.end(); ++it )
                (*it)->WriteArchive( "unit-time", "time", output );
            output << xml::end;
        }
        {
            output << xml::start( "speed-modifiers" );
            for( auto it = vConvoySpeedModificatorInfos_.begin(); it != vConvoySpeedModificatorInfos_.end(); ++it )
                (*it)->WriteArchive( "speed-modifier", "value", output );
            output << xml::end;
        }
        output << xml::end;
    }

    output << xml::start( "resource-availability-alerts" );
    if( vVectorWarnings_.empty() )
    {
        std::auto_ptr< ADN_AvailabilityWarning > pNew( new ADN_AvailabilityWarning() );
        pNew->WriteArchive( output );
    }
    else
        for( auto it = vVectorWarnings_.begin(); it != vVectorWarnings_.end(); ++it )
            (*it)->WriteArchive( output );
    output << xml::end;

    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Supply_Data constructor
// Created: APE 2005-03-22
// -----------------------------------------------------------------------------
ADN_Supply_Data::ADN_Supply_Data()
    : ADN_Data_ABC( eLogistic, eSupply )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Supply_Data destructor
// Created: APE 2005-03-22
// -----------------------------------------------------------------------------
ADN_Supply_Data::~ADN_Supply_Data()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Supply_Data::FilesNeeded
// Created: APE 2005-03-22
// -----------------------------------------------------------------------------
void ADN_Supply_Data::FilesNeeded( tools::Path::T_Paths& vFiles ) const
{
    vFiles.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szSupply_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Supply_Data::ReadArchive
// Created: APE 2005-03-22
// -----------------------------------------------------------------------------
void ADN_Supply_Data::ReadArchive( xml::xistream& input )
{
    infos_.ReadArchive( input );
}

// -----------------------------------------------------------------------------
// Name: ADN_Supply_Data::WriteArchive
// Created: SBO 2009-06-03
// -----------------------------------------------------------------------------
void ADN_Supply_Data::WriteArchive( xml::xostream& output ) const
{
    infos_.WriteArchive( output );
}

// -----------------------------------------------------------------------------
// Name: ADN_Supply_Data::CheckDatabaseValidity
// Created: PHC 2011-01-19
// -----------------------------------------------------------------------------
void ADN_Supply_Data::CheckDatabaseValidity( ADN_ConsistencyChecker& checker ) const
{
    ADN_Data_ABC::CheckDatabaseValidity( checker );
    infos_.CheckDatabaseValidity( checker );
}
