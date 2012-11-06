// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_People_Data.h"
#include "ADN_Workspace.h"
#include "ADN_Project_Data.h"
#include "ADN_DataException.h"
#include "ADN_Tr.h"
#pragma warning( push, 0 )
#include <boost/algorithm/string.hpp>
#pragma warning( pop )
#include <boost/lexical_cast.hpp>

tools::IdManager ADN_People_Data::idManager_;

// -----------------------------------------------------------------------------
// Name: EventInfos::EventInfos
// Created: LGY 2011-01-18
// -----------------------------------------------------------------------------
ADN_People_Data::EventInfos::EventInfos()
    : ptrAccommodation_( ADN_Workspace::GetWorkspace().GetUrban().GetData().GetAccommodationsInfos(), 0 )
    , day_( eDays_Monday )
    , from_( "00:00" )
    , to_( "00:00" )
{
    day_.SetAlphabeticalSort( false );
}

// -----------------------------------------------------------------------------
// Name: EventInfos::~EventInfos
// Created: LGY 2011-01-18
// -----------------------------------------------------------------------------
ADN_People_Data::EventInfos::~EventInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EventInfos::CreateCopy
// Created: LGY 2011-01-18
// -----------------------------------------------------------------------------
ADN_People_Data::EventInfos* ADN_People_Data::EventInfos::CreateCopy()
{
    EventInfos* pCopy = new EventInfos();

    pCopy->day_ = day_.GetData();
    pCopy->from_ = from_.GetData();
    pCopy->to_ = to_.GetData();
    pCopy->ptrAccommodation_ = ptrAccommodation_.GetData();
    pCopy->ptrAccommodation_.SetVector( ptrAccommodation_.GetVector() );

    return pCopy;
}

// -----------------------------------------------------------------------------
// Name: EventInfos::ReadArchive
// Created: LGY 2011-01-18
// -----------------------------------------------------------------------------
void ADN_People_Data::EventInfos::ReadArchive( xml::xistream& input )
{
    std::string day, accommodation;
    day = input.attribute< std::string >( "day" );
    day_ = ADN_Tr::ConvertToDays( day );
    from_ = input.attribute< std::string >( "from" );
    to_ = input.attribute< std::string >( "to" );
    accommodation = input.attribute< std::string >( "motivation" );

    ADN_Urban_Data::AccommodationInfos* pAccommodation = ADN_Workspace::GetWorkspace().GetUrban().GetData().FindAccommodation( accommodation );
    if( !pAccommodation )
        throw ADN_DataException( tools::translate( "People_Data", "Invalid data" ).toAscii().constData(), tools::translate( "People_Data", "Population - Invalid accommodation '%1'" ).arg( accommodation.c_str() ).toAscii().constData() );
    ptrAccommodation_ = pAccommodation;
}

// -----------------------------------------------------------------------------
// Name: EventInfos::WriteArchive
// Created: LGY 2011-01-18
// -----------------------------------------------------------------------------
void ADN_People_Data::EventInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "event" )
           << xml::attribute( "day", day_.Convert() )
           << xml::attribute( "from", from_ )
           << xml::attribute( "to", to_ )
           << xml::attribute( "motivation", ptrAccommodation_.GetData()->strName_ )
           << xml::end;
}

// -----------------------------------------------------------------------------
// Name: PeopleInfosConsumption::PeopleInfosConsumption
// Created: JSR 2011-01-31
// -----------------------------------------------------------------------------
ADN_People_Data::PeopleInfosConsumption::PeopleInfosConsumption()
    : ptrResource_( ADN_Workspace::GetWorkspace().GetResourceNetworks().GetData().GetResourceNetworksInfos(), 0 )
    , consumption_( 0 )
{
    BindExistenceTo( &ptrResource_ );
}

