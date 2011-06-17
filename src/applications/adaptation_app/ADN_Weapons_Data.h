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
#include "ADN_Types.h"
#include "ADN_Type_Vector_ABC.h"
#include "ADN_Type_VectorFixed_ABC.h"
#include "ADN_Categories_Data.h"
#include "ADN_Launchers_Data.h"
#include "ADN_Equipement_Data.h"

namespace xml { class xistream; }

// =============================================================================
/** @class  ADN_Weapons_Data
*/
// Created: APE 2004-11-19
// =============================================================================
class ADN_Weapons_Data : public ADN_Data_ABC
{

public:
// *****************************************************************************
    class PhInfos : public ADN_DataTreeNode_ABC
    {
    public:
        PhInfos();

        virtual std::string GetNodeName();
        std::string GetItemName();

        PhInfos* CreateCopy();

        void SetPhModifiers( double phModifier, double distModifier );
        void ApplyPhModifiers();

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output );

    public:
        ADN_Type_Int       nDistance_;
        ADN_Type_Double    rPerc_;
        ADN_Type_Int       nModifiedDistance_;
        ADN_Type_Double    rModifiedPerc_;
        double             phModifier_;
        double             distModifier_;
    };

    typedef ADN_Type_Vector_ABC<PhInfos>    T_PhInfosVector;
    typedef T_PhInfosVector::iterator       IT_PhInfosVector;


// *****************************************************************************
    class PhSizeInfos : public ADN_Ref_ABC
                      , public ADN_DataTreeNode_ABC
    {
    public:
        explicit PhSizeInfos( ADN_Categories_Data::SizeInfos *ptr );
        virtual ~PhSizeInfos();

        virtual std::string GetNodeName();
        std::string GetItemName();

        void ReadArchive( xml::xistream& input );
        void ReadHp( xml::xistream& input );
        void WriteArchive( xml::xostream& output );

    public:
        ADN_TypePtr_InVector_ABC<ADN_Categories_Data::SizeInfos>   ptrSize_;
        T_PhInfosVector                                            vPhs_;

    public:
        typedef ADN_Categories_Data::SizeInfos                  T_Item;

        class Cmp : public std::unary_function< PhSizeInfos* , bool >
        {
        public:
            Cmp(const std::string& val) : val_(val) {}
            ~Cmp() {}

            bool operator()( PhSizeInfos* tgtnfos ) const
            {   return tgtnfos->ptrSize_.GetData() && tgtnfos->ptrSize_.GetData()->GetData()==val_;}

        private:
            std::string val_;
        };

        class CmpRef : public std::unary_function< PhSizeInfos* , bool >
        {
        public:
            CmpRef(ADN_Categories_Data::SizeInfos* val) : val_(val) {}
            ~CmpRef(){}

            bool operator()( PhSizeInfos* tgtnfos ) const
            {   return tgtnfos->ptrSize_.GetData() == val_;}

        private:
            ADN_Categories_Data::SizeInfos* val_;
        };


    };

    typedef ADN_Type_VectorFixed_ABC<PhSizeInfos> T_PhSizeInfosVector;
    typedef T_PhSizeInfosVector::iterator        IT_PhSizeInfosVector;


