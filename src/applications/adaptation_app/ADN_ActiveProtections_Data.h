// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ADN_ActiveProtections_Data_h_
#define __ADN_ActiveProtections_Data_h_

#include "ADN_CrossedRef.h"
#include "ADN_Data_ABC.h"
#include "ADN_Types.h"
#include "ADN_Type_Vector_ABC.h"
#include "ADN_Resources_Data.h"

// =============================================================================
// @class  ADN_ActiveProtections_Data
//  @brief  ADN_ActiveProtections_Data
//
// Created: LDC 2010-01-13
// =============================================================================
class ADN_ActiveProtections_Data : public ADN_Data_ABC
{
public:

    class ActiveProtectionsInfosWeapons : public ADN_CrossedRef< ADN_Resources_Data::CategoryInfo >
    {
    public:
                 ActiveProtectionsInfosWeapons();
        virtual ~ActiveProtectionsInfosWeapons() {}
        std::string GetItemName();
        ActiveProtectionsInfosWeapons* CreateCopy();

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& xos ) const;

    public:
        ADN_Type_Double coefficient_;
    };

    typedef ADN_Type_Vector_ABC< ActiveProtectionsInfosWeapons > T_ActiveProtectionsInfosWeaponsVector;

    //! @name Types
    //@{
    class ActiveProtectionsInfos : public ADN_RefWithLocalizedName
    {
    public:
                 ActiveProtectionsInfos();
        virtual ~ActiveProtectionsInfos() {}
        std::string GetItemName();

        ActiveProtectionsInfos* CreateCopy();

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& xos ) const;
        void ReadWeapon( xml::xistream& xis );
        using ADN_RefWithLocalizedName::CheckValidity;
        virtual void CheckValidity( ADN_ConsistencyChecker& checker, const std::string& name, int tab, int subTab = -1, const std::string& optional = "" );

    public:
        ADN_TypePtr_InVector_ABC< ADN_Resources_Data::AmmoCategoryInfo > ptr_;
        ADN_Type_Double coefficient_;
        ADN_Type_Bool hardKill_;
        ADN_Type_Double usage_;
        T_ActiveProtectionsInfosWeaponsVector weapons_;
    };

    typedef ADN_Type_Vector_ABC< ActiveProtectionsInfos > T_ActiveProtectionsInfosVector;

    //! @name Constructors/Destructor
    //@{
             ADN_ActiveProtections_Data();
    virtual ~ADN_ActiveProtections_Data();
    //@}

    //! @name Helpers
    //@{
    ActiveProtectionsInfos* FindActiveProtection(const std::string& stdName );
    //@}

    //! @name Operations
    //@{
    virtual void FilesNeeded( tools::Path::T_Paths& vFiles ) const;
    virtual void CheckDatabaseValidity( ADN_ConsistencyChecker& checker ) const;

    T_ActiveProtectionsInfosVector& GetActiveProtectionsInfos();

    QStringList GetActiveProtectionsThatUse( ADN_Resources_Data::AmmoCategoryInfo& ammo );
    QStringList GetActiveProtectionsThatUse( ADN_Resources_Data::CategoryInfo& category );
    //@}

    //! @name Member data
    //@{
    T_ActiveProtectionsInfosVector activeProtections_;
    //@}
private:

    //! @name Helpers
    //@{
    virtual void ReadArchive( xml::xistream& xis );
    virtual void WriteArchive( xml::xostream& xos ) const;
    virtual void ReadProtection( xml::xistream& xis );
    //@}
};

#endif // __ADN_ActiveProtections_Data_h_