// -----------------------------------------------------------------------------
// Name: PeopleInfosConsumption::GetItemName
// Created: JSR 2011-01-31
// -----------------------------------------------------------------------------
std::string ADN_People_Data::PeopleInfosConsumption::GetItemName()
{
    return ptrResource_.GetData()->strName_.GetData();
}

// -----------------------------------------------------------------------------
// Name: PeopleInfosConsumption::CreateCopy
// Created: JSR 2011-01-31
// -----------------------------------------------------------------------------
ADN_People_Data::PeopleInfosConsumption* ADN_People_Data::PeopleInfosConsumption::CreateCopy()
{
    PeopleInfosConsumption* pCopy = new PeopleInfosConsumption();
    pCopy->ptrResource_ = ptrResource_.GetData();
    pCopy->ptrResource_.SetVector( ptrResource_.GetVector() );
    pCopy->consumption_ = consumption_.GetData();
    return pCopy;
}

// -----------------------------------------------------------------------------
// Name: PeopleInfosConsumption::ReadArchive
// Created: JSR 2011-01-31
// -----------------------------------------------------------------------------
void ADN_People_Data::PeopleInfosConsumption::ReadArchive( xml::xistream& xis )
{
    std::string strResource;
    xis >> xml::attribute( "type", strResource )
        >> xml::attribute( "need", consumption_ );
    ADN_ResourceNetworks_Data::ResourceNetworkInfos* pResource = ADN_Workspace::GetWorkspace().GetResourceNetworks().GetData().FindResourceNetwork( strResource );
    if( !pResource )
        throw ADN_DataException( tools::translate( "People_Data", "Invalid data" ).toAscii().constData(), tools::translate( "People_Data", "Population - Invalid resource '%1/%2'" ).arg( strResource.c_str() ).toAscii().constData() );
    ptrResource_ = pResource;
}

// -----------------------------------------------------------------------------
// Name: PeopleInfosConsumption::WriteArchive
// Created: JSR 2011-01-31
// -----------------------------------------------------------------------------
void ADN_People_Data::PeopleInfosConsumption::WriteArchive( xml::xostream& xos ) const
{
    xos << xml::start( "resource" )
            << xml::attribute( "type", ptrResource_.GetData()->strName_ )
            << xml::attribute( "need", consumption_ )
        << xml::end;
}

// =============================================================================
// ADN_People_Data::PeopleInfos
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PeopleInfos::PeopleInfos
// Created: SLG 2010-11-22
// -----------------------------------------------------------------------------
ADN_People_Data::PeopleInfos::PeopleInfos()
    : nId_                ( ADN_People_Data::idManager_.GetNextId() )
    , ptrModel_           ( ADN_Workspace::GetWorkspace().GetPopulation().GetData().GetPopulation(), 0 )
    , repartition_        ( tools::translate( "People_Data", "Population" ) )
    , securityLossOnFire_ ( 0 )
    , securityGainPerHour_( 0 )
    , healthNeed_         ( 0 )
    , transferTime_       ( "0h" )
{
    BindExistenceTo( &ptrModel_ );
}

// -----------------------------------------------------------------------------
// Name: PeopleInfos::PeopleInfos
// Created: SLG 2010-11-22
// -----------------------------------------------------------------------------
ADN_People_Data::PeopleInfos::PeopleInfos( unsigned int id )
    : nId_                ( id )
    , ptrModel_           ( ADN_Workspace::GetWorkspace().GetPopulation().GetData().GetPopulation(), 0 )
    , repartition_        ( tools::translate( "People_Data", "Population" ) )
    , securityLossOnFire_ ( 0 )
    , securityGainPerHour_( 0 )
    , healthNeed_         ( 0 )
    , transferTime_       ( "0h" )
{
    BindExistenceTo( &ptrModel_ );
    ADN_People_Data::idManager_.Lock( id );
}

// -----------------------------------------------------------------------------
// Name: PeopleInfos::~PeopleInfos
// Created: SLG 2010-11-22
// -----------------------------------------------------------------------------
ADN_People_Data::PeopleInfos::~PeopleInfos()
{
    consumptions_.Delete();
}

