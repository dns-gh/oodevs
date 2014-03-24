// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Weapons_Data_WEAPONINFOS_h_
#define __ADN_Weapons_Data_WEAPONINFOS_h_

#include "ADN_RefWithName.h"
#include "ADN_Launchers_Data.h"
#include "ADN_Resources_Data.h"
#include "ADN_Weapons_Data_PhInfos.h"

// =============================================================================
/** @class  ADN_Weapons_Data_WeaponInfos
*/
// Created: APE 2004-11-19
// =============================================================================
class ADN_Weapons_Data_WeaponInfos : public ADN_RefWithName
{
    Q_OBJECT
    
public:
             ADN_Weapons_Data_WeaponInfos();
    virtual ~ADN_Weapons_Data_WeaponInfos();

    ADN_Weapons_Data_WeaponInfos* CreateCopy();
    void ReadArchive( xml::xistream& input );
    void ReadTargetSize( xml::xistream& input );
    void WriteArchive( xml::xostream& output ) const;

    bool IsValidOnChange( bool bDirect, bool bLauncher ) const;
    void ConnectLauncherAmmunition();
    void CheckDatabaseValidity( ADN_ConsistencyChecker& ) const;

public:
    ADN_TypePtr_InVector_ABC< ADN_Launchers_Data::LauncherInfos > ptrLauncher_;
    ADN_TypePtr_InVector_ABC< ADN_Resources_Data::AmmoCategoryInfo > ptrAmmunition_;

    ADN_Type_Int    nRoundsPerBurst_;
    ADN_Type_Time   burstDuration_;
    ADN_Type_Int    nRoundsPerReload_;
    ADN_Type_Time   reloadDuration_;

    ADN_Type_Bool   bDirect_;
    ADN_Type_VectorFixed_ABC< ADN_Weapons_Data_PhSizeInfos > phs_;

    ADN_Type_Bool bSimulation_;
    ADN_Type_Enum< E_UnitPosture, eNbrUnitPosture > nFirePosture_;
    ADN_Type_Enum< E_UnitPosture, eNbrUnitPosture > nTargetPosture_;
    ADN_Type_Enum< E_UnitExperience, eNbrUnitExperience > nExperience_;
    ADN_Type_Enum< E_UnitTiredness, eNbrUnitTiredness > nTiredness_;
    ADN_Type_Enum< E_UnitStress, eNbrUnitStress > nStress_;

    ADN_Type_Bool bIndirect_;
    ADN_Type_Double rAverageSpeed_;
    ADN_Type_Double rMinRange_;
    ADN_Type_Double rMaxRange_;

private slots:
    void LauncherOrAmmunitionChanged();
    void OnNameChanged( void* );

public:
    class Cmp : public std::unary_function< ADN_Weapons_Data_WeaponInfos* , bool >
    {
    public:
        Cmp( const std::string& strLauncher, const std::string& strAmmo )
            : strLauncher_( strLauncher )
            , strAmmo_(strAmmo)
        {}
        ~Cmp(){}

        bool operator()( ADN_Weapons_Data_WeaponInfos* tgtnfos ) const
        {
            if( !tgtnfos->ptrLauncher_.GetData() || !tgtnfos->ptrAmmunition_.GetData() )
                return false;
            return tgtnfos->ptrLauncher_.GetData()->strName_ == strLauncher_ && tgtnfos->ptrAmmunition_.GetData()->strName_ == strAmmo_;
        }

    private:
        std::string strLauncher_;
        std::string strAmmo_;
    };
};

#endif // __ADN_Weapons_Data_WEAPONINFOS_h_
