// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Resources_Data_h_
#define __ADN_Resources_Data_h_

#include "ADN_Data_ABC.h"
#include "ADN_RefWithName.h"
#include "ADN_Types.h"
#include "ADN_Type_Vector_ABC.h"
#include "ADN_Enums.h"
#include "ADN_Objects_Data_ObjectInfos.h"
#include "ADN_Natures_Data.h"
#include "ADN_LogisticSupplyClasses_Data.h"
#include "ADN_UrbanAttritionInfos.h"

namespace sword
{
    enum DotationType;
}

// =============================================================================
/** @class  ADN_Resources_Data
*/
// Created: APE 2004-11-15
// =============================================================================
class ADN_Resources_Data : public ADN_Data_ABC
{
    friend class ADN_Equipement;

public:
    class ResourceInfos;

// *****************************************************************************
    class CategoryInfo : public ADN_RefWithLocalizedName
    {

    public:
                 CategoryInfo(); // For template usage.
        explicit CategoryInfo( ResourceInfos& parentDotation );
                 CategoryInfo( ResourceInfos& parentDotation, unsigned int id );
        virtual ~CategoryInfo();

        virtual CategoryInfo* CreateCopy();
        virtual void ReadArchive( xml::xistream& );
        virtual void WriteArchive( xml::xostream& ) const;
        virtual void WriteContent( xml::xostream& ) const;
        virtual void Initialize();

    public:
        ADN_Type_Int nId_;
        ResourceInfos&  parentResource_;
        ADN_Type_String category_;
        ADN_Type_String strCodeEMAT6_;
        ADN_Type_String strCodeEMAT8_;
        ADN_Type_String strCodeLFRIL_;
        ADN_Type_String strCodeNNO_;
        ADN_TypePtr_InVector_ABC< ADN_Natures_Data::NatureInfos > ptrResourceNature_;
        ADN_TypePtr_InVector_ABC< ADN_LogisticSupplyClasses_Data::LogisticSupplyClass > ptrLogisticSupplyClass_;
        ADN_Type_Double rNbrInPackage_;
        ADN_Type_Double rPackageVolume_;
        ADN_Type_Double rPackageWeight_;
        ADN_Type_Bool bNetworkUsable_;
    };
    typedef ADN_Type_Vector_ABC< CategoryInfo > T_CategoryInfos_Vector;

//*****************************************************************************
    class ModificatorPostureInfos : public ADN_Ref_ABC
    {
    public:
        explicit ModificatorPostureInfos( const E_UnitPosture& e );

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;

    public:
        E_UnitPosture eType_;
        ADN_Type_Double rCoeff_;

    public:
        class Cmp : public std::unary_function< ModificatorPostureInfos* , bool >
        {
        public:
             Cmp( const E_UnitPosture& val) : val_( val ) {}
            ~Cmp() {}

            bool operator()( ModificatorPostureInfos* tgtnfos ) const
            {
                return tgtnfos->eType_ == val_;
            }

        private:
             E_UnitPosture val_;
        };
    };
    typedef ADN_Type_Vector_ABC< ModificatorPostureInfos > T_ModificatorPostureInfos_Vector;

// *****************************************************************************
    class IndirectAmmoInfos : public ADN_Ref_ABC
    {
    public:
                 IndirectAmmoInfos();
        virtual ~IndirectAmmoInfos();

        void CopyFrom( IndirectAmmoInfos& ammoInfos );

        void ReadArchive( xml::xistream& );
        void ReadIndirectFire( xml::xistream& );
        void ReadPh( xml::xistream& );
        void WriteArchive( xml::xostream& ) const;
        void Initialize();

    public:
        // Present types
        ADN_Type_Bool bExplosive_;
        ADN_Type_Bool bSmoke_;
        ADN_Type_Bool bFlare_;
        ADN_Type_Bool bEffect_;

        ADN_Type_Int nIntervention_;
        ADN_Type_Double rDispersionX_;
        ADN_Type_Double rDispersionY_;
        ADN_Type_Double rDetectionRange_;

        // For explosive ammo
        ADN_Type_Double rNeutralizationRatio_;
        ADN_Type_Double rExplosiveDispersion_;
        T_ModificatorPostureInfos_Vector vModifStance_;

        // For flare
        ADN_Type_Time flareDeployTime_;
        ADN_Type_Time flareLifeTime_;
        ADN_Type_Double rFlareDispersion_;

        // For smoke
        ADN_Type_Time smokeDeployTime_;
        ADN_Type_Time smokeLifeTime_;
        ADN_Type_Double rSmokeDispersion_;

        // For effect
        std::string strObjectType_;
        ADN_TypePtr_InVector_ABC< ADN_Objects_Data_ObjectInfos > objectType_;
        ADN_Type_Time effectLifeTime_;
    };

// *****************************************************************************
    class AmmoCategoryInfo : public CategoryInfo
    {
    public:
        explicit AmmoCategoryInfo( ResourceInfos& parentDotation );
                 AmmoCategoryInfo( ResourceInfos& parentDotation, unsigned int id );
        virtual ~AmmoCategoryInfo();