// -----------------------------------------------------------------------------
// Name: PeopleInfos::CreateCopy
// Created: SLG 2010-11-22
// -----------------------------------------------------------------------------
ADN_People_Data::PeopleInfos* ADN_People_Data::PeopleInfos::CreateCopy()
{
    PeopleInfos* pCopy = new PeopleInfos();
    pCopy->ptrModel_ = ptrModel_.GetData();
    pCopy->strAngryCrowdMission_ = strAngryCrowdMission_.GetData();
    pCopy->repartition_ = repartition_;
    pCopy->transferTime_ = transferTime_.GetData();
    pCopy->securityLossOnFire_ = securityLossOnFire_.GetData();
    pCopy->securityGainPerHour_ = securityGainPerHour_.GetData();
    pCopy->healthNeed_ = healthNeed_.GetData();

    for( IT_EventInfosVector it = schedule_.begin(); it != schedule_.end(); ++it )
    {
        EventInfos* pNew = ( *it )->CreateCopy();
        pCopy->schedule_.AddItem( pNew );
    }
    for( IT_PeopleInfosConsumptionVector itConsumption = consumptions_.begin(); itConsumption != consumptions_.end(); ++itConsumption )
    {
        PeopleInfosConsumption* pNew = ( *itConsumption )->CreateCopy();
        pCopy->consumptions_.AddItem( pNew );
    }
    return pCopy;
}

// -----------------------------------------------------------------------------
// Name: PeopleInfos::ReadArchive
// Created: SLG 2010-11-22
// -----------------------------------------------------------------------------
void ADN_People_Data::PeopleInfos::ReadArchive( xml::xistream& input )
{
    std::string strModel;
    input >> xml::attribute( "name", strName_ )
          >> xml::attribute( "associated-crowd", strModel )
          >> xml::optional >> xml::attribute( "angry-crowd-mission", strAngryCrowdMission_ );

    ADN_Population_Data::PopulationInfos* pModel = ADN_Workspace::GetWorkspace().GetPopulation().GetData().FindPopulation( strModel );
    if( !pModel )
        throw ADN_DataException( "Invalid data", tools::translate( "Population_Data", "Population types - Invalid Population type '%1'" ).arg( strModel.c_str() ).toAscii().constData() );
    ptrModel_ = pModel;

    input >> xml::start( "repartition" );
    repartition_.ReadArchive( input );
    input >> xml::end
          >> xml::start( "schedule" )
            >> xml::attribute( "transfer-time", transferTime_ )
            >> xml::list( "event", *this, &ADN_People_Data::PeopleInfos::ReadEvent )
          >> xml::end
          >> xml::start( "safety-level" )
            >> xml::attribute( "loss-on-fire", securityLossOnFire_ )
            >> xml::attribute( "gain-per-hour", securityGainPerHour_ )
          >> xml::end
          >> xml::start( "consumption" )
            >> xml::list( "resource", *this, &ADN_People_Data::PeopleInfos::ReadConsumption )
          >> xml::end
          >> xml::start( "health-need" )
            >> xml::attribute( "people-per-facility", healthNeed_ )
          >> xml::end;
    securityLossOnFire_ = 100 * securityLossOnFire_.GetData();
    securityGainPerHour_ = 100 * securityGainPerHour_.GetData();
}

namespace
{
    QTime CreateTime( const std::string& date )
    {
        std::vector< std::string > result;
        boost::algorithm::split( result, date, boost::algorithm::is_any_of( ":" ) );
        if( result.size() != 2 )
            throw std::runtime_error( "Invalid date event" );
        try
        {
            return QTime( boost::lexical_cast< int >( result.front() ), boost::lexical_cast< int >( result.back() ) );
        }
        catch( boost::bad_lexical_cast& /*e*/ )
        {
            throw std::runtime_error( "Invalid date event" );
        }
    }

