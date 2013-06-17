// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Activities_Data.h"
#include "ADN_DataException.h"
#include "ADN_Missions_Data.h"
#include "ADN_Project_Data.h"
#include "ADN_Tr.h"
#include "ADN_Tools.h"
#include <boost/bind.hpp>

// -----------------------------------------------------------------------------
// Package
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: ADN_Activities_Data::Package::Package
// Created: NPT 2013-04-16
// -----------------------------------------------------------------------------
ADN_Activities_Data::PackageInfos::PackageInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Activities_Data::Package::Package
// Created: NPT 2013-04-16
// -----------------------------------------------------------------------------
ADN_Activities_Data::PackageInfos::PackageInfos( const std::string name )
    : strName_( name )
{
    // NOTHING
}
// -----------------------------------------------------------------------------
// Name: ADN_Activities_Data::Package::~Package
// Created: NPT 2013-04-16
// -----------------------------------------------------------------------------
ADN_Activities_Data::PackageInfos::~PackageInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Activities_Data::Package::GetItemName
// Created: NPT 2013-04-16
// -----------------------------------------------------------------------------
std::string ADN_Activities_Data::PackageInfos::GetItemName()
{
    return strName_.GetData();
}
// -----------------------------------------------------------------------------
// Name: ADN_Activities_Data::Package::CreateCopy
// Created: NPT 2013-04-16
// -----------------------------------------------------------------------------
ADN_Activities_Data::PackageInfos* ADN_Activities_Data::PackageInfos::CreateCopy()
{
    return new PackageInfos( strName_.GetData() );
}
// -----------------------------------------------------------------------------
// Name: ADN_Activities_Data::Package::WriteArchive
// Created: NPT 2013-04-16
// -----------------------------------------------------------------------------
void ADN_Activities_Data::PackageInfos::WriteArchive( xml::xostream& output )
{
    if( strName_ == " - " )
        return;
    output  << xml::start( "package" )
                << xml::attribute( "name", strName_ )
            << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Activities_Data::Package::ReadArchive
// Created: NPT 2013-04-16
// -----------------------------------------------------------------------------
void ADN_Activities_Data::PackageInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "name", strName_ );
}

