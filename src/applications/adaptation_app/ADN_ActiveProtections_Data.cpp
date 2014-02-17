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
#include "ADN_Tools.h"
#include "ADN_Tr.h"
#include "ADN_WorkspaceElement.h"
#include "clients_kernel/XmlTranslations.h"

// -----------------------------------------------------------------------------
// Name: ADN_ActiveProtections_Data constructor
// Created: LDC 2010-01-13
// -----------------------------------------------------------------------------
ADN_ActiveProtections_Data::ADN_ActiveProtections_Data()
    : ADN_Data_ABC( eActiveProtections )
{
    activeProtections_.AddUniquenessChecker( eError, duplicateName_, &ADN_Tools::NameExtractor );
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
// Name: ADN_ActiveProtections_Data::GetActiveProtectionsInfos
// Created: FDS 2010-02-24
// -----------------------------------------------------------------------------
ADN_ActiveProtections_Data::T_ActiveProtectionsInfosVector& ADN_ActiveProtections_Data::GetActiveProtectionsInfos()
{
    return activeProtections_;
}

// -----------------------------------------------------------------------------
// Name: ADN_ActiveProtections_Data::FindWeapon
// Created: FDS 2010-02-24
// -----------------------------------------------------------------------------
ADN_ActiveProtections_Data::ActiveProtectionsInfos* ADN_ActiveProtections_Data::FindActiveProtection(const std::string& strName )
{
    auto it = std::find_if( activeProtections_.begin(), activeProtections_.end(), ADN_Tools::NameCmp( strName ) );
    if( it == activeProtections_.end() )
        return 0;
    return *it;
}

// -----------------------------------------------------------------------------
// Name: ADN_ActiveProtections_Data::FilesNeeded
// Created: LDC 2010-01-13
// -----------------------------------------------------------------------------
void ADN_ActiveProtections_Data::FilesNeeded( tools::Path::T_Paths& vFiles) const
{
   vFiles.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szActiveProtections_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_ActiveProtections_Data::ReadArchive
// Created: LDC 2010-01-13
// -----------------------------------------------------------------------------
void ADN_ActiveProtections_Data::ReadArchive( xml::xistream& xis )
{
    xis >> xml::start( "protections" )
            >> xml::list( "protection", *this, &ADN_ActiveProtections_Data::ReadProtection )
        >> xml::end;
    activeProtections_.CheckValidity();
}

// -----------------------------------------------------------------------------
// Name: ADN_ActiveProtections_Data::ReadProtection
// Created: LDC 2010-01-13
// -----------------------------------------------------------------------------
void ADN_ActiveProtections_Data::ReadProtection( xml::xistream& xis )
{
    std::auto_ptr<ActiveProtectionsInfos> spNew( new ActiveProtectionsInfos() );
    spNew->ReadArchive( xis );
    activeProtections_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_ActiveProtections_Data::WriteArchive
// Created: LDC 2010-01-13
// -----------------------------------------------------------------------------
void ADN_ActiveProtections_Data::WriteArchive( xml::xostream& xos ) const
{
    if( activeProtections_.GetErrorStatus() == eError )
        throw MASA_EXCEPTION( GetInvalidDataErrorMsg() );

    xos << xml::start( "protections" );
    tools::SchemaWriter schemaWriter;
    schemaWriter.WritePhysicalSchema( xos, "ActiveProtections" );
    for( auto it = activeProtections_.begin(); it != activeProtections_.end(); ++it )
        (*it)->WriteArchive( xos );
    xos << xml::end;
}

namespace
{
    ADN_Type_Vector_ABC< ADN_Resources_Data::CategoryInfo >& GetMunitionsVector()
    {
        return ADN_Workspace::GetWorkspace().GetResources().GetData().GetResource( eDotationFamily_Munition ).categories_;
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_ActiveProtections_Data::ActiveProtectionInfosWeapons
// Created: FDS 2010-02-24
// -----------------------------------------------------------------------------
ADN_ActiveProtections_Data::ActiveProtectionsInfosWeapons::ActiveProtectionsInfosWeapons()
    : ADN_CrossedRef( GetMunitionsVector(), 0, true )
    , coefficient_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_ActiveProtections_Data::ReadArchive
// Created: FDS 2010-02-24
// -----------------------------------------------------------------------------
void ADN_ActiveProtections_Data::ActiveProtectionsInfosWeapons::ReadArchive( xml::xistream& xis )
{
    xis >> xml::attribute( "name", *this )
        >> xml::attribute( "coefficient", coefficient_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_ActiveProtections_Data::WriteArchive
// Created: FDS 2010-02-24
// -----------------------------------------------------------------------------
void ADN_ActiveProtections_Data::ActiveProtectionsInfosWeapons::WriteArchive( xml::xostream& xos ) const
{
    xos << xml::start( "weapon" )
          << xml::attribute( "name", *this )
          << xml::attribute( "coefficient", coefficient_)
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_ActiveProtections_Data::ActiveProtectionsInfosWeapons::GetItemName
// Created: FDS 2010-02-24
// -----------------------------------------------------------------------------
std::string ADN_ActiveProtections_Data::ActiveProtectionsInfosWeapons::GetItemName()
{
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: ADN_ActiveProtections_Data::ActiveProtectionsInfosWeapons::CreateCopy
// Created: ABR 2014-02-17
// -----------------------------------------------------------------------------
ADN_ActiveProtections_Data::ActiveProtectionsInfosWeapons* ADN_ActiveProtections_Data::ActiveProtectionsInfosWeapons::CreateCopy()
{
    ActiveProtectionsInfosWeapons* pCopy = new ActiveProtectionsInfosWeapons();
    pCopy->SetCrossedElement( GetCrossedElement() );
    pCopy->coefficient_ = coefficient_.GetData();
    return pCopy;
}

// -----------------------------------------------------------------------------
// Name: ADN_ActiveProtections_Data::ActiveProtectionInfos
// Created: FDS 2010-02-24
// -----------------------------------------------------------------------------
ADN_ActiveProtections_Data::ActiveProtectionsInfos::ActiveProtectionsInfos()
    : ptr_( ( ADN_Resources_Data::T_AmmoCategoryInfo_Vector& )GetMunitionsVector(), 0 )
    , coefficient_  ( 0 )
    , hardKill_     ( false )
    , usage_        ( 0 )
{
    strName_.SetContext( ADN_Workspace::GetWorkspace().GetContext( eActiveProtections, "protections" ) );
    BindExistenceTo( &ptr_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_ActiveProtections_Data::ActiveProtectionsInfos::GetItemName
// Created: FDS 2010-02-24
// -----------------------------------------------------------------------------
std::string ADN_ActiveProtections_Data::ActiveProtectionsInfos::GetItemName()
{
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: ADN_ActiveProtections_Data::ActiveProtectionsInfos::CreateCopy
// Created: LGY 2010-08-18
// -----------------------------------------------------------------------------
ADN_ActiveProtections_Data::ActiveProtectionsInfos* ADN_ActiveProtections_Data::ActiveProtectionsInfos::CreateCopy()
{
    ActiveProtectionsInfos* pCopy = new ActiveProtectionsInfos();
    pCopy->coefficient_ = coefficient_.GetData();
    pCopy->hardKill_ = hardKill_.GetData();
    pCopy->usage_ = usage_.GetData();
    for( auto it = weapons_.begin(); it != weapons_.end(); ++it )
        if( *it )
            pCopy->weapons_.AddItem( ( *it )->CreateCopy() );
    pCopy->ptr_ = ptr_.GetData();
    return pCopy;
}

// -----------------------------------------------------------------------------
// Name: ADN_ActiveProtections_Data::ReadArchive
// Created: LDC 2010-01-13
// -----------------------------------------------------------------------------
void ADN_ActiveProtections_Data::ActiveProtectionsInfos::ReadArchive( xml::xistream& xis )
{
    xis >> xml::attribute( "name", *this )
        >> xml::attribute( "coefficient", coefficient_ )
        >> xml::attribute( "hard-kill", hardKill_ )
        >> xml::optional >> xml::start( "resource" )
            >> xml::attribute( "name", ptr_ )
            >> xml::attribute( "usage", usage_ )
        >> xml::end
        >> xml::list( "weapon", *this, &ADN_ActiveProtections_Data::ActiveProtectionsInfos::ReadWeapon );
}

// -----------------------------------------------------------------------------
// Name: ADN_ActiveProtections_Data::Protection::ReadWeapon
// Created: LDC 2010-01-13
// -----------------------------------------------------------------------------
void ADN_ActiveProtections_Data::ActiveProtectionsInfos::ReadWeapon( xml::xistream& xis )
{
    std::auto_ptr<ActiveProtectionsInfosWeapons> spNew( new ActiveProtectionsInfosWeapons() );
    spNew->ReadArchive( xis );
    weapons_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_ActiveProtections_Data::Protection::Write
// Created: LDC 2010-01-13
// -----------------------------------------------------------------------------
void ADN_ActiveProtections_Data::ActiveProtectionsInfos::WriteArchive( xml::xostream& xos ) const
{
    xos << xml::start( "protection" )
        << xml::attribute( "name", *this )
        << xml::attribute( "coefficient", coefficient_ )
        << xml::attribute( "hard-kill", hardKill_ )
        << xml::start( "resource" )
            << xml::attribute( "name", ptr_ )
            << xml::attribute( "usage", usage_ )
        << xml::end;
    for( auto it = weapons_.begin(); it != weapons_.end(); ++it )
        (*it)->WriteArchive(xos);
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_ActiveProtections_Data::ActiveProtectionsInfos::CheckValidity
// Created: JSR 2013-04-03
// -----------------------------------------------------------------------------
void ADN_ActiveProtections_Data::ActiveProtectionsInfos::CheckValidity( ADN_ConsistencyChecker& checker, const std::string& name, int tab, int subTab /* = -1 */, const std::string& /* optional = "" */ )
{
    ptr_.CheckValidity( checker, name, tab, subTab, tools::translate( "ActiveProtections_Data", "Resource" ).toStdString() );
    for( auto it = weapons_.begin(); it != weapons_.end(); ++it )
        ( *it )->CheckValidity( checker, name, tab, subTab, tools::translate( "ActiveProtections_Data", "Ammunitions" ).toStdString() );
}

// -----------------------------------------------------------------------------
// Name: ADN_ActiveProtections_Data::GetActiveProtectionsThatUse
// Created: ABR 2012-08-01
// -----------------------------------------------------------------------------
QStringList ADN_ActiveProtections_Data::GetActiveProtectionsThatUse( ADN_Resources_Data::AmmoCategoryInfo& ammo )
{
    QStringList result;
    for( auto it = activeProtections_.begin(); it != activeProtections_.end(); ++it )
    {
        const ADN_Resources_Data::AmmoCategoryInfo* info = ( *it )->ptr_.GetData();
        if( info && info->strName_.GetData() == ammo.strName_.GetData() )
            result << ( *it )->strName_.GetData().c_str();
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_ActiveProtections_Data::GetActiveProtectionsThatUse
// Created: ABR 2012-08-01
// -----------------------------------------------------------------------------
QStringList ADN_ActiveProtections_Data::GetActiveProtectionsThatUse( ADN_Resources_Data::CategoryInfo& category )
{
    QStringList result;
    for( auto it = activeProtections_.begin(); it != activeProtections_.end(); ++it )
        for( auto itWeapon = ( *it )->weapons_.begin(); itWeapon != ( *it )->weapons_.end(); ++itWeapon )
        {
            const ADN_Resources_Data::CategoryInfo* info = ( *itWeapon )->GetCrossedElement();
            if( info && info->strName_.GetData() == category.strName_.GetData() )
                result << ( *it )->strName_.GetData().c_str();
        }
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_ActiveProtections_Data::CheckDatabaseValidity
// Created: JSR 2013-04-03
// -----------------------------------------------------------------------------
void ADN_ActiveProtections_Data::CheckDatabaseValidity( ADN_ConsistencyChecker& checker ) const
{
    ADN_Data_ABC::CheckDatabaseValidity( checker );
    for( auto it = activeProtections_.begin(); it != activeProtections_.end(); ++it )
        ( *it )->CheckValidity( checker, ( *it )->strName_.GetData(), eActiveProtections );
}