    bool CheckTime( const std::string& from, const std::string& to, const std::string& dataFrom, const std::string& dataTo )
    {
        QTime qFrom( CreateTime( from ) );
        QTime qTo( CreateTime( to ) );
        QTime qDataFrom( CreateTime( dataFrom ) );
        QTime qDataTo( CreateTime( dataTo ) );
        return ( qFrom < qDataFrom && qTo <= qDataFrom ) || ( qFrom >= qDataTo && qTo > qDataTo );
    }
}
// -----------------------------------------------------------------------------
// Name: ADN_People_Data::CheckErrors
// Created: LGY 2011-01-31
// -----------------------------------------------------------------------------
const std::string ADN_People_Data::PeopleInfos::CheckErrors() const
{
    for( CIT_EventInfosVector it1 = schedule_.begin(); it1 != schedule_.end(); ++it1 )
        for( CIT_EventInfosVector it2 = schedule_.begin(); it2 != schedule_.end(); ++it2 )
            if( it1 != it2 && ( *it1 )->day_.GetData() == ( *it2 )->day_.GetData() &&
                !CheckTime( ( *it1 )->from_.GetData(), ( *it1 )->to_.GetData(), ( *it2 )->from_.GetData(), ( *it2 )->to_.GetData() ) )
                return tools::translate( "People_Data", "Invalid schedule - You have already an appointment on the same moment :" ).toStdString() + std::string( "\n" ) +
                                         "- " + ( *it1 )->day_.Convert( ENT_Tr_ABC::eToTr ) + " : " + ( *it1 )->from_.GetData() + " / " + ( *it1 )->to_.GetData() + "\n" +
                                         "- " + ( *it2 )->day_.Convert( ENT_Tr_ABC::eToTr ) + " : " + ( *it2 )->from_.GetData() + " / " + ( *it2 )->to_.GetData() + "\n";
    repartition_.CheckNoError( "ADN_People_Data", strName_.GetData().c_str() );
    return "";
}

