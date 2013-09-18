// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Armors_Data_h_
#define __ADN_Armors_Data_h_

#include "ADN_Data_ABC.h"
#include "ADN_RefWithName.h"
#include "ADN_AttritionEffectOnHuman.h"

class ADN_ListView_Categories_Armor;

// =============================================================================
/** @class  ADN_Armors_Data
    @brief  ADN_Armors_Data
*/
// Created: ABR 2013-07-10
// =============================================================================
class ADN_Armors_Data : public ADN_Data_ABC
{
public:
    //! @name Armors infos
    //@{
    class ArmorInfos : public ADN_RefWithLocalizedName
    {

    public:
        ArmorInfos();
        std::string GetItemName();

        ArmorInfos* CreateCopy();
        void ReadArchive( xml::xistream& );
        void ReadAttrition( xml::xistream& );
        void WriteArchive( xml::xostream& );
        void CreateDefaultAttrition();

    public:
        ADN_Type_Enum< E_ProtectionType, eNbrProtectionType > nType_;
        ADN_Type_Time   neutralizationAverageTime_;
        ADN_Type_Time   neutralizationVariance_;
        ADN_Type_Double rBreakdownEVA_;
        ADN_Type_Double rBreakdownNEVA_;
        helpers::T_AttritionEffectOnHuman_Vector vAttritionEffects_;
    };

    typedef ADN_Type_Vector_ABC< ArmorInfos > T_ArmorInfos_Vector;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             ADN_Armors_Data();
    virtual ~ADN_Armors_Data();
    //@}

    //! @name Operations
    //@{
    void FilesNeeded( tools::Path::T_Paths& vFiles ) const;
    virtual void CheckDatabaseValidity( ADN_ConsistencyChecker& checker ) const;

    T_ArmorInfos_Vector& GetArmorsInfos();
    ArmorInfos* FindArmor( const std::string& strName );
    //@}

private:
    //! @name Serialization
    //@{
    void ReadArchive( xml::xistream& input );
    void ReadArmor( xml::xistream& input );
    void WriteArchive( xml::xostream& output );
    //@}

private:
    //! @name Member data
    //@{
    T_ArmorInfos_Vector vArmors_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: ADN_Armors_Data::GetArmorsInfos
// Created: ABR 2013-07-11
// -----------------------------------------------------------------------------
inline
ADN_Armors_Data::T_ArmorInfos_Vector& ADN_Armors_Data::GetArmorsInfos()
{
    return vArmors_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Armors_Data::FindArmor
// Created: ABR 2013-07-11
// -----------------------------------------------------------------------------
inline
ADN_Armors_Data::ArmorInfos* ADN_Armors_Data::FindArmor( const std::string& strName )
{
    auto it = std::find_if( vArmors_.begin(), vArmors_.end(), ADN_Tools::NameCmp( strName ) );
    if( it == vArmors_.end() )
        return 0;
    return *it;
}

#endif // __ADN_Armors_Data_h_
