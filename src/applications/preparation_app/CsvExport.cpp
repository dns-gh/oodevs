// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "CsvExport.h"
#include "Progress_ABC.h"
#include "preparation/Model.h"
#include "preparation/TeamsModel.h"
#include "preparation/ObjectsModel.h"
#include "preparation/AgentsModel.h"
#include "preparation/WeatherModel.h"
#include "preparation/InitialState.h"
#include "preparation/InitialStateResource.h"
#include "preparation/Stocks.h"
#include "preparation/Dotation.h"
#include "preparation/ProfilesModel.h"
#include "preparation/LogisticHierarchiesBase.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/PopulationType.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Inhabitant_ABC.h"
#include "clients_kernel/InhabitantType.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/Diplomacies_ABC.h"
#include "clients_kernel/Karma.h"
#include "clients_kernel/EntityType.h"
#include "clients_kernel/LogisticLevel.h"
#include "clients_gui/LongNameHelper.h"
#include "meteo/Meteo.h"
#include "meteo/MeteoLocal.h"
#include "meteo/PHY_Precipitation.h"
#include <boost/foreach.hpp>
#pragma warning( push, 0 )
#include <boost/algorithm/string.hpp>
#pragma warning( pop )
#include <boost/lexical_cast.hpp>

namespace bfs = boost::filesystem;

