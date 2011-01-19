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

// -----------------------------------------------------------------------------
// Name: ADN_People_Data::EventInfos
// Created: LGY 2011-01-18
// -----------------------------------------------------------------------------
ADN_People_Data::EventInfos::EventInfos()
    : ADN_Ref_ABC()
    , ADN_DataTreeNode_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_People_Data::~EventInfos
// Created: LGY 2011-01-18
// -----------------------------------------------------------------------------
ADN_People_Data::EventInfos::~EventInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_People_Data::GetNodeName
// Created: LGY 2011-01-18
// -----------------------------------------------------------------------------
std::string ADN_People_Data::EventInfos::GetNodeName()
{
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: ADN_People_Data::GetItemName
// Created: LGY 2011-01-18
// -----------------------------------------------------------------------------
std::string ADN_People_Data::EventInfos::GetItemName()
{
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: ADN_People_Data::CreateCopy
// Created: LGY 2011-01-18
// -----------------------------------------------------------------------------
ADN_People_Data::EventInfos* ADN_People_Data::EventInfos::CreateCopy()
{
    return 0;
}

// -----------------------------------------------------------------------------
// Name: ADN_People_Data::ReadArchive
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
// Name: ADN_People_Data::WriteArchive
// Created: LGY 2011-01-18
// -----------------------------------------------------------------------------
void ADN_People_Data::EventInfos::WriteArchive( xml::xostream& output )
{
    output << xml::start( "event" )
           << xml::attribute( "day", day_ )
           << xml::attribute( "from", from_ )
           << xml::attribute( "to", to_ )
           << xml::attribute( "motivation", motivation_ )
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
    , male_               ( 0 )
    , female_             ( 0 )
    , children_           ( 0 )
    , securityLossOnFire_ ( 0 )
    , securityGainPerHour_( 0 )
    , transferTime_       ( "0h" )
{
    BindExistenceTo( &ptrModel_ );
}

// -----------------------------------------------------------------------------
// Name: PeopleInfos::~PeopleInfos
// Created: SLG 2010-11-22
// -----------------------------------------------------------------------------
ADN_People_Data::PeopleInfos::~PeopleInfos()
{
    // NOTHING
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
    pCopy->male_ = male_.GetData();
    pCopy->female_ = female_.GetData();
    pCopy->children_ = children_.GetData();
    pCopy->transferTime_ = transferTime_.GetData();
    pCopy->securityLossOnFire_ = securityLossOnFire_.GetData();
    pCopy->securityGainPerHour_ = securityGainPerHour_.GetData();
    for( IT_Events it = schedule_.begin(); it != schedule_.end(); ++it )
    {
        pCopy->schedule_[ it->first ].reset( new EventInfos() );
        pCopy->schedule_[ it->first ]->day_ = it->second->day_.GetData();
        pCopy->schedule_[ it->first ]->from_ = it->second->from_.GetData();
        pCopy->schedule_[ it->first ]->to_ = it->second->to_.GetData();
        pCopy->schedule_[ it->first ]->motivation_ = it->second->motivation_.GetData();
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
          >> xml::attribute( "associated-crowd", strModel );

    ADN_Population_Data::PopulationInfos* pModel = ADN_Workspace::GetWorkspace().GetPopulation().GetData().FindPopulation( strModel );
    if( !pModel )
        throw ADN_DataException( "Invalid data", tools::translate( "Population_Data", "Population types - Invalid Population type '%1'" ).arg( strModel.c_str() ).ascii() );
    ptrModel_ = pModel;

    input >> xml::start( "repartition" )
            >> xml::attribute( "male", male_ )
            >> xml::attribute( "female", female_ )
            >> xml::attribute( "children", children_ )
          >> xml::end
          >> xml::start( "schedule" )
            >> xml::attribute( "transfer-time", transferTime_ )
            >> xml::list( "event", *this, &ADN_People_Data::PeopleInfos::ReadEvent, index )
          >> xml::end
          >> xml::start( "security-level" )
            >> xml::attribute( "loss-on-fire", securityLossOnFire_ )
            >> xml::attribute( "gain-per-hour", securityGainPerHour_ )
          >> xml::end;
    securityLossOnFire_ = 100 * securityLossOnFire_.GetData();
    securityGainPerHour_ = 100 * securityGainPerHour_.GetData();
}

// -----------------------------------------------------------------------------
// Name: PeopleInfos::WriteArchive
// Created: SLG 2010-11-22
// -----------------------------------------------------------------------------
void ADN_People_Data::PeopleInfos::WriteArchive( xml::xostream& output, int mosId )
{
    output << xml::start( "population" )
            << xml::attribute( "name", strName_ )
            << xml::attribute( "id", mosId )
            << xml::attribute( "associated-crowd", ptrModel_.GetData()->strName_ )
            << xml::start( "repartition" )
                << xml::attribute( "male", male_ )
                << xml::attribute( "female", female_ )
                << xml::attribute( "children", children_ )
            << xml::end
            << xml::start( "schedule" )
                << xml::attribute( "transfer-time", transferTime_ );
    for( IT_Events it = schedule_.begin(); it != schedule_.end(); ++it )
        it->second->WriteArchive( output );
    output  << xml::end
          << xml::start( "security-level" )
            << xml::attribute( "loss-on-fire", securityLossOnFire_.GetData() / 100.0 )
            << xml::attribute( "gain-per-hour", securityGainPerHour_.GetData() / 100.0 )
          << xml::end
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
    for( IT_PeopleInfosVector it = vPeople_.begin(); it != vPeople_.end(); ++it, ++n )
        ( *it )->WriteArchive( output, n );
    output << xml::end;
}
