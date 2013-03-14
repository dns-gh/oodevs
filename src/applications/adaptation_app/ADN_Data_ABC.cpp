// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Data_ABC.h"
#include "ADN_Project_Data.h"
#include "ADN_Tools.h"
#include "ADN_Tr.h"
#include "tools/Loader_ABC.h"
#include <boost/bind.hpp>
#pragma warning( push, 0 )
#include <QtGui/QApplication>
#pragma warning( pop )

QString ADN_Data_ABC::duplicateName_ = QT_TRANSLATE_NOOP( "ADN_Data_ABC", "Duplicate name" );
QString ADN_Data_ABC::invalidDataOntab_   = QT_TRANSLATE_NOOP( "ADN_Data_ABC", "Invalid data on tab '%1'" );

// -----------------------------------------------------------------------------
// Name: ADN_Data_ABC constructor
// Created: APE 2005-03-17
// -----------------------------------------------------------------------------
ADN_Data_ABC::ADN_Data_ABC( E_WorkspaceElements currentTab, int subTab /* = -1 */ )
    : QObject()
    , currentTab_( currentTab )
    , subTab_( subTab )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Data_ABC destructor
// Created: APE 2005-03-17
// -----------------------------------------------------------------------------
ADN_Data_ABC::~ADN_Data_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Data_ABC::Load
// Created: APE 2005-03-17
// -----------------------------------------------------------------------------
void ADN_Data_ABC::Load( const tools::Loader_ABC& fileLoader )
{
    tools::Path::T_Paths fileList;
    FilesNeeded( fileList );
    if( !fileList.empty() )
    {
        const tools::Path strFile = ADN_Project_Data::GetWorkDirInfos().GetWorkingDirectory().GetData() / fileList.front();
        fileLoader.LoadFile( strFile, boost::bind( &ADN_Data_ABC::ReadArchive, this, _1 ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Data_ABC::Initialize
// Created: ABR 2011-12-06
// -----------------------------------------------------------------------------
void ADN_Data_ABC::Initialize()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Data_ABC::Save
// Created: APE 2005-03-17
// -----------------------------------------------------------------------------
void ADN_Data_ABC::Save()
{
    tools::Path::T_Paths fileList;
    FilesNeeded( fileList );
    if( !fileList.empty() )
    {
        const tools::Path strFile = ADN_Project_Data::GetWorkDirInfos().GetSaveDirectory() / fileList.front();
        strFile.Parent().CreateDirectories();
        {
            tools::Xofstream output( strFile );
            WriteArchive( output );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Data_ABC::ReadArchive
// Created: APE 2005-03-17
// -----------------------------------------------------------------------------
void ADN_Data_ABC::ReadArchive( xml::xistream& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Data_ABC::WriteArchive
// Created: APE 2005-03-17
// -----------------------------------------------------------------------------
void ADN_Data_ABC::WriteArchive( xml::xostream& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Data_ABC::CheckDatabaseValidity
// Created: PHC 2011-01-19
// -----------------------------------------------------------------------------
void ADN_Data_ABC::CheckDatabaseValidity( ADN_ConsistencyChecker& ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Data_ABC::GetInvalidDataErrorMsg
// Created: ABR 2013-01-16
// -----------------------------------------------------------------------------
std::string ADN_Data_ABC::GetInvalidDataErrorMsg() const
{
    return invalidDataOntab_.arg( ADN_Tr::ConvertFromWorkspaceElement( currentTab_ ).c_str() ).toStdString();
}

// -----------------------------------------------------------------------------
// Name: ADN_Data_ABC::InitTranslations
// Created: ABR 2013-01-16
// -----------------------------------------------------------------------------
void ADN_Data_ABC::InitTranslations()
{
    duplicateName_ = qApp->translate( "ADN_Data_ABC", duplicateName_ );
    invalidDataOntab_ = qApp->translate( "ADN_Data_ABC", invalidDataOntab_ );
}