// -----------------------------------------------------------------------------
// Name: CsvExport constructor
// Created: LGY 2011-10-18
// -----------------------------------------------------------------------------
CsvExport::CsvExport( Model& model, const kernel::CoordinateConverter_ABC& converter )
    : model_    ( model )
    , converter_( converter )
{
        // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CsvExport destructor
// Created: LGY 2011-10-18
// -----------------------------------------------------------------------------
CsvExport::~CsvExport()
{
        // NOTHING
}

namespace
{
    std::string GetSeparator()
    {
        char buffer[256];
        GetLocaleInfo( LOCALE_SYSTEM_DEFAULT, LOCALE_SLIST, buffer, 256 );
        return buffer;
    }
}

// -----------------------------------------------------------------------------
// Name: CsvExport::Execute
// Created: LGY 2011-10-18
// -----------------------------------------------------------------------------
void CsvExport::Execute( bfs::path& path, Progress_ABC& progress )
{
    const std::string separator = GetSeparator();
    progress.Update( 0 );
    WriteEntity( path, separator );
    progress.Update( 10 );
    WriteResources( path, separator );
    progress.Update( 20 );
    WriteStocks( path, separator );
    progress.Update( 30 );
    WriteWeather( path, separator );
    progress.Update( 40 );
    WriteDiplomaty( path, separator );
    progress.Update( 50 );
    WriteProfiles( path, separator );
    progress.Update( 90 );
    WriteLogistic( path, separator );
    progress.Update( 100 );
}
namespace
{
    std::string  GetType( const QString& type )
    {
        if( type == kernel::Automat_ABC::typeName_.c_str() )
            return tools::translate( "CsvExport", "Automat" ).toAscii().constData();
        return tools::translate( "CsvExport", "Entity" ).toAscii().constData();
    }
    std::string GetType( const kernel::Entity_ABC& entity )
    {
        if( const kernel::Agent_ABC* pAgent = dynamic_cast< const kernel::Agent_ABC* >( &entity ) )
            return pAgent->GetType().GetName();
        else if( const kernel::EntityType< kernel::InhabitantType >* type = entity.Retrieve< kernel::EntityType< kernel::InhabitantType > >() )
            return type->GetType().GetName();
        else if( const kernel::EntityType< kernel::PopulationType >* type = entity.Retrieve< kernel::EntityType< kernel::PopulationType > >() )
            return type->GetType().GetName();
        else if( const kernel::EntityType< kernel::AutomatType >* type = entity.Retrieve< kernel::EntityType< kernel::AutomatType > >() )
            return type->GetType().GetName();
        return "";
    }

    std::string GetPosition( const kernel::Entity_ABC& entity, const kernel::CoordinateConverter_ABC& converter, const std::string& separator )
    {
        if( const kernel::Agent_ABC* pAgent = dynamic_cast< const kernel::Agent_ABC* >( &entity ) )
            return separator + converter.ConvertToMgrs( pAgent->Get< kernel::Positions >().GetPosition() );
        else if( const kernel::Object_ABC* pObject = dynamic_cast< const kernel::Object_ABC* >( &entity ) )
            return separator + converter.ConvertToMgrs( pObject->Get< kernel::Positions >().GetPosition() );
        else if( const kernel::Population_ABC* pPopulation = dynamic_cast< const kernel::Population_ABC* >( &entity ) )
            return separator + converter.ConvertToMgrs( pPopulation->Get< kernel::Positions >().GetPosition() );
        return separator;
    }
    std::string GetName( const kernel::Entity_ABC& entity )
    {
        std::string longName = gui::longname::GetEntityLongName( entity );
        return longName.empty() ? entity.GetName().toAscii().constData() : longName;
    }
    std::string GetSide( const kernel::Entity_ABC& entity )
    {
        return entity.Get< kernel::TacticalHierarchies >().GetTop().GetName().toAscii().constData();
    }
}

// -----------------------------------------------------------------------------
// Name: CsvExport::WriteEntity
// Created: LGY 2011-10-18
// -----------------------------------------------------------------------------
void CsvExport::WriteEntity( bfs::path& path, const std::string& separator )
{
    bfs::path odbPath( bfs::path( path / bfs::path( std::string( tools::translate( "CsvExport", "orbat" ).toAscii().constData() ) + ".csv" ).filename() ) );
    std::ofstream file( odbPath.string().c_str() );
    file    << tools::translate( "CsvExport", "SIDE" ) << separator << tools::translate( "CsvExport", "CATEGORY" )
            << separator << tools::translate( "CsvExport", "NAME" ) << separator << tools::translate( "CsvExport", "TYPE" )
            << separator << tools::translate( "CsvExport", "POSITION" ) << std::endl;
    tools::Iterator< const kernel::Team_ABC& > itTeam = model_.teams_.CreateIterator();
    while( itTeam.HasMoreElements() )
    {
        const kernel::Team_ABC& team = itTeam.NextElement();
        WriteEntity( team, team.GetName().toAscii().constData(), file, separator );
    }
    tools::Iterator< const kernel::Object_ABC& > itObject = model_.objects_.CreateIterator();
    while( itObject.HasMoreElements() )
    {
        const kernel::Object_ABC& object = itObject.NextElement();
        Write( file, separator, GetSide( object ), tools::translate( "CsvExport", "Object" ).toAscii().constData(),
               object.GetName().toAscii().constData(), object.GetType().GetName(), GetPosition( object, converter_, separator ) );
    }
    tools::Iterator< const kernel::Population_ABC& > itPopulation = model_.GetPopulationResolver().CreateIterator();
    while( itPopulation.HasMoreElements() )
    {
        const kernel::Population_ABC& population = itPopulation.NextElement();
        Write( file, separator, GetSide( population ), tools::translate( "CsvExport", "Population" ).toAscii().constData(),
               population.GetName().toAscii().constData(), GetType( population ), GetPosition( population, converter_, separator ) );
    }
    tools::Iterator< const kernel::Inhabitant_ABC& > itInhabitant = model_.GetInhabitantResolver().CreateIterator();
    while( itInhabitant.HasMoreElements() )
    {
        const kernel::Inhabitant_ABC& inhabitant = itInhabitant.NextElement();
        Write( file, separator, GetSide( inhabitant ), tools::translate( "CsvExport", "Inhabitant" ).toAscii().constData(),
               inhabitant.GetName().toAscii().constData(), GetType( inhabitant ), GetPosition( inhabitant, converter_, separator ) );
    }
    file.close();
}

// -----------------------------------------------------------------------------
// Name: CsvExport::WriteEntity
// Created: LGY 2011-10-18
// -----------------------------------------------------------------------------
void CsvExport::WriteEntity( const kernel::Entity_ABC& entity, const std::string& side, std::ofstream& file, const std::string& separator )
{
    tools::Iterator< const kernel::Entity_ABC& > it = entity.Get< kernel::TacticalHierarchies >().CreateSubordinateIterator();
    while( it.HasMoreElements() )
    {
        const kernel::Entity_ABC& child = it.NextElement();
        if( child.GetTypeName() != kernel::Formation_ABC::typeName_ && child.GetTypeName() != kernel::Team_ABC::typeName_ )
            Write( file, separator, side, GetType( child.GetTypeName().c_str() ), GetName( child ),
                   GetType( child ), GetPosition( child, converter_, separator ) );
        WriteEntity( child, side, file, separator );
    }
}

// -----------------------------------------------------------------------------
// Name: CsvExport::Write
// Created: LGY 2011-10-18
// -----------------------------------------------------------------------------
void CsvExport::Write( std::ofstream& file, const std::string& separator, const std::string& side,
                const std::string& category, const std::string& name, const std::string& type, const std::string& position )
{
    file << side << separator << category << separator << name << separator << type << position << std::endl;
}

// -----------------------------------------------------------------------------
// Name: CsvExport::WriteResources
// Created: LGY 2011-10-18
// -----------------------------------------------------------------------------
void CsvExport::WriteResources( boost::filesystem::path& path, const std::string& separator )
{
    bfs::path resourcesPath( bfs::path( path / bfs::path( std::string( tools::translate( "CsvExport", "resources" ).toAscii().constData() ) + ".csv" ).filename() ) );
    std::ofstream file( resourcesPath.string().c_str() );
    file << tools::translate( "CsvExport", "ENTITY" ) << separator << tools::translate( "CsvExport", "TYPE" ) << separator
         << tools::translate( "CsvExport", "QUANTITY" ) << std::endl;
    tools::Iterator< const kernel::Agent_ABC& > it = model_.GetAgentResolver().CreateIterator();
    while( it.HasMoreElements() )
    {
        const kernel::Agent_ABC& child = it.NextElement();
        const InitialState& extension = child.Get< InitialState >();
        for( InitialState::CIT_Resources it = extension.resources_.begin(); it != extension.resources_.end(); ++it )
            file << GetName( child ) << separator << it->name_ << separator << it->number_ << std::endl;
    }
    file.close();
}

// -----------------------------------------------------------------------------
// Name: CsvExport::WriteStocks
// Created: LGY 2011-10-18
// -----------------------------------------------------------------------------
void CsvExport::WriteStocks( boost::filesystem::path& path, const std::string& separator )
{
    bfs::path stocksPath( bfs::path( path / bfs::path( std::string( tools::translate( "CsvExport", "stocks" ).toAscii().constData() ) + ".csv" ).filename() ) );
    std::ofstream file( stocksPath.string().c_str() );
    file << tools::translate( "CsvExport", "ENTITY" ) << separator << tools::translate( "CsvExport", "TYPE" ) << separator
         << tools::translate( "CsvExport", "QUANTITY" ) << std::endl;
    tools::Iterator< const kernel::Agent_ABC& > it = model_.GetAgentResolver().CreateIterator();
    while( it.HasMoreElements() )
    {
        const kernel::Agent_ABC& child = it.NextElement();
        const Stocks* stocks = child.Retrieve< Stocks >();
        if( stocks && stocks->HasDotations() )
        {
            tools::Iterator< const Dotation& > itStock = stocks->CreateIterator();
            while( itStock.HasMoreElements() )
            {
                const Dotation& dotation = itStock.NextElement();
                file << GetName( child ) << separator << dotation.type_.GetName() << separator << dotation.quantity_ << std::endl;
            }
        }
    }
    file.close();
}

// -----------------------------------------------------------------------------
// Name: CsvExport::WriteWeather
// Created: LGY 2011-10-18
// -----------------------------------------------------------------------------
void CsvExport::WriteWeather( boost::filesystem::path& path, const std::string& separator )
{
    bfs::path weatherPath( bfs::path( path / bfs::path( std::string( tools::translate( "CsvExport", "weather" ).toAscii().constData() ) + ".csv" ).filename() ) );
    std::ofstream file( weatherPath.string().c_str() );
    file << tools::translate( "CsvExport", "WIND SPEED" ) << separator << tools::translate( "CsvExport", "WIND DIRECTION" ) << separator
         << tools::translate( "CsvExport", "TEMPERATURE" ) << separator <<  tools::translate( "CsvExport", "CLOUDS FLOOR" ) << separator
         << tools::translate( "CsvExport", "CLOUDS CEILING" ) << separator <<  tools::translate( "CsvExport", "DENSITY CEILING" ) << separator
         << tools::translate( "CsvExport", "WEATHER TYPE" ) << separator <<  tools::translate( "CsvExport", "START TIME" ) << separator
         << tools::translate( "CsvExport", "END TIME" ) << std::endl;

    weather::Meteo& global = *model_.weather_.globalWeather_;
    file << global.GetWind().rSpeed_ << separator << global.GetWind().eAngle_ << separator << global.GetTemperature() << separator
         << global.GetCloud().nFloor_ << separator << global.GetCloud().nCeiling_ << separator << global.GetCloud().nDensityPercentage_ << separator
         << global.GetPrecipitation().GetName() << separator << separator << std::endl;

    tools::Iterator< const weather::MeteoLocal& > it = model_.weather_.CreateIterator();
    while( it.HasMoreElements() )
    {
        const weather::MeteoLocal& meteo = it.NextElement();
        const std::string start = meteo.GetStartTime().is_not_a_date_time() ? "19700101Thhmmss" : boost::posix_time::to_iso_string( meteo.GetStartTime() );
        const std::string end   = meteo.GetEndTime().is_not_a_date_time() ? "19700101Thhmmss" : boost::posix_time::to_iso_string( meteo.GetEndTime() );
        file << meteo.GetWind().rSpeed_ << separator << meteo.GetWind().eAngle_ << separator << meteo.GetTemperature() << separator
             << meteo.GetCloud().nFloor_ << separator << meteo.GetCloud().nCeiling_ << separator << meteo.GetCloud().nDensityPercentage_ << separator
             << meteo.GetPrecipitation().GetName() << separator << start << separator << end << std::endl;
    }
    file.close();
}

// -----------------------------------------------------------------------------
// Name: CsvExport::WriteDiplomaty
// Created: LGY 2011-10-18
// -----------------------------------------------------------------------------
void CsvExport::WriteDiplomaty( boost::filesystem::path& path, const std::string& separator )
{
    bfs::path diplomacyPath( bfs::path( path / bfs::path( std::string( tools::translate( "CsvExport", "diplomacy" ).toAscii().constData() ) + ".csv" ).filename() ) );
    std::ofstream file( diplomacyPath.string().c_str() );
    tools::Iterator< const kernel::Team_ABC& > it = model_.teams_.CreateIterator();
    while( it.HasMoreElements() )
        file << separator<< it.NextElement().GetName();
    file << std::endl;
    tools::Iterator< const kernel::Team_ABC& > itRhs = model_.teams_.CreateIterator();
    while( itRhs.HasMoreElements() )
    {
        const kernel::Team_ABC& rhs = itRhs.NextElement();
        file << rhs.GetName();
        tools::Iterator< const kernel::Team_ABC& > itLhs = model_.teams_.CreateIterator();
        while( itLhs.HasMoreElements() )
        {
            file << separator;
            const kernel::Team_ABC& lhs = itLhs.NextElement();
            if( lhs.GetId() != rhs.GetId() )
               file << rhs.Get< kernel::Diplomacies_ABC >().GetDiplomacy( lhs ).GetName();
        }
        file << std::endl;
    }
    file.close();
}

// -----------------------------------------------------------------------------
// Name: CsvExport::WriteProfiles
// Created: LGY 2011-10-19
// -----------------------------------------------------------------------------
void CsvExport::WriteProfiles( boost::filesystem::path& path, const std::string& separator )
{
    bfs::path profilesPath( bfs::path( path / bfs::path( std::string( tools::translate( "CsvExport", "profiles" ).toAscii().constData() ) + ".csv" ).filename() ) );
    std::ofstream file( profilesPath.string().c_str() );
    ProfilesModel::T_Profiles profiles;
    model_.profiles_.Visit( profiles );
    BOOST_FOREACH( const std::string& profile, profiles )
        file << separator << profile;
    file << std::endl;
    tools::Iterator< const kernel::Automat_ABC& > itAutomat = model_.GetAutomatResolver().CreateIterator();
    while( itAutomat.HasMoreElements() )
        WriteProfiles( file, separator, itAutomat.NextElement(), profiles );
    tools::Iterator< const kernel::Population_ABC& > itPopulation = model_.GetPopulationResolver().CreateIterator();
    while( itPopulation.HasMoreElements() )
        WriteProfiles( file, separator, itPopulation.NextElement(), profiles );
    file.close();
}

// -----------------------------------------------------------------------------
// Name: CsvExport::WriteProfiles
// Created: LGY 2011-10-19
// -----------------------------------------------------------------------------
void CsvExport::WriteProfiles( std::ofstream& file, const std::string& separator, const kernel::Entity_ABC& entity,
                               const std::set< std::string >& profiles )
{
    file << GetName( entity );
    BOOST_FOREACH( const std::string& profile, profiles )
    {
        file << separator;
        if( model_.profiles_.IsWriteable( entity, profile ) )
            file << "RW";
        else if( model_.profiles_.IsReadable( entity, profile ) )
            file << "R";
    }
    file << std::endl;
}

// -----------------------------------------------------------------------------
// Name: CsvExport::WriteLogistic
// Created: LGY 2011-10-19
// -----------------------------------------------------------------------------
void CsvExport::WriteLogistic( boost::filesystem::path& path, const std::string& separator )
{
    bfs::path logisticPath( bfs::path( path / bfs::path( std::string( tools::translate( "CsvExport", "logistic" ).toAscii().constData() ) + ".csv" ).filename() ) );
    std::ofstream file( logisticPath.string().c_str() );
    tools::Iterator< const kernel::Automat_ABC& > itTC2 = model_.GetAutomatResolver().CreateIterator();
    while( itTC2.HasMoreElements() )
    {
        const kernel::Automat_ABC& child = itTC2.NextElement();
        if( child.GetLogisticLevel() == kernel::LogisticLevel::logistic_base_ )
            file << separator << GetName( child );
    }
    tools::Iterator< const kernel::Formation_ABC& > itBL = model_.GetFormationResolver().CreateIterator();
    while( itBL.HasMoreElements() )
    {
        const kernel::Formation_ABC& child = itBL.NextElement();
        if( child.GetLogisticLevel() == kernel::LogisticLevel::logistic_base_ )
            file << separator << GetName( child );
    }
    file << std::endl;
    tools::Iterator< const kernel::Automat_ABC& > itAutomat = model_.GetAutomatResolver().CreateIterator();
    while( itAutomat.HasMoreElements() )
    {
        const kernel::Automat_ABC& child = itAutomat.NextElement();
        file << GetName( child );
        WriteLogistic( file, separator, child );
        file << std::endl;
    }
    tools::Iterator< const kernel::Formation_ABC& > itFormation = model_.GetFormationResolver().CreateIterator();
    while( itFormation.HasMoreElements() )
    {
        const kernel::Formation_ABC& child = itFormation.NextElement();
        if( child.GetLogisticLevel() == kernel::LogisticLevel::logistic_base_ )
        {
            file << GetName( child );
            WriteLogistic( file, separator, child );
            file << std::endl;
        }
    }
    file.close();
}

namespace
{
    bool IsLogisticSuperior( const kernel::Entity_ABC& entity, const kernel::Entity_ABC& superior )
    {
        if( const LogisticHierarchiesBase* pHierarchy = entity.Retrieve< LogisticHierarchiesBase >() )
            if( pHierarchy->GetSuperior() && pHierarchy->GetSuperior()->GetId() == superior.GetId() )
                return true;
        return false;
    }
}

// -----------------------------------------------------------------------------
// Name: CsvExport::WriteLogistic
// Created: LGY 2011-10-19
// -----------------------------------------------------------------------------
void CsvExport::WriteLogistic( std::ofstream& file, const std::string& separator, const kernel::Entity_ABC& entity )
{
    tools::Iterator< const kernel::Automat_ABC& > itTC2 = model_.GetAutomatResolver().CreateIterator();
    while( itTC2.HasMoreElements() )
    {
        const kernel::Automat_ABC& child = itTC2.NextElement();
        if( child.GetLogisticLevel() == kernel::LogisticLevel::logistic_base_ )
        {
            file << separator;
            if( IsLogisticSuperior( entity, child ) )
                file << "X";
        }
    }
    tools::Iterator< const kernel::Formation_ABC& > itBL = model_.GetFormationResolver().CreateIterator();
    while( itBL.HasMoreElements() )
    {
        const kernel::Formation_ABC& child = itBL.NextElement();
        if( child.GetLogisticLevel() == kernel::LogisticLevel::logistic_base_ )
        {
            file << separator;
            if( IsLogisticSuperior( entity, child ) )
                file << "X";
        }
    }
}
