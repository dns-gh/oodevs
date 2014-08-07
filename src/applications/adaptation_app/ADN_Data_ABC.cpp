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
#include "ADN_Languages_Data.h"
#include "ADN_Project_Data.h"
#include "ADN_Tools.h"
#include "ADN_Tr.h"
#include "ADN_WorkspaceElement.h"
#include "clients_kernel/XmlTranslations.h"
#include "tools/Language.h"
#include "tools/Loader_ABC.h"
#include <boost/bind.hpp>
#pragma warning( push, 0 )
#include <QtGui/QApplication>
#pragma warning( pop )

#include "ADN_ConsistencyErrorTypes.h"

QString ADN_Data_ABC::duplicateName_ = QT_TRANSLATE_NOOP( "ADN_Data_ABC", "Duplicate name" );
QString ADN_Data_ABC::invalidDataOntab_ = QT_TRANSLATE_NOOP( "ADN_Data_ABC", "Invalid data on tab '%1'" );

namespace
{
    tools::Path BuildLocalDirectory()
    {
        return ADN_Project_Data::GetWorkDirInfos().GetWorkingDirectory().GetData() / ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szLocalesDirectory_;
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Data_ABC constructor
// Created: APE 2005-03-17
// -----------------------------------------------------------------------------
ADN_Data_ABC::ADN_Data_ABC( E_WorkspaceElements currentTab, int subTab /* = -1 */ )
    : QObject()
    , currentTab_( currentTab )
    , subTab_( subTab )
    , translations_( new kernel::XmlTranslations() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Data_ABC destructor
// Created: APE 2005-03-17
// -----------------------------------------------------------------------------
ADN_Data_ABC::~ADN_Data_ABC()
{
    translations_->Purge();
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
        LoadFile( fileLoader, ADN_Project_Data::GetWorkDirInfos().GetWorkingDirectory().GetData() / fileList.front(), boost::bind( &ADN_Data_ABC::ReadArchive, this, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Data_ABC::LoadFile
// Created: ABR 2013-07-10
// -----------------------------------------------------------------------------
void ADN_Data_ABC::LoadFile( const tools::Loader_ABC& fileLoader, const tools::Path& xmlFile, T_Loader loader )
{
    LoadTranslations( xmlFile );
    fileLoader.LoadFile( xmlFile, loader );
}

// -----------------------------------------------------------------------------
// Name: ADN_Data_ABC::LoadTranslations
// Created: ABR 2013-07-10
// -----------------------------------------------------------------------------
void ADN_Data_ABC::LoadTranslations( const tools::Path& xmlFile, kernel::XmlTranslations* translations /*= 0*/ )
{
    kernel::XmlTranslations* currentTranslation = ( translations ) ?  translations : translations_.get();
    if( currentTranslation )
    {
        const tools::LanguagesVector& languages = ADN_Workspace::GetWorkspace().GetLanguages().GetData().GetActiveLanguages();
        for( auto it = languages.cbegin(); it != languages.cend(); ++it )
            currentTranslation->LoadTranslationFile( xmlFile, BuildLocalDirectory(), it->GetCode() );
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
// Name: ADN_Data_ABC::IsActivated
// Created: JSR 2014-03-26
// -----------------------------------------------------------------------------
bool ADN_Data_ABC::IsActivated() const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: ADN_Data_ABC::Save
// Created: APE 2005-03-17
// -----------------------------------------------------------------------------
void ADN_Data_ABC::Save() const
{
    tools::Path::T_Paths fileList;
    FilesNeeded( fileList );
    if( !fileList.empty() )
        SaveFile( ADN_Project_Data::GetWorkDirInfos().GetSaveDirectory() / fileList.front(), boost::bind( &ADN_Data_ABC::WriteArchive, this, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Data_ABC::SaveFile
// Created: ABR 2013-07-10
// -----------------------------------------------------------------------------
void ADN_Data_ABC::SaveFile( const tools::Path& xmlFile, T_Saver saver ) const
{
    xmlFile.Parent().CreateDirectories();
    tools::Xofstream output( xmlFile );
    saver( output );
    SaveTranslations( xmlFile );
}

// -----------------------------------------------------------------------------
// Name: ADN_Data_ABC::SaveTranslations
// Created: ABR 2013-07-10
// -----------------------------------------------------------------------------
void ADN_Data_ABC::SaveTranslations( const tools::Path& xmlFile, kernel::XmlTranslations* translations /*= 0*/ ) const
{
    kernel::XmlTranslations* currentTranslation = ( translations ) ?  translations : translations_.get();
    if( currentTranslation )
        currentTranslation->SaveTranslationFiles( xmlFile, BuildLocalDirectory(), ADN_Workspace::GetWorkspace().GetLanguages().GetData().GetActiveLanguages() );
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
void ADN_Data_ABC::WriteArchive( xml::xostream& ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Data_ABC::CheckDatabaseValidity
// Created: PHC 2011-01-19
// -----------------------------------------------------------------------------
void ADN_Data_ABC::CheckDatabaseValidity( ADN_ConsistencyChecker& checker ) const
{
    if( translations_->HasDuplicateErrors() )
        checker.AddError( eDuplicateTranslation, ADN_Tr::ConvertFromWorkspaceElement( currentTab_ ), currentTab_, subTab_ );
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
// Name: ADN_Data_ABC::InitQtTranslations
// Created: ABR 2013-01-16
// -----------------------------------------------------------------------------
void ADN_Data_ABC::InitQtTranslations()
{
    duplicateName_ = qApp->translate( "ADN_Data_ABC", duplicateName_ );
    invalidDataOntab_ = qApp->translate( "ADN_Data_ABC", invalidDataOntab_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Data_ABC::GetContext
// Created: ABR 2013-08-23
// -----------------------------------------------------------------------------
const boost::shared_ptr< kernel::Context >& ADN_Data_ABC::GetContext( const std::string& context ) const
{
    return translations_->GetContext( context );
}

// -----------------------------------------------------------------------------
// Name: ADN_Data_ABC::ApplyOnTranslations
// Created: ABR 2013-10-08
// -----------------------------------------------------------------------------
bool ADN_Data_ABC::ApplyOnTranslations( const std::function< bool( kernel::LocalizedString& ) >& functor ) const
{
    return translations_->ApplyOnTranslations( functor );
}

// -----------------------------------------------------------------------------
// Name: ADN_Data_ABC::FixConsistency
// Created: ABR 2013-10-10
// -----------------------------------------------------------------------------
bool ADN_Data_ABC::FixConsistency()
{
    translations_->MergeDuplicateTranslations();
    return false;
}
