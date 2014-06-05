// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math?tiques Appliqu? SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Inhabitants_Data.h"
#include "ADN_Workspace.h"
#include "ADN_Project_Data.h"
#include "ADN_Tr.h"
#include "ADN_WorkspaceElement.h"
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

tools::IdManager ADN_Inhabitants_Data::idManager_;

// -----------------------------------------------------------------------------
// Name: EventInfos::EventInfos
// Created: LGY 2011-01-18
// -----------------------------------------------------------------------------
ADN_Inhabitants_Data::EventInfos::EventInfos()
    : ptrAccommodation_( ADN_Workspace::GetWorkspace().GetUrban().GetData().GetAccommodationsInfos(), 0 )
    , day_( eDays_Monday )
    , from_( "00:00" )
    , to_( "00:00" )
{
    BindExistenceTo( &ptrAccommodation_ );
    day_.SetAlphabeticalSort( false );
}

// -----------------------------------------------------------------------------
// Name: EventInfos::~EventInfos
// Created: LGY 2011-01-18
// -----------------------------------------------------------------------------
ADN_Inhabitants_Data::EventInfos::~EventInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EventInfos::CreateCopy
// Created: LGY 2011-01-18
// -----------------------------------------------------------------------------
ADN_Inhabitants_Data::EventInfos* ADN_Inhabitants_Data::EventInfos::CreateCopy()
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
void ADN_Inhabitants_Data::EventInfos::ReadArchive( xml::xistream& input )
{
    std::string day = input.attribute< std::string >( "day" );
    day_ = ADN_Tr::ConvertToDays( day );
    from_ = input.attribute< std::string >( "from" );
    to_ = input.attribute< std::string >( "to" );
    input >> xml::attribute( "motivation", ptrAccommodation_ );
}

// -----------------------------------------------------------------------------
// Name: EventInfos::WriteArchive
// Created: LGY 2011-01-18
// -----------------------------------------------------------------------------
void ADN_Inhabitants_Data::EventInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "event" )
           << xml::attribute( "day", day_.Convert() )
           << xml::attribute( "from", from_ )
           << xml::attribute( "to", to_ )
           << xml::attribute( "motivation", ptrAccommodation_ )
           << xml::end;
}

// -----------------------------------------------------------------------------
// Name: InhabitantsInfosConsumption::InhabitantsInfosConsumption
// Created: JSR 2011-01-31
// -----------------------------------------------------------------------------
ADN_Inhabitants_Data::InhabitantsInfosConsumption::InhabitantsInfosConsumption()
    : ADN_CrossedRef( ADN_Workspace::GetWorkspace().GetResourceNetworks().GetData().GetResourceNetworksInfos(), 0, true )
    , consumption_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InhabitantsInfosConsumption::GetItemName
// Created: JSR 2011-01-31
// -----------------------------------------------------------------------------
std::string ADN_Inhabitants_Data::InhabitantsInfosConsumption::GetItemName()
{
    return GetCrossedElement() ? GetCrossedElement()->strName_.GetData() : "";
}

// -----------------------------------------------------------------------------
// Name: InhabitantsInfosConsumption::CreateCopy
// Created: JSR 2011-01-31
// -----------------------------------------------------------------------------
ADN_Inhabitants_Data::InhabitantsInfosConsumption* ADN_Inhabitants_Data::InhabitantsInfosConsumption::CreateCopy()
{
    InhabitantsInfosConsumption* pCopy = new InhabitantsInfosConsumption();
    pCopy->SetCrossedElement( GetCrossedElement() );
    pCopy->consumption_ = consumption_.GetData();
    return pCopy;
}

// -----------------------------------------------------------------------------
// Name: InhabitantsInfosConsumption::ReadArchive
// Created: JSR 2011-01-31
// -----------------------------------------------------------------------------
void ADN_Inhabitants_Data::InhabitantsInfosConsumption::ReadArchive( xml::xistream& xis )
{
    xis >> xml::attribute( "type", *this )
        >> xml::attribute( "need", consumption_ );
}

