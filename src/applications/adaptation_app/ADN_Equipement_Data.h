// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-11-15 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Equipement_Data.h $
// $Author: Nld $
// $Modtime: 20/07/05 14:11 $
// $Revision: 13 $
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
#include "ADN_Urban_Data.h"

class xml::xistream;


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

        virtual void ReadArchive( xml::xistream& );
        virtual void WriteArchive( xml::xostream& );
        virtual void WriteContent( xml::xostream& );

    public:
        DotationInfos&  parentDotation_;
        ADN_Type_String strName_;
        int             nMosId_;
        ADN_Type_String strCodeEMAT6_; 
        ADN_Type_String strCodeEMAT8_;
        ADN_Type_String strCodeLFRIL_;
        ADN_Type_String strCodeNNO_;
        ADN_TypePtr_InVector_ABC<ADN_Categories_Data::DotationNatureInfos> ptrDotationNature_;
        ADN_Type_Double rNbrInPackage_;
        ADN_Type_Double rPackageVolume_;
        ADN_Type_Double rPackageWeight_;
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

        void ReadArchive( xml::xistream& );
        void WriteArchive( xml::xostream&, const std::string& tag = "attrition" );

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
            bool operator()( AttritionInfos* tgtnfos ) const 
            {   return tgtnfos->ptrArmor_.GetData() == val_;}

        private:
            ADN_Categories_Data::ArmorInfos* val_;
        };

        class Cmp : public std::unary_function< AttritionInfos* , bool >
        {
        public:
            Cmp( const std::string& name ) : name_(name) {}
            bool operator()( AttritionInfos* tgtnfos ) const 
            {   return tgtnfos->ptrArmor_.GetData()->strName_ == name_;}

        private:
            std::string name_;
        };
    };

    typedef ADN_Type_VectorFixed_ABC<AttritionInfos> T_AttritionInfos_Vector;
    typedef T_AttritionInfos_Vector::iterator        IT_AttritionInfos_Vector;

// *****************************************************************************
    class UrbanAttritionInfos
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( UrbanAttritionInfos );

    public:
        UrbanAttritionInfos(ADN_Urban_Data::UrbanInfos* ptr);

        virtual std::string GetNodeName();
        std::string GetItemName();

        void CopyFrom( UrbanAttritionInfos& attritions );

        void ReadArchive( xml::xistream& );
        void WriteArchive( xml::xostream&, const std::string& tag = "urbanModifier" );

    public:
        ADN_TypePtr_InVector_ABC<ADN_Urban_Data::UrbanInfos>        ptrMaterial_;
        ADN_Type_Double                                             rCoeff_;
        ADN_Type_String&                                            strName_;

    public:
        typedef ADN_Urban_Data::UrbanInfos                  T_Item;

        class CmpRef : public std::unary_function< UrbanAttritionInfos* , bool >
        {
        public:
            CmpRef(ADN_Urban_Data::UrbanInfos* val) : val_(val) {}
            bool operator()( UrbanAttritionInfos* tgtnfos ) const 
            {   return tgtnfos->ptrMaterial_.GetData() == val_;}

        private:
            ADN_Urban_Data::UrbanInfos* val_;
        };

        class Cmp : public std::unary_function< UrbanAttritionInfos* , bool >
        {
        public:
            Cmp(const std::string& val) : val_(val) {}
            ~Cmp() {}

            bool operator()( UrbanAttritionInfos* tgtnfos ) const 
            { return tgtnfos->ptrMaterial_.GetData() && tgtnfos->ptrMaterial_.GetData()->GetData()==val_; }

        private:
            std::string val_;
        };
    };

    typedef ADN_Type_VectorFixed_ABC< UrbanAttritionInfos > T_UrbanAttritionInfos_Vector;
    typedef T_UrbanAttritionInfos_Vector::iterator         IT_UrbanAttritionInfos_Vector;

//*****************************************************************************
    class ModificatorPostureInfos
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( ModificatorPostureInfos )

    public:
        ModificatorPostureInfos(const E_UnitPosture& e );

        virtual std::string GetNodeName();
        std::string GetItemName();

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output );

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

    typedef ADN_Type_Vector_ABC<ModificatorPostureInfos>     T_ModificatorPostureInfos_Vector;
    typedef T_ModificatorPostureInfos_Vector::iterator       IT_ModificatorPostureInfos_Vector;
    typedef T_ModificatorPostureInfos_Vector::const_iterator CIT_ModificatorPostureInfos_Vector;


// *****************************************************************************
    class IndirectAmmoInfos
    {
        MT_COPYNOTALLOWED( IndirectAmmoInfos );

    public:
        IndirectAmmoInfos();

        void CopyFrom( IndirectAmmoInfos& ammoInfos );

        void ReadArchive( xml::xistream& );
        void ReadPh( xml::xistream& );
        void WriteArchive( xml::xostream& );

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

        // For smoke
        ADN_Type_Int                                                          nMineNumber_;

        //Common for effect or flares
        ADN_Type_Time                                                         lifeTime_;

        // For effect
        ADN_Type_String                                                       objectType_;
    };

// *****************************************************************************
    class AmmoCategoryInfo
        : public CategoryInfo
    {
        MT_COPYNOTALLOWED( AmmoCategoryInfo );

    public:
        AmmoCategoryInfo( DotationInfos& parentDotation );

        CategoryInfo* CreateCopy();

        void ReadArchive( xml::xistream& );
        void ReadAttrition( xml::xistream& );
        void ReadUrbanModifer( xml::xistream& );
        void ReadIndirectFire( xml::xistream& );
        void WriteArchive( xml::xostream& );

    public:
        ADN_Type_Enum< E_MunitionType, eNbrMunitionType > nType_;

        ADN_Type_Bool bTrancheD_;

        ADN_Type_Bool bDirect_;
        ADN_Type_Bool bUrbanAttrition_;
        ADN_Type_Bool bIndirect_;

        T_AttritionInfos_Vector         attritions_;
        T_UrbanAttritionInfos_Vector    modifUrbanBlocks_;
        ADN_Type_Bool bIlluminating_;
        ADN_Type_Bool bMaintainIllumination_;
        ADN_Type_Double fRange_;

        IndirectAmmoInfos indirectAmmoInfos_;

        ADN_Type_Bool bGuided_;
        ADN_Type_Bool bMaintainGuidance_;
        ADN_Type_Double fGuidanceRange_;
    };

    typedef ADN_Type_Vector_ABC<AmmoCategoryInfo> T_AmmoCategoryInfo_Vector;

// *****************************************************************************
    class DotationInfos
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( DotationInfos );
    
    public:
         DotationInfos( E_DotationFamily nType );
        ~DotationInfos();

        void Reset();

        virtual std::string GetNodeName();
        std::string GetItemName();

        T_CategoryInfos_Vector& GetCategories();
        CategoryInfo* FindCategory( const std::string& strName );
        
        void ReadArchive( xml::xistream& );
        void WriteArchive( xml::xostream& );

    public:
        E_DotationFamily      nType_;
        ADN_Type_String        strName_;
        T_CategoryInfos_Vector categories_;
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
    DotationInfos&          GetDotation( E_DotationFamily nType );
    CategoryInfo*           FindEquipementCategory( const std::string& strDotationName, const std::string& strCategoryName );
    CategoryInfo*           FindEquipementCategory( const std::string& strCategoryName );

    int                     GetNextCatId();

private:
    void ReadArchive( xml::xistream& );
    void ReadDotation( xml::xistream& );
    void WriteArchive( xml::xostream& );

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
