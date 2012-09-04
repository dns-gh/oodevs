// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "ConsignResolver_ABC.h"
#include <boost/filesystem/operations.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "dispatcher/Model_ABC.h"
#include "dispatcher/Agent_ABC.h"
#include "dispatcher/Automat_ABC.h"
#include "dispatcher/Formation_ABC.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/BreakdownType.h"
#include "clients_kernel/ComponentType.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/StaticModel.h"
#include "clients_kernel/Tools.h"
#include "ENT/ENT_Tr_Gen.h"

using namespace plugins::logistic;
namespace bpt = boost::posix_time;
namespace bfs = boost::filesystem;
namespace bg = boost::gregorian;


namespace
{
    const int maxFileIndex = 1000;
    const int maxFileIndexToRemove = 20;
    const int maxFileDaysToRemove = 20;
    const int maxLines = 50000;
}

// -----------------------------------------------------------------------------
// Name: ConsignResolver_ABC constructor
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
ConsignResolver_ABC::ConsignResolver_ABC( const std::string& name, const dispatcher::Model_ABC& model, const kernel::StaticModel& staticModel )
    : name_( name ), curTick_( 0 ), model_( model ), staticModel_( staticModel ), curLineIndex_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ConsignResolver_ABC destructor
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
ConsignResolver_ABC::~ConsignResolver_ABC()
{
    for ( std::map< int, ConsignData_ABC* >::iterator it = consignsData_.begin(); it != consignsData_.end(); ++it )
        delete it->second;
    consignsData_.clear();
}

// -----------------------------------------------------------------------------
// Name: ConsignResolver_ABC::Receive
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
void ConsignResolver_ABC::Receive( const sword::SimToClient& message )
{
    if( !IsManageable( message ) )
        return;
    CheckOutputFile();
    if( output_.is_open() )
        ManageMessage( message );
}

// -----------------------------------------------------------------------------
// Name: ConsignResolver_ABC::GetConsign
// Created: MMC 2012-08-23
// -----------------------------------------------------------------------------
ConsignData_ABC& ConsignResolver_ABC::GetConsign( int requestId )
{
    std::map< int, ConsignData_ABC* >::iterator it = consignsData_.find( requestId );
    if( it == consignsData_.end() )
    {
        ConsignData_ABC* pConsign = CreateConsignData( requestId );
        consignsData_[ requestId ] = pConsign;
        return *pConsign;
    }
    return *it->second;
}

// -----------------------------------------------------------------------------
// Name: ConsignResolver_ABC::DestroyConsignData
// Created: MMC 2012-09-03
// -----------------------------------------------------------------------------
void ConsignResolver_ABC::DestroyConsignData( int requestId )
{
    std::map< int, ConsignData_ABC* >::iterator it = consignsData_.find( requestId );
    if( it != consignsData_.end() )
    {
        delete it->second;
        consignsData_.erase( it );
    }
}

// -----------------------------------------------------------------------------
// Name: ConsignResolver_ABC::AppendDateWithExtension
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
void ConsignResolver_ABC::AppendDateWithExtension( std::string& fileName, const bg::date& d, int fileIndex )
{
    fileName += "." + to_iso_string( d ) + "." + boost::lexical_cast< std::string >( fileIndex ) + ".csv";
}

// -----------------------------------------------------------------------------
// Name: ConsignResolver_ABC::SetNewFile
// Created: MMC 2012-08-27
// -----------------------------------------------------------------------------
void ConsignResolver_ABC::SetNewFile()
{
    bg::date today = bpt::second_clock::local_time().date();
    std::string newFileName;
    int fileIndex = 0;
    while( fileIndex < maxFileIndex )
    {
        newFileName = name_ ;
        AppendDateWithExtension( newFileName, today, fileIndex );
        try
        {
            if( !bfs::exists( newFileName ) )
                break;
        }
        catch( ... )
        {
            break;
        }
        ++fileIndex;
    }
    curLineIndex_ = 0;
    fileDate_ = today;
    fileName_ = newFileName;
}

