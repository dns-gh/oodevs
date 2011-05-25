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

// -----------------------------------------------------------------------------
// Name: EventInfos::EventInfos
// Created: LGY 2011-01-18
// -----------------------------------------------------------------------------
ADN_People_Data::EventInfos::EventInfos()
    : ADN_Ref_ABC()
    , ADN_DataTreeNode_ABC()
{
    // NOTHING
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
// Name: EventInfos::GetNodeName
// Created: LGY 2011-01-18
// -----------------------------------------------------------------------------
std::string ADN_People_Data::EventInfos::GetNodeName()
{
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: EventInfos::GetItemName
// Created: LGY 2011-01-18
// -----------------------------------------------------------------------------
std::string ADN_People_Data::EventInfos::GetItemName()
{
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: EventInfos::CreateCopy
// Created: LGY 2011-01-18
// -----------------------------------------------------------------------------
ADN_People_Data::EventInfos* ADN_People_Data::EventInfos::CreateCopy()
{
    return 0;
}

// -----------------------------------------------------------------------------
// Name: EventInfos::ReadArchive
// Created: LGY 2011-01-18
// -----------------------------------------------------------------------------
void ADN_People_Data::EventInfos::ReadArchive( xml::xistream& input )
{
    day_ = input.attribute< std::string >( "day" );
    from_ = input.attribute< std::string >( "from" );
    to_ = input.attribute< std::string >( "to" );
    motivation_ = input.attribute< std::string >( "motivation" );
}

// -----------------------------------------------------------------------------
// Name: EventInfos::WriteArchive
// Created: LGY 2011-01-18
// -----------------------------------------------------------------------------
void ADN_People_Data::EventInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "event" )
           << xml::attribute( "day", day_ )
           << xml::attribute( "from", from_ )
           << xml::attribute( "to", to_ )
           << xml::attribute( "motivation", motivation_ )
           << xml::end;
}

// -----------------------------------------------------------------------------
// Name: PeopleInfosConsumption::PeopleInfosConsumption
// Created: JSR 2011-01-31
// -----------------------------------------------------------------------------
ADN_People_Data::PeopleInfosConsumption::PeopleInfosConsumption()
    : ADN_Ref_ABC()
    , ADN_DataTreeNode_ABC()
    , ptrResource_( ADN_Workspace::GetWorkspace().GetResourceNetworks().GetData().GetResourceNetworksInfos(), 0 )
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
    return ptrResource_.GetData()->strName_.GetData().c_str();
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
        throw ADN_DataException( tools::translate( "People_Data", "Invalid data" ).ascii(), tools::translate( "People_Data", "Population - Invalid resource '%1/%2'" ).arg( strResource.c_str() ).ascii() );
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
    : ADN_Ref_ABC()
    , ADN_DataTreeNode_ABC()
    , ptrModel_           ( ADN_Workspace::GetWorkspace().GetPopulation().GetData().GetPopulation(), 0 )
    , repartition_        ( tools::translate( "People_Data", "Population" ) )
    , securityLossOnFire_ ( 0 )
    , securityGainPerHour_( 0 )
    , healthNeed_         ( 0 )
    , transferTime_       ( "0h" )
{
    BindExistenceTo( &ptrModel_ );
    consumptions_.SetParentNode( *this );
    consumptions_.SetItemTypeName( "une consommation" );
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
// Name: PeopleInfos::GetNodeName
// Created: SLG 2010-11-22
// -----------------------------------------------------------------------------
std::string ADN_People_Data::PeopleInfos::GetNodeName()
{
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: PeopleInfos::GetItemName
// Created: SLG 2010-11-22
// -----------------------------------------------------------------------------
std::string ADN_People_Data::PeopleInfos::GetItemName()
{
    return std::string();
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
    for( IT_Events it = schedule_.begin(); it != schedule_.end(); ++it )
    {
        pCopy->schedule_[ it->first ].reset( new EventInfos() );
        pCopy->schedule_[ it->first ]->day_ = it->second->day_.GetData();
        pCopy->schedule_[ it->first ]->from_ = it->second->from_.GetData();
        pCopy->schedule_[ it->first ]->to_ = it->second->to_.GetData();
        pCopy->schedule_[ it->first ]->motivation_ = it->second->motivation_.GetData();
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
    int index = 0;
    std::string strModel;
    input >> xml::attribute( "name", strName_ )
          >> xml::attribute( "associated-crowd", strModel )
          >> xml::optional >> xml::attribute( "angry-crowd-mission", strAngryCrowdMission_ );

    ADN_Population_Data::PopulationInfos* pModel = ADN_Workspace::GetWorkspace().GetPopulation().GetData().FindPopulation( strModel );
    if( !pModel )
        throw ADN_DataException( "Invalid data", tools::translate( "Population_Data", "Population types - Invalid Population type '%1'" ).arg( strModel.c_str() ).ascii() );
    ptrModel_ = pModel;

    input >> xml::start( "repartition" );
    repartition_.ReadArchive( input );
    input >> xml::end
          >> xml::start( "schedule" )
            >> xml::attribute( "transfer-time", transferTime_ )
            >> xml::list( "event", *this, &ADN_People_Data::PeopleInfos::ReadEvent, index )
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
    for( CIT_Events it1 = schedule_.begin(); it1 != schedule_.end(); ++it1 )
        for( CIT_Events it2 = schedule_.begin(); it2 != schedule_.end(); ++it2 )
            if( it1->first != it2->first && it1->second->day_.GetData() == it2->second->day_.GetData() &&
                !CheckTime( it1->second->from_.GetData(), it1->second->to_.GetData(), it2->second->from_.GetData(), it2->second->to_.GetData() ) )
                    return tools::translate( "People_Data", "Invalid schedule - You have already an appointment on the same moment :" ).ascii() + std::string( "\n" ) + "- " + it1->second->day_.GetData() + " : " + it1->second->from_.GetData() + " / " + it1->second->to_.GetData() + "\n" +
                           "- " + it2->second->day_.GetData() + " : " + it2->second->from_.GetData() + " / " + it2->second->to_.GetData() + "\n";
    repartition_.CheckNoError( "ADN_People_Data" );
    return "";
}

// -----------------------------------------------------------------------------
// Name: PeopleInfos::WriteArchive
// Created: SLG 2010-11-22
// -----------------------------------------------------------------------------
void ADN_People_Data::PeopleInfos::WriteArchive( xml::xostream& output, int mosId ) const
{
    const std::string error = CheckErrors();
    if( error != "" )
        throw ADN_DataException( tools::translate( "Categories_Data", "Invalid data" ).ascii(), error );
    output << xml::start( "population" )
            << xml::attribute( "name", strName_ )
            << xml::attribute( "id", mosId )
            << xml::attribute( "associated-crowd", ptrModel_.GetData()->strName_ );
    if( !strAngryCrowdMission_.GetData().empty() )
        output << xml::attribute( "angry-crowd-mission", strAngryCrowdMission_ );
    output << xml::start( "repartition" );
    repartition_.WriteArchive( output );
    output  << xml::end
            << xml::start( "schedule" )
                << xml::attribute( "transfer-time", transferTime_ );
    for( CIT_Events it = schedule_.begin(); it != schedule_.end(); ++it )
        it->second->WriteArchive( output );
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
void ADN_People_Data::PeopleInfos::ReadEvent( xml::xistream& input, int& index )
{
    schedule_[ index ].reset( new EventInfos() );
    schedule_[ index ]->ReadArchive( input );
    index++;
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
    std::auto_ptr< PeopleInfos > spNew( new PeopleInfos() );
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
    int n = 0;
    for( CIT_PeopleInfosVector it = vPeople_.begin(); it != vPeople_.end(); ++it, ++n )
        ( *it )->WriteArchive( output, n );
    output << xml::end;
}
