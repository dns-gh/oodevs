// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Languages_Data.h"
#include "ADN_GeneralConfig.h"
#include "ADN_Tools.h"
#include "ADN_Project_Data.h"
#include "clients_kernel/Language.h"
#include "clients_kernel/Languages.h"

// -----------------------------------------------------------------------------
// Name: ADN_Languages_Data constructor
// Created: ABR 2013-07-08
// -----------------------------------------------------------------------------
ADN_Languages_Data::ADN_Languages_Data()
    : ADN_Data_ABC( eLanguages )
    , allLanguages_( new kernel::Languages( ADN_Workspace::GetWorkspace().GetConfig().BuildResourceChildFile( "languages.xml" ) ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_Data destructor
// Created: ABR 2013-07-08
// -----------------------------------------------------------------------------
ADN_Languages_Data::~ADN_Languages_Data()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_Data::FilesNeeded
// Created: ABR 2013-07-08
// -----------------------------------------------------------------------------
void ADN_Languages_Data::FilesNeeded( tools::Path::T_Paths& files ) const
{
    files.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szLanguages_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_Data::ReadArchive
// Created: ABR 2013-07-08
// -----------------------------------------------------------------------------
void ADN_Languages_Data::ReadArchive( xml::xistream& input )
{
    input >> xml::start( "languages" )
            >> xml::list( "language", *this, &ADN_Languages_Data::ReadLanguage )
          >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_Data::ReadLanguage
// Created: ABR 2013-07-08
// -----------------------------------------------------------------------------
void ADN_Languages_Data::ReadLanguage( xml::xistream& input )
{
    const boost::shared_ptr< kernel::Language >& language = allLanguages_->Get( input.attribute( "code", "" ) );
    if( input.attribute( "master", false ) )
        InternalSetMaster( language->GetCode() );
    else
        activeLanguages_.push_back( language );
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_Data::WriteArchive
// Created: ABR 2013-07-08
// -----------------------------------------------------------------------------
void ADN_Languages_Data::WriteArchive( xml::xostream& output )
{
    output << xml::start( "languages" );
    tools::SchemaWriter schemaWriter;
    schemaWriter.WritePhysicalSchema( output, "Languages" );

    if( !master_.empty() )
        output << xml::start( "language" )
                 << xml::attribute( "code", master_ )
                 << xml::attribute( "master", true )
               << xml::end;
    for( auto it = activeLanguages_.begin(); it != activeLanguages_.end(); ++it )
        output << xml::start( "language" )
                 << xml::attribute( "code", ( *it )->GetCode() )
               << xml::end;
    output << xml::end; //! languages
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_Data::Master
// Created: ABR 2013-08-29
// -----------------------------------------------------------------------------
const std::string& ADN_Languages_Data::Master()
{
    return master_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_Data::SetMaster
// Created: ABR 2013-10-01
// -----------------------------------------------------------------------------
void ADN_Languages_Data::SetMaster( const std::string& language )
{
    if( master_ == language )
        throw MASA_EXCEPTION( "Can't set master language with the actual master language" );

    for( auto it = activeLanguages_.begin(); it != activeLanguages_.end(); ++it )
        if( ( *it )->GetCode() == language )
        {
            activeLanguages_.erase( it );
            break;
        }
    if( !master_.empty() )
        activeLanguages_.push_back( allLanguages_->Get( master_ ) );
    InternalSetMaster( language );
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_Data::InternalSetMaster
// Created: ABR 2013-10-03
// -----------------------------------------------------------------------------
void ADN_Languages_Data::InternalSetMaster( const std::string& language )
{
    if( IsCurrentMaster() )
        kernel::Language::SetCurrent( language );
    master_ = language;
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_Data::IsMaster
// Created: ABR 2013-08-22
// -----------------------------------------------------------------------------
bool ADN_Languages_Data::IsMaster( const std::string& language )
{
    return language == master_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_Data::IsCurrentMaster
// Created: ABR 2013-08-21
// -----------------------------------------------------------------------------
bool ADN_Languages_Data::IsCurrentMaster()
{
    return IsMaster( kernel::Language::Current() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_Data::GetAllLanguages
// Created: ABR 2013-10-07
// -----------------------------------------------------------------------------
const kernel::Languages& ADN_Languages_Data::GetAllLanguages() const
{
    return *allLanguages_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Languages_Data::GetActiveLanguages
// Created: ABR 2013-10-07
// -----------------------------------------------------------------------------
const kernel::Languages::T_Languages& ADN_Languages_Data::GetActiveLanguages() const
{
    return activeLanguages_;
}