// -----------------------------------------------------------------------------
// Name: InhabitantsInfosConsumption::WriteArchive
// Created: JSR 2011-01-31
// -----------------------------------------------------------------------------
void ADN_Inhabitants_Data::InhabitantsInfosConsumption::WriteArchive( xml::xostream& xos ) const
{
    xos << xml::start( "resource" )
          << xml::attribute( "type", *this )
          << xml::attribute( "need", consumption_ )
        << xml::end;
}

// =============================================================================
// ADN_Inhabitants_Data::InhabitantsInfos
// =============================================================================

// -----------------------------------------------------------------------------
// Name: InhabitantsInfos::InhabitantsInfos
// Created: SLG 2010-11-22
// -----------------------------------------------------------------------------
ADN_Inhabitants_Data::InhabitantsInfos::InhabitantsInfos()
    : nId_                ( ADN_Inhabitants_Data::idManager_.GetNextId() )
    , ptrModel_           ( ADN_Workspace::GetWorkspace().GetCrowds().GetData().GetCrowds(), 0 )
    , repartition_        ( tools::translate( "People_Data", "Population" ) )
    , securityLossOnFire_ ( 0 )
    , securityGainPerHour_( 0 )
    , healthNeed_         ( 0 )
    , transferTime_       ( "0h" )
{
    strName_.SetContext( ADN_Workspace::GetWorkspace().GetContext( eInhabitants, "inhabitants" ) );
    BindExistenceTo( &ptrModel_ );
}

// -----------------------------------------------------------------------------
// Name: InhabitantsInfos::InhabitantsInfos
// Created: SLG 2010-11-22
// -----------------------------------------------------------------------------
ADN_Inhabitants_Data::InhabitantsInfos::InhabitantsInfos( unsigned int id )
    : nId_                ( id )
    , ptrModel_           ( ADN_Workspace::GetWorkspace().GetCrowds().GetData().GetCrowds(), 0 )
    , repartition_        ( tools::translate( "People_Data", "Population" ) )
    , securityLossOnFire_ ( 0 )
    , securityGainPerHour_( 0 )
    , healthNeed_         ( 0 )
    , transferTime_       ( "0h" )
{
    strName_.SetContext( ADN_Workspace::GetWorkspace().GetContext( eInhabitants, "inhabitants" ) );
    BindExistenceTo( &ptrModel_ );
    ADN_Inhabitants_Data::idManager_.Lock( id );
}

// -----------------------------------------------------------------------------
// Name: InhabitantsInfos::~InhabitantsInfos
// Created: SLG 2010-11-22
// -----------------------------------------------------------------------------
ADN_Inhabitants_Data::InhabitantsInfos::~InhabitantsInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InhabitantsInfos::CreateCopy
// Created: SLG 2010-11-22
// -----------------------------------------------------------------------------
ADN_Inhabitants_Data::InhabitantsInfos* ADN_Inhabitants_Data::InhabitantsInfos::CreateCopy()
{
    InhabitantsInfos* pCopy = new InhabitantsInfos();
    pCopy->ptrModel_ = ptrModel_.GetData();
    pCopy->strAngryCrowdMission_ = strAngryCrowdMission_.GetData();
    pCopy->repartition_ = repartition_;
    pCopy->transferTime_ = transferTime_.GetData();
    pCopy->securityLossOnFire_ = securityLossOnFire_.GetData();
    pCopy->securityGainPerHour_ = securityGainPerHour_.GetData();
    pCopy->healthNeed_ = healthNeed_.GetData();

    for( auto it = schedule_.begin(); it != schedule_.end(); ++it )
    {
        EventInfos* pNew = ( *it )->CreateCopy();
        pCopy->schedule_.AddItem( pNew );
    }
    for( auto itConsumption = consumptions_.begin(); itConsumption != consumptions_.end(); ++itConsumption )
    {
        InhabitantsInfosConsumption* pNew = ( *itConsumption )->CreateCopy();
        pCopy->consumptions_.AddItem( pNew );
    }
    return pCopy;
}