// -----------------------------------------------------------------------------
// Name: PeopleInfos::WriteArchive
// Created: SLG 2010-11-22
// -----------------------------------------------------------------------------
void ADN_People_Data::PeopleInfos::WriteArchive( xml::xostream& output ) const
{
    const std::string error = CheckErrors();
    if( error != "" )
        throw ADN_DataException( tools::translate( "Categories_Data", "Invalid data" ).toAscii().constData(), error );
    output << xml::start( "population" )
            << xml::attribute( "name", strName_ )
            << xml::attribute( "id", nId_ )
            << xml::attribute( "associated-crowd", ptrModel_.GetData()->strName_ );
    if( !strAngryCrowdMission_.GetData().empty() )
        output << xml::attribute( "angry-crowd-mission", strAngryCrowdMission_ );
    output << xml::start( "repartition" );
    repartition_.WriteArchive( output );
    output  << xml::end
            << xml::start( "schedule" )
                << xml::attribute( "transfer-time", transferTime_ );
    for( CIT_EventInfosVector it = schedule_.begin(); it != schedule_.end(); ++it )
        ( *it )->WriteArchive( output );
    output  << xml::end
            << xml::start( "safety-level" )
                << xml::attribute( "loss-on-fire", securityLossOnFire_.GetData() / 100.0 )
                << xml::attribute( "gain-per-hour", securityGainPerHour_.GetData() / 100.0 )
            << xml::end
            << xml::start( "health-need" )
                << xml::attribute( "people-per-facility", healthNeed_ )
            << xml::end
            << xml::start( "consumption" );
    for( CIT_PeopleInfosConsumptionVector it = consumptions_.begin(); it != consumptions_.end(); ++it )
        ( *it )->WriteArchive( output );
    output  << xml::end
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_People_Data::ReadEvent
// Created: LGY 2011-01-18
// -----------------------------------------------------------------------------
void ADN_People_Data::PeopleInfos::ReadEvent( xml::xistream& input )
{
    std::auto_ptr< EventInfos > spNew( new EventInfos() );
    spNew->ReadArchive( input );
    schedule_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: PeopleInfos::ReadConsumption
// Created: JSR 2011-01-31
// -----------------------------------------------------------------------------
void ADN_People_Data::PeopleInfos::ReadConsumption( xml::xistream& input )
{
    std::auto_ptr< PeopleInfosConsumption > spNew( new PeopleInfosConsumption() );
    spNew->ReadArchive( input );
    consumptions_.AddItem( spNew.release() );
}

// =============================================================================
// ADN_People_Data
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ADN_People_Data constructor
// Created: SLG 2010-11-22
// -----------------------------------------------------------------------------
ADN_People_Data::ADN_People_Data()
    : ADN_Data_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_People_Data destructor
// Created: SLG 2010-11-22
// -----------------------------------------------------------------------------
ADN_People_Data::~ADN_People_Data()
{
    Reset();
}

// -----------------------------------------------------------------------------
// Name: ADN_People_Data::Reset
// Created: SLG 2010-11-22
// -----------------------------------------------------------------------------
void ADN_People_Data::Reset()
{
    idManager_.Reset();
    vPeople_.Reset();
}

// -----------------------------------------------------------------------------
// Name: ADN_People_Data::FilesNeeded
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
void ADN_People_Data::FilesNeeded( T_StringList& vFiles ) const
{
    vFiles.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szPeople_.GetData() );
}

// -----------------------------------------------------------------------------
// Name: ADN_People_Data::ReadArchive
// Created: SLG 2010-11-22
// -----------------------------------------------------------------------------
void ADN_People_Data::ReadArchive( xml::xistream& input )
{
    input >> xml::start( "populations" )
              >> xml::list( "population", *this, &ADN_People_Data::ReadPeople )
          >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_People_Data::ReadPeople
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
void ADN_People_Data::ReadPeople( xml::xistream& input )
{
    std::auto_ptr< PeopleInfos > spNew( new PeopleInfos( input.attribute< unsigned int >( "id" ) ) );
    spNew->ReadArchive( input );
    vPeople_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_People_Data::WriteArchive
// Created: SLG 2010-11-22
// -----------------------------------------------------------------------------
void ADN_People_Data::WriteArchive( xml::xostream& output )
{
    output << xml::start( "populations" );
    ADN_Tools::AddSchema( output, "Inhabitants" );
    for( CIT_PeopleInfosVector it = vPeople_.begin(); it != vPeople_.end(); ++it )
        ( *it )->WriteArchive( output );
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_People_Data::GetPeopleThatUse
// Created: ABR 2012-08-01
// -----------------------------------------------------------------------------
QStringList ADN_People_Data::GetPeopleThatUse( ADN_Population_Data::PopulationInfos& population )
{
    QStringList result;
    for( CIT_PeopleInfosVector it = vPeople_.begin(); it != vPeople_.end(); ++it )
        if( ( *it )->ptrModel_.GetData()->strName_.GetData() == population.strName_.GetData() )
            result << ( *it )->strName_.GetData().c_str();
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_People_Data::GetPeopleThatUse
// Created: ABR 2012-08-02
// -----------------------------------------------------------------------------
QStringList ADN_People_Data::GetPeopleThatUse( ADN_ResourceNetworks_Data::ResourceNetworkInfos& network )
{
    QStringList result;
    for( CIT_PeopleInfosVector it = vPeople_.begin(); it != vPeople_.end(); ++it )
        for( CIT_PeopleInfosConsumptionVector itConsumption = ( *it )->consumptions_.begin(); itConsumption != ( *it )->consumptions_.end(); ++itConsumption )
            if( ( *itConsumption )->ptrResource_.GetData()->strName_.GetData() == network.strName_.GetData() )
                result << ( *it )->strName_.GetData().c_str();
    return result;
}