        CategoryInfo* CreateCopy();

        void ReadArchive( xml::xistream& );
        void ReadAttrition( xml::xistream& );
        void ReadUrbanModifer( xml::xistream& );
        void WriteArchive( xml::xostream& ) const;
        bool HasUrbanAttrition() const;
        virtual void Initialize();

    public:
        ADN_Type_Enum< E_AmmunitionType, eNbrAmmunitionType > nType_;

        ADN_Type_Bool bDirect_;
        ADN_Type_Bool bIndirect_;

        helpers::T_AttritionInfos_Vector attritions_;
        helpers::T_UrbanAttritionInfos_Vector modifUrbanBlocks_;
        ADN_Type_Bool bIlluminating_;
        ADN_Type_Bool bMaintainIllumination_;
        ADN_Type_Double fRange_;
        ADN_Type_Bool bIED_;

        IndirectAmmoInfos indirectAmmoInfos_;

        ADN_Type_Bool bGuided_;
        ADN_Type_Bool bMaintainGuidance_;
        ADN_Type_Double fGuidanceRange_;
    };

    typedef ADN_Type_Vector_ABC< AmmoCategoryInfo > T_AmmoCategoryInfo_Vector;

// *****************************************************************************
    class ResourceInfos : public ADN_RefWithName
    {
    public:
        explicit ResourceInfos( sword::DotationType nType, T_CategoryInfos_Vector& networkUsableResources );
        virtual ~ResourceInfos();

        T_CategoryInfos_Vector& GetCategories();
        CategoryInfo* FindCategory( const std::string& strName );

        void ReadArchive( xml::xistream&, kernel::XmlTranslations& );
        void WriteArchive( xml::xostream& ) const;
        void Initialize();
        void CheckDatabaseValidity( ADN_ConsistencyChecker& checker ) const;
        void NotifyDestroyed( CategoryInfo* );
        void AddUsableInNetwork( CategoryInfo* );
        void RemoveNetworkUsableResources();

    public:
        sword::DotationType nType_;
        T_CategoryInfos_Vector categories_;
        T_CategoryInfos_Vector* networkUsableResources_;
    };

    class T_ResourceInfos_Vector : public ADN_Type_Vector_ABC< ResourceInfos >
    {
    public:
        explicit T_ResourceInfos_Vector( bool bAutoRef = true ) : ADN_Type_Vector_ABC< ResourceInfos >( bAutoRef ) {}
        virtual ~T_ResourceInfos_Vector() {}

        virtual void CheckValidity();
    };

// *****************************************************************************
public:
             ADN_Resources_Data();
    virtual ~ADN_Resources_Data();

public:
    void FilesNeeded(tools::Path::T_Paths& files ) const;
    virtual void CheckDatabaseValidity( ADN_ConsistencyChecker& checker ) const;

    T_ResourceInfos_Vector& GetResources();
    ResourceInfos& GetResource( sword::DotationType nType );
    T_CategoryInfos_Vector& GetNetworkUsableResources();
    CategoryInfo* FindResourceCategory( const std::string& strDotationName, const std::string& strCategoryName );
    CategoryInfo* FindResourceCategory( const std::string& strCategoryName );

    void Initialize();

    QStringList GetResourcesThatUse( ADN_Objects_Data_ObjectInfos& object );
    QStringList GetResourcesThatUse( ADN_Natures_Data::NatureInfos& object );
    QStringList GetResourcesThatUse( ADN_Natures_Data::NatureInfos& object, sword::DotationType familly );
    QStringList GetResourcesThatUse( ADN_LogisticSupplyClasses_Data::LogisticSupplyClass& object );
    QStringList GetResourcesWithDirectFire();

    static bool IsMineOrExplosive( sword::DotationType type );

private:
    void ReadArchive( xml::xistream& );
    void ReadResource( xml::xistream& );
    void WriteArchive( xml::xostream& ) const;

private:
    T_ResourceInfos_Vector resources_;
    T_CategoryInfos_Vector networkUsableResources_;

private:
    static tools::IdManager idManager_;
};

// -----------------------------------------------------------------------------
// Name: ADN_Resources_Data::GetResources
// Created: APE 2004-12-01
// -----------------------------------------------------------------------------
inline
ADN_Resources_Data::T_ResourceInfos_Vector& ADN_Resources_Data::GetResources()
{
    return resources_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Resources_Data::GetNetworkUsableResources
// Created: ABR 2012-02-29
// -----------------------------------------------------------------------------
inline
ADN_Resources_Data::T_CategoryInfos_Vector& ADN_Resources_Data::GetNetworkUsableResources()
{
    return networkUsableResources_;
}

#endif // __ADN_Resources_Data_h_
