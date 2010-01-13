// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_ActiveProtections_Data.h"

#include "ADN_Workspace.h"
#include "ADN_Project_Data.h"
#include "ADN_DataException.h"
#include "ADN_OpenFile_Exception.h"
#include "ADN_SaveFile_Exception.h"
#include "ADN_Tools.h"

// -----------------------------------------------------------------------------
// Name: ADN_ActiveProtections_Data constructor
// Created: LDC 2010-01-13
// -----------------------------------------------------------------------------
ADN_ActiveProtections_Data::ADN_ActiveProtections_Data()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_ActiveProtections_Data destructor
// Created: LDC 2010-01-13
// -----------------------------------------------------------------------------
ADN_ActiveProtections_Data::~ADN_ActiveProtections_Data()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_ActiveProtections_Data::FilesNeeded
// Created: LDC 2010-01-13
// -----------------------------------------------------------------------------
void ADN_ActiveProtections_Data::FilesNeeded( T_StringList& vFiles ) const
{
    vFiles.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szActiveProtections_.GetData() );
}
    
// -----------------------------------------------------------------------------
// Name: ADN_ActiveProtections_Data::Reset
// Created: LDC 2010-01-13
// -----------------------------------------------------------------------------
void ADN_ActiveProtections_Data::Reset()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_ActiveProtections_Data::ReadArchive
// Created: LDC 2010-01-13
// -----------------------------------------------------------------------------
void ADN_ActiveProtections_Data::ReadArchive( xml::xistream& xis )
{
    xis >> xml::start( "protections" )
        >> xml::list( "protection", *this, &ADN_ActiveProtections_Data::ReadProtection );
}

// -----------------------------------------------------------------------------
// Name: ADN_ActiveProtections_Data::ReadProtection
// Created: LDC 2010-01-13
// -----------------------------------------------------------------------------
void ADN_ActiveProtections_Data::ReadProtection( xml::xistream& xis )
{
    Protection protection( xis );
    protections_.push_back( protection );
}

// -----------------------------------------------------------------------------
// Name: ADN_ActiveProtections_Data::WriteArchive
// Created: LDC 2010-01-13
// -----------------------------------------------------------------------------
void ADN_ActiveProtections_Data::WriteArchive( xml::xostream& xos )
{
    xos << xml::start( "protections" )
            << xml::attribute( "xmlns:xsi","http://www.w3.org/2001/XMLSchema-instance" )
            << xml::attribute( "xsi:noNamespaceSchemaLocation", "schemas/physical/ActiveProtections.xsd" );
    for( std::vector< ADN_ActiveProtections_Data::Protection >::const_iterator it = protections_.begin(); it != protections_.end(); ++it )
        it->WriteArchive( xos );
    xos << xml::end();
}

// -----------------------------------------------------------------------------
// Name: ADN_ActiveProtections_Data::Protection
// Created: LDC 2010-01-13
// -----------------------------------------------------------------------------
ADN_ActiveProtections_Data::Protection::Protection( xml::xistream& xis )
{
    xis >> xml::attribute( "name", name_ )
        >> xml::attribute( "coefficient", coefficient_ )
        >> xml::attribute( "hard-kill", hardKill_ )
        >> xml::optional() >> xml::start( "dotation" )
            >> xml::attribute( "name", strDotationName_ )
            >> xml::attribute( "usage", usage_ )
        >> xml::end()
        >> xml::list( "weapon", *this, &ADN_ActiveProtections_Data::Protection::ReadWeapon );
}

// -----------------------------------------------------------------------------
// Name: ADN_ActiveProtections_Data::Protection::ReadWeapon
// Created: LDC 2010-01-13
// -----------------------------------------------------------------------------
void ADN_ActiveProtections_Data::Protection::ReadWeapon( xml::xistream& xis )
{
    std::string name;
    double value;
    xis >> xml::attribute( "name", name ) >> xml::attribute( "coefficient", value );
    weapons_.push_back( std::pair< std::string, double >( name, value ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_ActiveProtections_Data::Protection::Write
// Created: LDC 2010-01-13
// -----------------------------------------------------------------------------
void ADN_ActiveProtections_Data::Protection::WriteArchive( xml::xostream& xos ) const
{
    xos << xml::start( "protection" )
        << xml::attribute( "name", name_ )
        << xml::attribute( "coefficient", coefficient_ )
        << xml::attribute( "hard-kill", hardKill_ );
    if( !strDotationName_.empty() )
        xos << xml::start( "dotation" )
                << xml::attribute( "name", strDotationName_ )
                << xml::attribute( "usage", usage_ )
            << xml::end();
    for( std::vector< std::pair<std::string, double> >::const_iterator it = weapons_.begin(); it != weapons_.end(); ++it )
        xos << xml::start( "weapon" )
                << xml::attribute( "name", it->first )
                << xml::attribute( "coefficient", it->second )
            << xml::end();
    xos << xml::end();
}
