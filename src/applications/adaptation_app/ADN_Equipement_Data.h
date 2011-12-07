// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Equipement_Data_h_
#define __ADN_Equipement_Data_h_

#include "ADN_Data_ABC.h"
#include "ADN_Types.h"
#include "ADN_Type_Vector_ABC.h"
#include "ADN_Type_VectorFixed_ABC.h"
#include "ADN_Enums.h"
#include "ADN_Categories_Data.h"
#include "ADN_Objects_Data_ObjectInfos.h"
#include "ADN_ResourceNatureInfos.h"
#include "ADN_LogisticSupplyClass.h"
#include "ADN_UrbanAttritionInfos.h"
#include <boost/noncopyable.hpp>

namespace xml { class xistream; }

// =============================================================================
/** @class  ADN_Equipement_Data
*/
// Created: APE 2004-11-15
// =============================================================================
class ADN_Equipement_Data : public ADN_Data_ABC
{
    friend class ADN_Equipement;

public:
    class ResourceInfos;

// *****************************************************************************
    class CategoryInfo : public ADN_Ref_ABC
                       , public ADN_DataTreeNode_ABC
    {

    public:
                 CategoryInfo(); // For template usage.
        explicit CategoryInfo( ResourceInfos& parentDotation );

        virtual std::string GetNodeName();
        std::string GetItemName();

        virtual CategoryInfo* CreateCopy();

        virtual void ReadArchive( xml::xistream& );
        virtual void WriteArchive( xml::xostream& ) const;
        virtual void WriteContent( xml::xostream& ) const;
        virtual void Initialize();

    public:
        ResourceInfos&  parentResource_;
        ADN_Type_String strName_;
        ADN_Type_String category_;
        int             nMosId_;
        ADN_Type_String strCodeEMAT6_;
        ADN_Type_String strCodeEMAT8_;
        ADN_Type_String strCodeLFRIL_;
        ADN_Type_String strCodeNNO_;
        ADN_TypePtr_InVector_ABC<helpers::ResourceNatureInfos> ptrResourceNature_;
        ADN_TypePtr_InVector_ABC<helpers::LogisticSupplyClass> ptrLogisticSupplyClass_;
        ADN_Type_Double rNbrInPackage_;
        ADN_Type_Double rPackageVolume_;
        ADN_Type_Double rPackageWeight_;
    };
    TYPEDEF_FULL_DECLARATION( ADN_Type_Vector_ABC<CategoryInfo>, CategoryInfos_Vector );

//*****************************************************************************
    class ModificatorPostureInfos : public ADN_Ref_ABC
                                  , public ADN_DataTreeNode_ABC
    {
    public:
        explicit ModificatorPostureInfos(const E_UnitPosture& e );

        virtual std::string GetNodeName();
        std::string GetItemName();

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;

    public:
        E_UnitPosture       eType_;
        ADN_Type_Double     rCoeff_;

    public:
        class Cmp : public std::unary_function< ModificatorPostureInfos* , bool >
        {
        public:
            Cmp(const E_UnitPosture& val) : val_(val) {}
            ~Cmp() {}

            bool operator()( ModificatorPostureInfos* tgtnfos ) const
            { return tgtnfos->eType_==val_; }

        private:
             E_UnitPosture val_;
        };
    };
    TYPEDEF_FULL_DECLARATION( ADN_Type_Vector_ABC<ModificatorPostureInfos>, ModificatorPostureInfos_Vector );

// *****************************************************************************
    class IndirectAmmoInfos : private boost::noncopyable
    {
    public:
        IndirectAmmoInfos();

        void CopyFrom( IndirectAmmoInfos& ammoInfos );

        void ReadArchive( xml::xistream& );
        void ReadPh( xml::xistream& );
        void WriteArchive( xml::xostream& ) const;
        void Initialize();

    public:
        ADN_Type_Enum<E_TypeMunitionTirIndirect,eNbrTypeMunitionTirIndirect>  nIndirectType_;
        ADN_Type_Int                                                          nIntervention_;
        ADN_Type_Double                                                       rDispersionX_;
        ADN_Type_Double                                                       rDispersionY_;

        // For explosive ammo
        ADN_Type_Double                                                       rNeutralizationRatio_;
        T_ModificatorPostureInfos_Vector                                      vModifStance_;

        // For flares
        ADN_Type_Time                                                         deployTime_;
        ADN_Type_Time                                                         flareLifeTime_;

        // For smoke
        ADN_Type_Int                                                          nMineNumber_;

        // For effect
        std::string                                                           strObjectType_;
        ADN_TypePtr_InVector_ABC< ADN_Objects_Data_ObjectInfos >              objectType_;
        ADN_Type_Time                                                         effectLifeTime_;
    };

// *****************************************************************************
    class AmmoCategoryInfo : public CategoryInfo
    {

    public:
        explicit AmmoCategoryInfo( ResourceInfos& parentDotation );

        CategoryInfo* CreateCopy();

        void ReadArchive( xml::xistream& );
        void ReadAttrition( xml::xistream& );
        void ReadUrbanModifer( xml::xistream& );
        void ReadIndirectFire( xml::xistream& );
        void WriteArchive( xml::xostream& ) const;
        bool HasUrbanAttrition() const;
        virtual void Initialize();

    public:
        ADN_Type_Enum< E_MunitionType, eNbrMunitionType > nType_;

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

    typedef ADN_Type_Vector_ABC<AmmoCategoryInfo> T_AmmoCategoryInfo_Vector;

// *****************************************************************************
    class ResourceInfos : public ADN_Ref_ABC
                        , public ADN_DataTreeNode_ABC
    {

    public:
        explicit ResourceInfos( E_DotationFamily nType );
        virtual ~ResourceInfos();

        void Reset();

        virtual std::string GetNodeName();
        std::string GetItemName();

        T_CategoryInfos_Vector& GetCategories();
        CategoryInfo* FindCategory( const std::string& strName );

        void ReadArchive( xml::xistream& );
        void WriteArchive( xml::xostream& ) const;
        void Initialize();

    public:
        E_DotationFamily      nType_;
        ADN_Type_String        strName_;
        T_CategoryInfos_Vector categories_;
    };
    TYPEDEF_WITH_ITERATOR_DECLARATION( ADN_Type_Vector_ABC<ResourceInfos>, ResourceInfos_Vector );

// *****************************************************************************
public:
             ADN_Equipement_Data();
    virtual ~ADN_Equipement_Data();

public:
    void FilesNeeded(T_StringList& files ) const;
    void Reset();

    T_ResourceInfos_Vector& GetDotations();
    ResourceInfos&          GetDotation( E_DotationFamily nType );
    CategoryInfo*           FindEquipementCategory( const std::string& strDotationName, const std::string& strCategoryName );
    CategoryInfo*           FindEquipementCategory( const std::string& strCategoryName );

    int                     GetNextCatId();
    void                    Initialize();

private:
    void ReadArchive( xml::xistream& );
    void ReadResource( xml::xistream& );
    void WriteArchive( xml::xostream& );

private:
    int nNextCatId_;

    T_ResourceInfos_Vector resources_;
};

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_Data::GetDotations
// Created: APE 2004-12-01
// -----------------------------------------------------------------------------
inline
ADN_Equipement_Data::T_ResourceInfos_Vector& ADN_Equipement_Data::GetDotations()
{
    return resources_;
}

#endif // __ADN_Equipement_Data_h_