// *****************************************************************************
    class WeaponInfos : public ADN_Ref_ABC
                      , public ADN_DataTreeNode_ABC
    {
    public:
        WeaponInfos();

        virtual std::string GetNodeName();
        std::string GetItemName();

        WeaponInfos* CreateCopy();

        void ReadArchive( xml::xistream& input );
        void ReadTargetSize( xml::xistream& input );
        void WriteArchive( xml::xostream& output );

    public:
        ADN_Type_String strName_; // $$$ Dummy for  ADN_ChangeValueCommand_Pointer<>::UpdateDescription()

        ADN_TypePtr_InVector_ABC<ADN_Launchers_Data::LauncherInfos>         ptrLauncher_;
        ADN_TypePtr_InVector_ABC<ADN_Equipement_Data::AmmoCategoryInfo>     ptrAmmunition_;

        ADN_Type_Int       nRoundsPerBurst_;
        ADN_Type_Time      burstDuration_;
        ADN_Type_Int       nRoundsPerReload_;
        ADN_Type_Time      reloadDuration_;

        ADN_Type_Bool       bDirect_;
        T_PhSizeInfosVector phs_;

        ADN_Type_Bool bSimulation_;
        ADN_Type_Enum< E_UnitPosture, eNbrUnitPosture > nFirePosture_;
        ADN_Type_Enum< E_UnitPosture, eNbrUnitPosture > nTargetPosture_;
        ADN_Type_Enum< E_UnitExperience, eNbrUnitExperience > nExperience_;
        ADN_Type_Enum< E_UnitTiredness, eNbrUnitTiredness > nTiredness_;
        ADN_Type_Enum< E_UnitStress, eNbrUnitStress > nStress_;

        ADN_Type_Bool      bIndirect_;
        ADN_Type_Double    rAverageSpeed_;
        ADN_Type_Double    rMinRange_;
        ADN_Type_Double    rMaxRange_;

    public:
        class Cmp : public std::unary_function< WeaponInfos* , bool >
        {
        public:
            Cmp( const std::string& strLauncher, const std::string& strAmmo )
                : strLauncher_(strLauncher)
                , strAmmo_(strAmmo)
            {}
            ~Cmp(){}

            bool operator()( WeaponInfos* tgtnfos ) const
            {
                return (  ADN_Tools::CaselessCompare( tgtnfos->ptrLauncher_.GetData()->strName_.GetData(), strLauncher_ )
                       && ADN_Tools::CaselessCompare( tgtnfos->ptrAmmunition_.GetData()->strName_.GetData(), strAmmo_ ) );
            }

        private:
            std::string strLauncher_;
            std::string strAmmo_;
        };
    };

    typedef ADN_Type_Vector_ABC<WeaponInfos>       T_WeaponInfosVector;
    typedef T_WeaponInfosVector::iterator           IT_WeaponInfosVector;


// *****************************************************************************
public:
             ADN_Weapons_Data();
    virtual ~ADN_Weapons_Data();

    void FilesNeeded( T_StringList& l ) const;
    void Reset();

    T_WeaponInfosVector& GetWeaponInfos();
    WeaponInfos*         FindWeapon( const std::string& strLauncher, const std::string& strAmmunition );
    int                  GetIndex( WeaponInfos& weapon );
    std::string          GetWeaponThatUse( ADN_Launchers_Data::LauncherInfos& launcher );
    std::string          GetWeaponThatUse( ADN_Equipement_Data::AmmoCategoryInfo& ammunition );
    void                 UpdateNames();

private:
    void ReadWeapon( xml::xistream& input );
    void ReadArchive( xml::xistream& input );
    void WriteArchive( xml::xostream& output );

public:
    T_WeaponInfosVector weapons_;
};

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_Data::GetWeaponInfos
// Created: APE 2004-11-30
// -----------------------------------------------------------------------------
inline
ADN_Weapons_Data::T_WeaponInfosVector& ADN_Weapons_Data::GetWeaponInfos()
{
    return weapons_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_Data::FindWeapon
// Created: APE 2004-11-30
// -----------------------------------------------------------------------------
inline
ADN_Weapons_Data::WeaponInfos* ADN_Weapons_Data::FindWeapon( const std::string& strLauncher, const std::string& strAmmunition )
{
    IT_WeaponInfosVector it = std::find_if( weapons_.begin(), weapons_.end(), WeaponInfos::Cmp( strLauncher, strAmmunition ) );
    if( it == weapons_.end() )
        return 0;
    return *it;
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_Data::GetIndex
// Created: APE 2005-04-21
// -----------------------------------------------------------------------------
inline
int ADN_Weapons_Data::GetIndex( WeaponInfos& weapon )
{
    IT_WeaponInfosVector it = std::find( weapons_.begin(), weapons_.end(), &weapon );
    assert( it != weapons_.end() );
    return static_cast< int >( std::distance( weapons_.begin(), it ) );
}

#endif // __ADN_Weapons_Data_h_
