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
    Reset();
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
    IT_ActiveProtectionsInfosVector it = std::find_if( activeProtections_.begin(), activeProtections_.end(), ADN_Tools::NameCmp<ActiveProtectionsInfos>( strName ) );
    if( it == activeProtections_.end() )
        return 0;
    return *it;
}

// -----------------------------------------------------------------------------
// Name: ADN_ActiveProtections_Data::FilesNeeded
// Created: LDC 2010-01-13
// -----------------------------------------------------------------------------
void ADN_ActiveProtections_Data::FilesNeeded( T_StringList& vFiles) const
{
   vFiles.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szActiveProtections_.GetData() );
}

// -----------------------------------------------------------------------------
// Name: ADN_ActiveProtections_Data::Reset
// Created: LDC 2010-01-13
// -----------------------------------------------------------------------------
void ADN_ActiveProtections_Data::Reset()
{
    activeProtections_.Reset();
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
    std::auto_ptr<ActiveProtectionsInfos> spNew( new ActiveProtectionsInfos() );
    spNew->ReadArchive( xis );
    activeProtections_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_ActiveProtections_Data::WriteArchive
// Created: LDC 2010-01-13
// -----------------------------------------------------------------------------
void ADN_ActiveProtections_Data::WriteArchive( xml::xostream& xos )
{
    xos << xml::start( "protections" )
            << xml::prefix( "http://www.w3.org/2001/XMLSchema-instance", "xsi" )
                << xml::ns( "http://www.w3.org/2001/XMLSchema-instance" )
                    << xml::attribute( "noNamespaceSchemaLocation", "schemas/physical/ActiveProtections.xsd" );
    for( IT_ActiveProtectionsInfosVector it = activeProtections_.begin(); it != activeProtections_.end(); ++it )
        (*it)->WriteArchive( xos );
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_ActiveProtections_Data::ActiveProtectionInfosWeapons
// Created: FDS 2010-02-24
// -----------------------------------------------------------------------------
ADN_ActiveProtections_Data::ActiveProtectionsInfosWeapons::ActiveProtectionsInfosWeapons()
: ADN_Ref_ABC           ()
, ADN_DataTreeNode_ABC  ()
, strName_              ( tr( "Active Protection weapons" ).ascii() )
, ptrWeapon_            ( ADN_Workspace::GetWorkspace().GetEquipements().GetData().GetDotation( eDotationFamily_Munition ).categories_, 0 )
, coefficient_          ( 0 )
{
        BindExistenceTo(&ptrWeapon_);
}

// -----------------------------------------------------------------------------
// Name: ADN_ActiveProtections_Data::ReadArchive
// Created: FDS 2010-02-24
// -----------------------------------------------------------------------------
void ADN_ActiveProtections_Data::ActiveProtectionsInfosWeapons::ReadArchive( xml::xistream& xis )
{
    std::string strAmmunition;
    xis >> xml::attribute( "name", strAmmunition)
        >> xml::attribute( "coefficient", coefficient_ );

    ADN_Equipement_Data::CategoryInfo* pWeapon = ADN_Workspace::GetWorkspace().GetEquipements().GetData().FindEquipementCategory( "munition", strAmmunition );
    if( !pWeapon )
        throw ADN_DataException( tr( "Invalid data" ).ascii(), tr( "Equipment - Invalid amunition '%1/%2'" ).arg( strAmmunition.c_str() ).ascii() );
    ptrWeapon_ = (ADN_Equipement_Data::AmmoCategoryInfo*)pWeapon;
    strName_ = pWeapon->strName_.GetData();
}

// -----------------------------------------------------------------------------
// Name: ADN_ActiveProtections_Data::WriteArchive
// Created: FDS 2010-02-24
// -----------------------------------------------------------------------------
void ADN_ActiveProtections_Data::ActiveProtectionsInfosWeapons::WriteArchive( xml::xostream& xos )
{
    xos << xml::start( "weapon" )
        << xml::attribute( "name", ptrWeapon_.GetData()->strName_ )
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
// Name: ADN_ActiveProtections_Data::ActiveProtectionInfos
// Created: FDS 2010-02-24
// -----------------------------------------------------------------------------
ADN_ActiveProtections_Data::ActiveProtectionsInfos::ActiveProtectionsInfos()
: ADN_Ref_ABC           ()
, ADN_DataTreeNode_ABC  ()
, strName_              ( tr( "Active Protection" ).ascii() )
, coefficient_          ( 0 )
, hardKill_             ( false )
, usage_                (0.0)
, ptrAmmunition_        ( (ADN_Equipement_Data::T_AmmoCategoryInfo_Vector&)ADN_Workspace::GetWorkspace().GetEquipements().GetData().GetDotation( eDotationFamily_Munition ).categories_, 0, "" )
{
    BindExistenceTo( &ptrAmmunition_ );
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
// Name: ADN_ActiveProtections_Data::ActiveProtectionsInfos::GetActiveProtectionsInfosWeapons
// Created: FDS 2010-02-24
// -----------------------------------------------------------------------------
ADN_ActiveProtections_Data::T_ActiveProtectionsInfosWeaponsVector& ADN_ActiveProtections_Data::ActiveProtectionsInfos::GetActiveProtectionsInfosWeapons()
{
    return weapons_;
}

// -----------------------------------------------------------------------------
// Name: ADN_ActiveProtections_Data::ReadArchive
// Created: LDC 2010-01-13
// -----------------------------------------------------------------------------
void ADN_ActiveProtections_Data::ActiveProtectionsInfos::ReadArchive( xml::xistream& xis )
{
   std::string strAmmunition;

    xis >> xml::attribute( "name"       , strName_ )
        >> xml::attribute( "coefficient", coefficient_ )
        >> xml::attribute( "hard-kill"  , hardKill_ )
        >> xml::optional >> xml::start( "dotation" )
            >> xml::attribute( "name", strAmmunition )
            >> xml::attribute( "usage", usage_ )
        >> xml::end
        >> xml::list( "weapon", *this, &ADN_ActiveProtections_Data::ActiveProtectionsInfos::ReadWeapon );

    ADN_Equipement_Data::CategoryInfo* pAmmo = ADN_Workspace::GetWorkspace().GetEquipements().GetData().FindEquipementCategory( "munition", strAmmunition );
    if( !pAmmo )
        throw ADN_DataException( tr( "Invalid data" ).ascii(), tr( "Active protection '%1' - Invalid ammunition type '%2'" ).arg( strName_.GetData().c_str() ,strAmmunition.c_str() ).ascii() );
    ptrAmmunition_ = (ADN_Equipement_Data::AmmoCategoryInfo*)pAmmo;
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
void ADN_ActiveProtections_Data::ActiveProtectionsInfos::WriteArchive( xml::xostream& xos )
{
    xos << xml::start( "protection" )
        << xml::attribute( "name", strName_ )
        << xml::attribute( "coefficient", coefficient_ )
        << xml::attribute( "hard-kill", hardKill_ );

    if( ptrAmmunition_.GetData()->strName_ != "" )
        xos << xml::start( "dotation" )
                << xml::attribute( "name", ptrAmmunition_.GetData()->strName_ )
                << xml::attribute( "usage", usage_ )
            << xml::end;

    for( IT_ActiveProtectionsInfosWeaponsVector it = weapons_.begin(); it != weapons_.end(); ++it )
        (*it)->WriteArchive(xos);

    xos << xml::end;
}