// -----------------------------------------------------------------------------
// Name: InhabitantsInfos::ReadArchive
// Created: SLG 2010-11-22
// -----------------------------------------------------------------------------
void ADN_Inhabitants_Data::InhabitantsInfos::ReadArchive( xml::xistream& input )
{
    std::string strModel;
    input >> xml::attribute( "name", *this )
          >> xml::attribute( "associated-crowd", strModel )
          >> xml::optional >> xml::attribute( "angry-crowd-mission", strAngryCrowdMission_ );

    ADN_Crowds_Data::CrowdsInfos* pModel = ADN_Workspace::GetWorkspace().GetCrowds().GetData().FindCrowd( strModel );
    if( !pModel )
        throw MASA_EXCEPTION( tools::translate( "Population_Data", "Population types - Invalid Population type '%1'" ).arg( strModel.c_str() ).toStdString() );
    ptrModel_ = pModel;

    input >> xml::start( "repartition" );
    repartition_.ReadArchive( input );
    input >> xml::end
          >> xml::start( "schedule" )
            >> xml::attribute( "transfer-time", transferTime_ )
            >> xml::list( "event", *this, &ADN_Inhabitants_Data::InhabitantsInfos::ReadEvent )
          >> xml::end
          >> xml::start( "safety-level" )
            >> xml::attribute( "loss-on-fire", securityLossOnFire_ )
            >> xml::attribute( "gain-per-hour", securityGainPerHour_ )
          >> xml::end
          >> xml::start( "consumption" )
            >> xml::list( "resource", *this, &ADN_Inhabitants_Data::InhabitantsInfos::ReadConsumption )
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
            throw MASA_EXCEPTION( "Invalid date event" );
        try
        {
            return QTime( boost::lexical_cast< int >( result.front() ), boost::lexical_cast< int >( result.back() ) );
        }
        catch( const std::exception& /*e*/ )
        {
            throw MASA_EXCEPTION( "Invalid date event" );
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
// Name: ADN_Inhabitants_Data::CheckErrors
// Created: LGY 2011-01-31
// -----------------------------------------------------------------------------
void ADN_Inhabitants_Data::InhabitantsInfos::CheckErrors( ADN_ConsistencyChecker& checker ) const
{
    for( auto it1 = schedule_.begin(); it1 != schedule_.end(); ++it1 )
        for( auto it2 = schedule_.begin(); it2 != schedule_.end(); ++it2 )
            if( it1 != it2 && ( *it1 )->day_.GetData() == ( *it2 )->day_.GetData() &&
                !CheckTime( ( *it1 )->from_.GetData(), ( *it1 )->to_.GetData(), ( *it2 )->from_.GetData(), ( *it2 )->to_.GetData() ) )
                checker.AddError( eInhabitantSchedule, 
                    tools::translate( "People_Data", "Invalid schedule - You have already an appointment on the same moment :" ).toStdString() + std::string( "\n" ) +
                                         "- " + ( *it1 )->day_.Convert( ENT_Tr::eToTr ) + " : " + ( *it1 )->from_.GetData() + " / " + ( *it1 )->to_.GetData() + "\n" +
                                         "- " + ( *it2 )->day_.Convert( ENT_Tr::eToTr ) + " : " + ( *it2 )->from_.GetData() + " / " + ( *it2 )->to_.GetData() + "\n",
                    eInhabitants );
    repartition_.CheckNoError( strName_.GetData().c_str(), checker, eInhabitants );
}

// -----------------------------------------------------------------------------
// Name: InhabitantsInfos::WriteArchive
// Created: SLG 2010-11-22
// -----------------------------------------------------------------------------
void ADN_Inhabitants_Data::InhabitantsInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "population" )
            << xml::attribute( "name", *this )
            << xml::attribute( "id", nId_ )
            << xml::attribute( "associated-crowd", ptrModel_ );
    if( !strAngryCrowdMission_.GetData().empty() )
        output << xml::attribute( "angry-crowd-mission", strAngryCrowdMission_ );
    output << xml::start( "repartition" );
    repartition_.WriteArchive( output );
    output  << xml::end
            << xml::start( "schedule" )
                << xml::attribute( "transfer-time", transferTime_ );
    for( auto it = schedule_.begin(); it != schedule_.end(); ++it )
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
    for( auto it = consumptions_.begin(); it != consumptions_.end(); ++it )
        ( *it )->WriteArchive( output );
    output  << xml::end
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Inhabitants_Data::ReadEvent
// Created: LGY 2011-01-18
// -----------------------------------------------------------------------------
void ADN_Inhabitants_Data::InhabitantsInfos::ReadEvent( xml::xistream& input )
{
    std::unique_ptr< EventInfos > spNew( new EventInfos() );
    spNew->ReadArchive( input );
    if( spNew->ptrAccommodation_.GetData() )
        schedule_.AddItem( spNew.release() );
    else
        ADN_ConsistencyChecker::AddLoadingError( eInvalidCrossedRef, strName_.GetData(), eInhabitants, -1, tools::translate( "ADN_Inhabitants_Data", "Schedule" ).toStdString() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Inhabitants_Data::CheckDatabaseValidity
// Created: JSR 2013-04-12
// -----------------------------------------------------------------------------
void ADN_Inhabitants_Data::InhabitantsInfos::CheckDatabaseValidity( ADN_ConsistencyChecker& checker, const std::string& name, int tab, int subTab /* = -1 */, const std::string& /* optional = "" */ )
{
    ptrModel_.CheckValidity( checker, name, tab, subTab, tools::translate( "ADN_Inhabitants_Data", "Associated Crowd" ).toStdString() );
    CheckErrors( checker );
}

// -----------------------------------------------------------------------------
// Name: InhabitantsInfos::ReadConsumption
// Created: JSR 2011-01-31
// -----------------------------------------------------------------------------
void ADN_Inhabitants_Data::InhabitantsInfos::ReadConsumption( xml::xistream& input )
{
    std::unique_ptr< InhabitantsInfosConsumption > spNew( new InhabitantsInfosConsumption() );
    spNew->ReadArchive( input );
    if( spNew->GetCrossedElement() )
        consumptions_.AddItem( spNew.release() );
    else
        ADN_ConsistencyChecker::AddLoadingError( eInvalidCrossedRef, strName_.GetData(), eInhabitants, -1, tools::translate( "ADN_Inhabitants_Data", "Consumptions" ).toStdString() );
}

// =============================================================================
// ADN_Inhabitants_Data
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ADN_Inhabitants_Data constructor
// Created: SLG 2010-11-22
// -----------------------------------------------------------------------------
ADN_Inhabitants_Data::ADN_Inhabitants_Data()
    : ADN_Data_ABC( eInhabitants )
{
    vInhabitants_.AddUniquenessChecker( eError, duplicateName_, &ADN_Tools::NameExtractor );
}

// -----------------------------------------------------------------------------
// Name: ADN_Inhabitants_Data destructor
// Created: SLG 2010-11-22
// -----------------------------------------------------------------------------
ADN_Inhabitants_Data::~ADN_Inhabitants_Data()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Inhabitants_Data::FilesNeeded
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
void ADN_Inhabitants_Data::FilesNeeded( tools::Path::T_Paths& vFiles ) const
{
    vFiles.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szPeople_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Inhabitants_Data::ReadArchive
// Created: SLG 2010-11-22
// -----------------------------------------------------------------------------
void ADN_Inhabitants_Data::ReadArchive( xml::xistream& input )
{
    input >> xml::start( "populations" )
              >> xml::list( "population", *this, &ADN_Inhabitants_Data::ReadPeople )
          >> xml::end;
    vInhabitants_.CheckValidity();
}

// -----------------------------------------------------------------------------
// Name: ADN_Inhabitants_Data::ReadPeople
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
void ADN_Inhabitants_Data::ReadPeople( xml::xistream& input )
{
    std::unique_ptr< InhabitantsInfos > spNew( new InhabitantsInfos( input.attribute< unsigned int >( "id" ) ) );
    spNew->ReadArchive( input );
    vInhabitants_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Inhabitants_Data::WriteArchive
// Created: SLG 2010-11-22
// -----------------------------------------------------------------------------
void ADN_Inhabitants_Data::WriteArchive( xml::xostream& output ) const
{
    if( vInhabitants_.GetErrorStatus() == eError )
        throw MASA_EXCEPTION( GetInvalidDataErrorMsg() );

    output << xml::start( "populations" );
    tools::SchemaWriter schemaWriter;
    schemaWriter.WritePhysicalSchema( output, "Inhabitants" );
    for( auto it = vInhabitants_.begin(); it != vInhabitants_.end(); ++it )
        ( *it )->WriteArchive( output );
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Inhabitants_Data::GetInhabitantsThatUse
// Created: ABR 2012-08-01
// -----------------------------------------------------------------------------
QStringList ADN_Inhabitants_Data::GetInhabitantsThatUse( ADN_Crowds_Data::CrowdsInfos& population )
{
    QStringList result;
    for( auto it = vInhabitants_.begin(); it != vInhabitants_.end(); ++it )
        if( ( *it )->ptrModel_.GetData() && ( *it )->ptrModel_.GetData()->strName_.GetData() == population.strName_.GetData() )
            result << ( *it )->strName_.GetData().c_str();
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Inhabitants_Data::GetInhabitantsThatUse
// Created: ABR 2012-08-02
// -----------------------------------------------------------------------------
QStringList ADN_Inhabitants_Data::GetInhabitantsThatUse( ADN_ResourceNetworks_Data::ResourceNetworkInfos& network )
{
    QStringList result;
    for( auto it = vInhabitants_.begin(); it != vInhabitants_.end(); ++it )
        for( auto itConsumption = ( *it )->consumptions_.begin(); itConsumption != ( *it )->consumptions_.end(); ++itConsumption )
            if( ( *itConsumption )->GetCrossedElement() && ( *itConsumption )->GetCrossedElement()->strName_.GetData() == network.strName_.GetData() )
                result << ( *it )->strName_.GetData().c_str();
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Inhabitants_Data::GetSchedulesThatUse
// Created: JSR 2013-04-15
// -----------------------------------------------------------------------------
QStringList ADN_Inhabitants_Data::GetSchedulesThatUse( ADN_Urban_Data::AccommodationInfos& accomodation )
{
    QStringList result;
    for( auto it = vInhabitants_.begin(); it != vInhabitants_.end(); ++it )
        for( auto itSchedule = ( *it )->schedule_.begin(); itSchedule!= ( *it )->schedule_.end(); ++itSchedule )
            if( ( *itSchedule )->ptrAccommodation_.GetData() && ( *itSchedule )->ptrAccommodation_.GetData()->strName_.GetData() == accomodation.strName_.GetData() )
                if( !result.contains( ( *it )->strName_.GetData().c_str() ) )
                    result << ( *it )->strName_.GetData().c_str();
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Inhabitants_Data::CheckDatabaseValidity
// Created: JSR 2013-04-11
// -----------------------------------------------------------------------------
void ADN_Inhabitants_Data::CheckDatabaseValidity( ADN_ConsistencyChecker& checker ) const
{
    ADN_Data_ABC::CheckDatabaseValidity( checker );
    for( auto it = vInhabitants_.begin(); it != vInhabitants_.end(); ++it )
        ( *it )->CheckDatabaseValidity( checker, ( *it )->strName_.GetData(), eInhabitants );
}
