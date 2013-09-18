// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Weapons_Data_h_
#define __ADN_Weapons_Data_h_

#include "ADN_Data_ABC.h"
#include "ADN_Type_Vector_ABC.h"
#include "ADN_Launchers_Data.h"
#include "ADN_Resources_Data.h"
#include "ADN_Weapons_Data_WeaponInfos.h"

// =============================================================================
/** @class  ADN_Weapons_Data
*/
// Created: APE 2004-11-19
// =============================================================================
class ADN_Weapons_Data : public ADN_Data_ABC
{
public:
             ADN_Weapons_Data();
    virtual ~ADN_Weapons_Data();

    void FilesNeeded( tools::Path::T_Paths& l ) const;
    virtual void CheckDatabaseValidity( ADN_ConsistencyChecker& checker ) const;

    ADN_Type_Vector_ABC< ADN_Weapons_Data_WeaponInfos >& GetWeaponInfos();
    ADN_Weapons_Data_WeaponInfos* FindWeapon( const std::string& strLauncher, const std::string& strAmmunition );
    int GetIndex( ADN_Weapons_Data_WeaponInfos& weapon );
    QStringList GetWeaponThatUse( ADN_Launchers_Data::LauncherInfos& launcher );
    QStringList GetWeaponThatUse( ADN_Resources_Data::AmmoCategoryInfo& ammunition );

private:
    void ReadWeapon( xml::xistream& input );
    void ReadArchive( xml::xistream& input );
    void WriteArchive( xml::xostream& output );

public:
    ADN_Type_Vector_ABC< ADN_Weapons_Data_WeaponInfos > weapons_;
};

#endif // __ADN_Weapons_Data_h_
