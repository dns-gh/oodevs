// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-11-15 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Equipement_Data.h $
// $Author: Ape $
// $Modtime: 21/04/05 11:03 $
// $Revision: 12 $
// $Workfile: ADN_Equipement_Data.h $
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

class ADN_XmlInput_Helper;


// =============================================================================
/** @class  ADN_Equipement_Data
*/
// Created: APE 2004-11-15
// =============================================================================
class ADN_Equipement_Data : public ADN_Data_ABC
{
    friend class ADN_Equipement;
    MT_COPYNOTALLOWED( ADN_Equipement_Data );

public:
    class DotationInfos;

// *****************************************************************************
    class CategoryInfo
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( CategoryInfo );

    public:
        CategoryInfo(); // For template usage. 
        CategoryInfo( DotationInfos& parentDotation );

        virtual std::string GetNodeName();
        std::string GetItemName();

        virtual CategoryInfo* CreateCopy();

        virtual void ReadArchive( ADN_XmlInput_Helper& );
        virtual void WriteArchive( MT_OutputArchive_ABC& );

    public:
        DotationInfos&  parentDotation_;
        ADN_Type_String strName_;
        int             nMosId_;
        ADN_Type_String strCodeEMAT6_; 
        ADN_Type_String strCodeEMAT8_;
        ADN_Type_String strCodeLFRIL_;
        ADN_Type_String strCodeNNO_;
        ADN_Type_Double rNbrInUnitVolume_;
    };

    typedef ADN_Type_Vector_ABC<CategoryInfo> T_CategoryInfos_Vector;
    typedef T_CategoryInfos_Vector::iterator  IT_CategoryInfos_Vector;


// *****************************************************************************
    class AttritionInfos
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( AttritionInfos );

    public:
        AttritionInfos(ADN_Categories_Data::ArmorInfos* ptr);

        virtual std::string GetNodeName();
        std::string GetItemName();

        void CopyFrom( AttritionInfos& attritions );

        void ReadArchive( ADN_XmlInput_Helper& );
        void WriteArchive( MT_OutputArchive_ABC& );

    public:
        ADN_TypePtr_InVector_ABC<ADN_Categories_Data::ArmorInfos>   ptrArmor_;
        ADN_Type_Double                                             rDestroy_;
        ADN_Type_Double                                             rRepairWithEvac_;
        ADN_Type_Double                                             rRepairNoEvac_;

    public:
        typedef ADN_Categories_Data::ArmorInfos     T_Item;

        class CmpRef : public std::unary_function< AttritionInfos* , bool >
        {
        public:
            CmpRef(ADN_Categories_Data::ArmorInfos* val) : val_(val) {}
            ~CmpRef(){}

            bool operator()( AttritionInfos* tgtnfos ) const 
            {   return tgtnfos->ptrArmor_.GetData() == val_;}

        private:
            ADN_Categories_Data::ArmorInfos* val_;
        };
    };

    typedef ADN_Type_VectorFixed_ABC<AttritionInfos> T_AttritionInfos_Vector;
    typedef T_AttritionInfos_Vector::iterator        IT_AttritionInfos_Vector;


// *****************************************************************************
    class IndirectAmmoInfos
    {
        MT_COPYNOTALLOWED( IndirectAmmoInfos );

    public:
        IndirectAmmoInfos();

        void CopyFrom( IndirectAmmoInfos& ammoInfos );

        void ReadArchive( ADN_XmlInput_Helper& );
        void WriteArchive( MT_OutputArchive_ABC& );

    public:
        ADN_Type_Enum<E_TypeMunitionTirIndirect,eNbrTypeMunitionTirIndirect>  nIndirectType_;
        ADN_Type_Int                                                    nIntervention_;
        ADN_Type_Double                                                 rDispersionX_;
        ADN_Type_Double                                                 rDispersionY_;

        // For explosive ammo
        ADN_Type_Double                                                 rNeutralizationRatio_;

        // For flares
        ADN_Type_Double                                                 rDeployTime_;
        ADN_Type_Double                                                 rLifeTime_;

        // For smoke
        ADN_Type_Int                                                    nMineNumber_;
    };


// *****************************************************************************
    class AmmoCategoryInfo
        : public CategoryInfo
    {
        MT_COPYNOTALLOWED( AmmoCategoryInfo );

    public:
        AmmoCategoryInfo( DotationInfos& parentDotation );

        CategoryInfo* CreateCopy();

        void ReadArchive( ADN_XmlInput_Helper& );
        void WriteArchive( MT_OutputArchive_ABC& );

    public:
        ADN_Type_Enum< E_MunitionType, eNbrMunitionType > nType_;

        ADN_Type_Bool bTrancheD_;

        ADN_Type_Bool bDirect_;
        ADN_Type_Bool bIndirect_;

        T_AttritionInfos_Vector attritions_;
        IndirectAmmoInfos indirectAmmoInfos_;
    };

    typedef ADN_Type_Vector_ABC<AmmoCategoryInfo> T_AmmoCategoryInfo_Vector;

// *****************************************************************************
    class DotationInfos
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( DotationInfos );
    
    public:
         DotationInfos( E_TypeDotation nType );
        ~DotationInfos();

        void Reset();

        virtual std::string GetNodeName();
        std::string GetItemName();

        T_CategoryInfos_Vector& GetCategories();
        CategoryInfo* FindCategory( const std::string& strName );
        
        void ReadArchive( ADN_XmlInput_Helper& );
        void WriteArchive( MT_OutputArchive_ABC& );

    public:
        E_TypeDotation          nType_;
        ADN_Type_String         strName_;
        T_CategoryInfos_Vector  categories_; 
    };

    typedef ADN_Type_Vector_ABC<DotationInfos> T_DotationInfos_Vector;
    typedef T_DotationInfos_Vector::iterator   IT_DotationInfos_Vector;


// *****************************************************************************
public:
    ADN_Equipement_Data();
    virtual ~ADN_Equipement_Data();

public:
    void FilesNeeded(T_StringList& files ) const;
    void Reset();

    T_DotationInfos_Vector& GetDotations();
    DotationInfos&          GetDotation( E_TypeDotation nType );
    CategoryInfo*           FindEquipementCategory( const std::string& strDotationName, const std::string& strCategoryName );

    int                     GetNextCatId();

private:
    void ReadArchive( ADN_XmlInput_Helper& );
    void WriteArchive( MT_OutputArchive_ABC& );

private:
    int nNextCatId_;

    T_DotationInfos_Vector dotations_;
};


// -----------------------------------------------------------------------------
// Name: ADN_Equipement_Data::GetDotations
// Created: APE 2004-12-01
// -----------------------------------------------------------------------------
inline
ADN_Equipement_Data::T_DotationInfos_Vector& ADN_Equipement_Data::GetDotations()
{
    return dotations_;
}


#endif // __ADN_Equipement_Data_h_