// -----------------------------------------------------------------------------
// Activity
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: ADN_Activities_Data::Activity
// Created: NPT 2013-04-16
// -----------------------------------------------------------------------------
ADN_Activities_Data::ActivityInfos::ActivityInfos()
    : id_( 0 )
    , strName_( "" )
    , description_( "" )
    , meleeActivity_( false )
    , package_( ADN_Workspace::GetWorkspace().GetMissions().GetData().activitiesData_->GetPackages(), 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Activities_Data::Activity::Activity
// Created: NPT 2013-04-16
// -----------------------------------------------------------------------------
ADN_Activities_Data::ActivityInfos::ActivityInfos( unsigned int id, const std::string& name, const std::string& description, bool meleeActivity )
    : id_( id )
    , strName_( name )
    , description_( description )
    , meleeActivity_( meleeActivity )
    , package_( ADN_Workspace::GetWorkspace().GetMissions().GetData().activitiesData_->GetPackages(), 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Activities_Data::Activity::~Activity
// Created: NPT 2013-04-16
// -----------------------------------------------------------------------------
ADN_Activities_Data::ActivityInfos::~ActivityInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Activities_Data::Activity::GetItemName
// Created: NPT 2013-04-16
// -----------------------------------------------------------------------------
std::string ADN_Activities_Data::ActivityInfos::GetItemName()
{
    return strName_.GetData();
}
// -----------------------------------------------------------------------------
// Name: ADN_Activities_Data::Activity::CreateCopy
// Created: NPT 2013-04-16
// -----------------------------------------------------------------------------
ADN_Activities_Data::ActivityInfos* ADN_Activities_Data::ActivityInfos::CreateCopy()
{
    ADN_Activities_Data::ActivityInfos* activity = new ADN_Activities_Data::ActivityInfos( id_.GetData(), strName_.GetData(), description_.GetData(), meleeActivity_.GetData() );
    activity->package_ = package_.GetData();
    return activity;
}

// -----------------------------------------------------------------------------
// Name: ADN_Activities_Data::Activity::WriteArchive
// Created: NPT 2013-04-16
// -----------------------------------------------------------------------------
void ADN_Activities_Data::ActivityInfos::WriteArchive( xml::xostream& output )
{
    output  << xml::start( "activity" )
                << xml::attribute( "id", id_ )
                << xml::attribute( "name", strName_ )
                << xml::attribute( "doc", description_ )
                << xml::attribute( "melee", meleeActivity_ );
    if( package_.GetData()->strName_.GetData() == " - " )
        output << xml::attribute( "package", "none" );
    else
        output << xml::attribute( "package", package_.GetData()->strName_.GetData() );
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Activities_Data::Activity::ReadArchive
// Created: NPT 2013-04-16
// -----------------------------------------------------------------------------
void ADN_Activities_Data::ActivityInfos::ReadArchive( xml::xistream& input )
{
    std::string package;
    input  >> xml::attribute( "id", id_ )
           >> xml::attribute( "name", strName_ )
           >> xml::attribute( "doc", description_ )
           >> xml::attribute( "melee", meleeActivity_ )
           >> xml::attribute( "package", package );
    ADN_Activities_Data::PackageInfos* pPackage;
    if( package != "none" )
        pPackage = ADN_Workspace::GetWorkspace().GetMissions().GetData().activitiesData_->FindPackage( package );
    else
        pPackage = new ADN_Activities_Data::PackageInfos( " - " );
    if( !pPackage )
        throw ADN_DataException( tools::translate( "Activities_Data", "Invalid data" ).toAscii().constData(), tools::translate( "Activities_Data", "Activity '%1' - Invalid package type" ).arg( package.c_str() ).toAscii().constData() );
    package_ = pPackage;
}

// -----------------------------------------------------------------------------
// Activities data
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: ADN_Activities_Data constructor
// Created: NPT 2013-04-16
// -----------------------------------------------------------------------------
ADN_Activities_Data::ADN_Activities_Data()
{
     packages_.AddItem( new PackageInfos( " - " ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Activities_Data destructor
// Created: NPT 2013-04-16
// -----------------------------------------------------------------------------
ADN_Activities_Data::~ADN_Activities_Data()
{
        // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Activities_Data::Reset
// Created: NPT 2013-04-16
// -----------------------------------------------------------------------------
void ADN_Activities_Data::Reset()
{
    activities_.Reset();
    packages_.Reset();
}


// -----------------------------------------------------------------------------
// Name: ADN_Activities_Data::WriteArchive
// Created: NPT 2013-04-16
// -----------------------------------------------------------------------------
void ADN_Activities_Data::WriteArchive( xml::xostream& output )
{
    EnsureActivityActivitesIdSingle();
    output  << xml::start( "package-activity" );
    ADN_Tools::AddSchema( output, "Activity" );
    output      << xml::start( "activities" );
                    for( auto it = activities_.begin(); it != activities_.end(); ++it )
                        (*it)->WriteArchive( output );
    output      << xml::end
                << xml::start( "packages" );
                    for( auto it = packages_.begin(); it != packages_.end(); ++it )
                        (*it)->WriteArchive( output );
    output      << xml::end
            << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Activities_Data::ReadArchive
// Created: NPT 2013-04-16
// -----------------------------------------------------------------------------
void ADN_Activities_Data::ReadArchive( xml::xistream& input )
{
    input  >> xml::start( "package-activity" )
               >> xml::start( "packages" )
                   >> xml::list( "package", *this, &ADN_Activities_Data::ReadPackage )
               >> xml::end
               >> xml::start( "activities" )
                    >> xml::list( "activity", *this, &ADN_Activities_Data::ReadActivity )
               >> xml::end
           >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Activities_Data::ReadActivity
// Created: NPT 2013-04-16
// -----------------------------------------------------------------------------
void ADN_Activities_Data::ReadActivity( xml::xistream& input )
{
    ActivityInfos* newActivity = new ActivityInfos();
    newActivity->ReadArchive( input );
    activities_.AddItem( newActivity );
}
// -----------------------------------------------------------------------------
// Name: ADN_Activities_Data::ReadPackage
// Created: NPT 2013-04-16
// -----------------------------------------------------------------------------
void ADN_Activities_Data::ReadPackage( xml::xistream& input )
{
    PackageInfos* newPackage = new PackageInfos();
    newPackage->ReadArchive( input );
    packages_.AddItem( newPackage );
}

// -----------------------------------------------------------------------------
// Name: ADN_Activities_Data::GetActivities
// Created: NPT 2013-04-16
// -----------------------------------------------------------------------------
ADN_Activities_Data::T_Activity_Vector& ADN_Activities_Data::GetActivities()
{
    return activities_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Activities_Data::GetPackages
// Created: NPT 2013-04-16
// -----------------------------------------------------------------------------
ADN_Activities_Data::T_Package_Vector& ADN_Activities_Data::GetPackages()
{
    return packages_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Activities_Data::FilesNeeded
// Created: NPT 2013-04-16
// -----------------------------------------------------------------------------
void ADN_Activities_Data::FilesNeeded( T_StringList& vFiles ) const
{
    vFiles.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szActivities_.GetData() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Activities_Data::FindPackage
// Created: NPT 2013-04-17
// -----------------------------------------------------------------------------
ADN_Activities_Data::PackageInfos* ADN_Activities_Data::FindPackage( const std::string& strName )
{
    auto it = std::find_if( packages_.begin(), packages_.end(), ADN_Tools::NameCmp<ADN_Activities_Data::PackageInfos>( strName ) );
    if( it == packages_.end() )
        return 0;
    return *it;
}

// -----------------------------------------------------------------------------
// Name: ADN_Missions_Data::GetUnitThatUse
// Created: NPT 2013-04-19
// -----------------------------------------------------------------------------
QStringList ADN_Activities_Data::GetActivitiesThatUse( ADN_Activities_Data::PackageInfos& package )
{
    QStringList result;
    for( auto it = activities_.begin(); it != activities_.end(); ++it )
    {
        ActivityInfos* activity = *it;
        if( activity->package_.GetData()->strName_.GetData() == package.strName_.GetData() )
            result << activity->strName_.GetData().c_str();
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Activities_Data::SetActivityActivitesIdsUnique
// Created: NPT 2013-04-25
// -----------------------------------------------------------------------------
void ADN_Activities_Data::EnsureActivityActivitesIdSingle()
{
    std::vector< int > ids;
    for( auto it = activities_.begin(); it != activities_.end(); ++it )
    {
        while( std::find( ids.begin(), ids.end(), ( *it )->id_.GetData() ) != ids.end() || ( *it )->id_.GetData() == 0 )
            ( *it )->id_ = ( *it )->id_.GetData() + 1;
        ids.push_back( ( *it )->id_.GetData() );
    }
}
