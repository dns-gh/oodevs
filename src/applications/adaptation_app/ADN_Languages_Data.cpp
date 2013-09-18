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
#include "ADN_Tools.h"
#include "ADN_Project_Data.h"
#include "clients_kernel/Language.h"

// -----------------------------------------------------------------------------
// Name: ADN_Languages_Data constructor
// Created: ABR 2013-07-08
// -----------------------------------------------------------------------------
ADN_Languages_Data::ADN_Languages_Data()
    : ADN_Data_ABC( eLanguages )
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
    languages_.push_back( kernel::Language( input ) );
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
    for( auto it = languages_.begin(); it != languages_.end(); ++it )
        output << *it;
    output << xml::end; //! languages
}