// -----------------------------------------------------------------------------
// Name: ConsignResolver_ABC::RemoveOldFiles
// Created: MMC 2012-08-27
// -----------------------------------------------------------------------------
void ConsignResolver_ABC::RemoveOldFiles()
{
    bg::date today = bpt::second_clock::local_time().date();
    for( int before = 2; before < maxFileDaysToRemove; ++before )
        for( int index = 0; index < maxFileIndexToRemove; ++index )
        {
            std::string fileDayBeforeYesterday( name_ );
            AppendDateWithExtension( fileDayBeforeYesterday, today - bg::days( before ), index );
            try
            {
                if( bfs::exists( fileDayBeforeYesterday ) )
                    bfs::remove( fileDayBeforeYesterday );
            }
            catch( ... )
            {
                // NOTHING
            }
        }
}

// -----------------------------------------------------------------------------
// Name: ConsignResolver_ABC::OpenFile
// Created: MMC 2012-08-27
// -----------------------------------------------------------------------------
void ConsignResolver_ABC::OpenFile()
{
    if( output_.is_open() )
        output_.close();
    if( !fileName_.empty() )
    {
        try
        {
            if( bfs::exists( fileName_ ) )
            {
                if( !output_.is_open() )
                    output_.open( fileName_, std::ios_base::out | std::ios_base::app );
            }
            else
            {
                if( output_.is_open() )
                    output_.close();
                output_.open( fileName_, std::ios_base::out | std::ios_base::app );
                output_ << header_;
            }
        }
        catch( ... )
        {
            // NOTHING
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ConsignResolver_ABC::CheckOutputFile
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
void ConsignResolver_ABC::CheckOutputFile()
{
    bg::date today = bpt::second_clock::local_time().date();
    if( fileDate_ != today || curLineIndex_ > maxLines )
    {
        SetNewFile();
        OpenFile();
        RemoveOldFiles();
    }
}

// -----------------------------------------------------------------------------
// Name: ConsignResolver_ABC::SetTime
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
void ConsignResolver_ABC::SetTime( int tick, const std::string& simTime )
{
    curTick_ = tick;
    simTime_ = simTime;
}

// -----------------------------------------------------------------------------
// Name: ConsignResolver_ABC::GetSimTime
// Created: MMC 2012-08-21
// -----------------------------------------------------------------------------
void ConsignResolver_ABC::GetSimTime( std::string& simTime, std::string& tick ) const
{
    if( !simTime_.empty() )
        simTime = simTime_;
    if( curTick_ >= 0 )
        tick = boost::lexical_cast< std::string >( curTick_ );
}

// -----------------------------------------------------------------------------
// Name: ConsignResolver_ABC::GetAgentName
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
void ConsignResolver_ABC::GetAgentName( int id, std::string& name ) const
{
    const dispatcher::Agent_ABC* pAgent = model_.Agents().Find( static_cast< unsigned int >( id ) );
    if( pAgent && !pAgent->GetExtension( "NomLong" , name ) )
        name = pAgent->GetName().toAscii().constData();
}

// -----------------------------------------------------------------------------
// Name: ConsignResolver_ABC::GetAutomatName
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
void ConsignResolver_ABC::GetAutomatName( int id, std::string& name ) const
{
    const dispatcher::Automat_ABC* pAutomat = model_.Automats().Find( static_cast< unsigned int >( id ) );
    if( pAutomat && !pAutomat->GetExtension( "NomLong" , name ) )
        name = pAutomat->GetName().toAscii().constData();
}

// -----------------------------------------------------------------------------
// Name: ConsignResolver_ABC::GetFormationName
// Created: MMC 2012-08-06
// -----------------------------------------------------------------------------
void ConsignResolver_ABC::GetFormationName( int id, std::string& name ) const
{
    const dispatcher::Formation_ABC* pFormation = model_.Formations().Find( static_cast< unsigned int >( id ) );
    if( pFormation && !pFormation->GetExtension( "NomLong" , name ) )
        name = pFormation->GetName().toAscii().constData();
}

// -----------------------------------------------------------------------------
// Name: ConsignResolver_ABC::GetSupplykName
// Created: MMC 2012-08-21
// -----------------------------------------------------------------------------
void ConsignResolver_ABC::GetSupplykName( const sword::LogSupplyHandlingUpdate::EnumLogSupplyHandlingStatus& eSupply, std::string& name ) const
{
    name = ENT_Tr::ConvertFromLogSupplyHandlingStatus( static_cast< E_LogSupplyHandlingStatus >( eSupply ), ENT_Tr::eToTr );
}

// -----------------------------------------------------------------------------
// Name: ConsignResolver_ABC::GetRankName
// Created: MMC 2012-08-21
// -----------------------------------------------------------------------------
void ConsignResolver_ABC::GetRankName( const sword::EnumHumanRank& eRank, std::string& name ) const
{
    name = ENT_Tr::ConvertFromHumanRank( static_cast< E_HumanRank >( eRank ), ENT_Tr::eToTr );
}

// -----------------------------------------------------------------------------
// Name: ConsignResolver_ABC::GetWoundName
// Created: MMC 2012-08-21
// -----------------------------------------------------------------------------
void ConsignResolver_ABC::GetWoundName( const sword::EnumHumanWound& eWound, std::string& name ) const
{
    name = ENT_Tr::ConvertFromHumanWound( static_cast< E_HumanWound >( eWound ), ENT_Tr::eToTr );
}

// -----------------------------------------------------------------------------
// Name: ConsignResolver_ABC::GetMedicalName
// Created: MMC 2012-08-21
// -----------------------------------------------------------------------------
void ConsignResolver_ABC::GetMedicalName( const sword::LogMedicalHandlingUpdate::EnumLogMedicalHandlingStatus& eMedical, std::string& name ) const
{
    name = ENT_Tr::ConvertFromLogMedicalHandlingStatus( static_cast< E_LogMedicalHandlingStatus >( eMedical ), ENT_Tr::eToTr );
}

// -----------------------------------------------------------------------------
// Name: ConsignResolver_ABC::GetMaintenanceName
// Created: MMC 2012-08-21
// -----------------------------------------------------------------------------
void ConsignResolver_ABC::GetMaintenanceName( const sword::LogMaintenanceHandlingUpdate::EnumLogMaintenanceHandlingStatus& eMaintenance, std::string& name ) const
{
    name = ENT_Tr::ConvertFromLogMaintenanceHandlingStatus( static_cast< E_LogMaintenanceHandlingStatus >( eMaintenance ), ENT_Tr::eToTr );
}

// -----------------------------------------------------------------------------
// Name: ConsignResolver_ABC::GetFuneralName
// Created: MMC 2012-08-21
// -----------------------------------------------------------------------------
void ConsignResolver_ABC::GetFuneralName( const sword::LogFuneralHandlingUpdate::EnumLogFuneralHandlingStatus& eFuneral, std::string& name ) const
{
    name = ENT_Tr::ConvertFromLogFuneralHandlingStatus( static_cast< E_LogFuneralHandlingStatus >( eFuneral ), ENT_Tr::eToTr );
}

// -----------------------------------------------------------------------------
// Name: ConsignResolver_ABC::GetEquipmentName
// Created: MMC 2012-08-22
// -----------------------------------------------------------------------------
void ConsignResolver_ABC::GetEquipmentName( const sword::EquipmentType& equipmentType, std::string& name ) const
{
    kernel::ComponentType* pEquipment = staticModel_.types_.tools::Resolver< kernel::ComponentType >::Find( equipmentType.id() );
    if( pEquipment )
        name = pEquipment->GetName();
}

// -----------------------------------------------------------------------------
// Name: ConsignResolver_ABC::GetBreakdownName
// Created: MMC 2012-08-22
// -----------------------------------------------------------------------------
void ConsignResolver_ABC::GetBreakdownName( const sword::BreakdownType& breakdownType, std::string& name ) const
{
    kernel::BreakdownType* pBreakdown = staticModel_.objectTypes_.kernel::Resolver2< kernel::BreakdownType >::Find( breakdownType.id() );
    if( pBreakdown )
        name = pBreakdown->GetName();
}

// -----------------------------------------------------------------------------
// Name: ConsignResolver_ABC::GetResourceName
// Created: MMC 2012-08-23
// -----------------------------------------------------------------------------
void ConsignResolver_ABC::GetResourceName( const sword::ResourceType& resourceType, std::string& name ) const
{
    kernel::DotationType* pDotation = staticModel_.objectTypes_.kernel::Resolver2< kernel::DotationType >::Find( resourceType.id() );
    if( pDotation )
        name = pDotation->GetName();
}

// -----------------------------------------------------------------------------
// Name: ConsignResolver_ABC::SetHeader
// Created: MMC 2012-08-23
// -----------------------------------------------------------------------------
void ConsignResolver_ABC::SetHeader( const ConsignData_ABC& consign )
{
    std::stringstream header;
    consign >> header;
    header_ = header.str();
}
