// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Population_Data_h_
#define __ADN_Population_Data_h_

#include "ADN_Data_ABC.h"
#include "ADN_Types.h"
#include "ADN_Type_Vector_ABC.h"
#include "ADN_Type_VectorFixed_ABC.h"
#include "ADN_Type_Repartition.h"
#include "ADN_Models_Data.h"
#include "ADN_Categories_Data.h"

namespace xml { class xistream; }

// =============================================================================
/** @class  ADN_Population_Data
*/
// Created: APE 2004-12-02
// =============================================================================
class ADN_Population_Data : public ADN_Data_ABC
{

public:
// *****************************************************************************
    class FireEffectProtectionInfos : public ADN_Ref_ABC
                                    , public ADN_DataTreeNode_ABC
    {
    public:
        explicit FireEffectProtectionInfos( helpers::ArmorInfos* ptr );
        virtual ~FireEffectProtectionInfos();

        virtual std::string GetNodeName();
        std::string GetItemName();

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;

    public:
        ADN_Type_String                                           strName_;
        ADN_TypePtr_InVector_ABC<helpers::ArmorInfos> ptrProtection_;
        ADN_Type_Double                                           rUnarmedDestruction_;
        ADN_Type_Double                                           rUnarmedFixableWithEvacuation_;
        ADN_Type_Double                                           rUnarmedFixableWithoutEvacuation_;
        ADN_Type_Double                                           rArmedDestruction_;
        ADN_Type_Double                                           rArmedFixableWithEvacuation_;
        ADN_Type_Double                                           rArmedFixableWithoutEvacuation_;

    public:
        typedef helpers::ArmorInfos T_Item;

        class CmpRef : public std::unary_function< FireEffectProtectionInfos* , bool >
        {
        public:
            CmpRef(helpers::ArmorInfos* val) : val_(val) {}
            ~CmpRef(){}

            bool operator()( FireEffectProtectionInfos* tgtnfos ) const
            {   return tgtnfos->ptrProtection_.GetData() == val_;}

        private:
            helpers::ArmorInfos* val_;
        };
    };
    TYPEDEF_FULL_DECLARATION( ADN_Type_VectorFixed_ABC<FireEffectProtectionInfos>, FireEffectProtectionInfosVector )

// *****************************************************************************
    class FireEffectInfos : public ADN_Ref_ABC
                          , public ADN_DataTreeNode_ABC
    {
    public:
        explicit FireEffectInfos( E_PopulationAttitude nAttitude );
        virtual ~FireEffectInfos();

        virtual std::string GetNodeName();
        std::string GetItemName();

        void ReadArchive( xml::xistream& input );
        void ReadProtection( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;

    public:
        ADN_Type_String                   strName_;
        E_PopulationAttitude              nAttitude_;
        ADN_Type_Double                   rIntensityDensity_;
        ADN_Type_Double                   rIntensityFactor_;
        T_FireEffectProtectionInfosVector vProtectionInfos_;

    public:
        class CmpAttitude : public std::unary_function< FireEffectInfos* , bool >
        {
        public:
            CmpAttitude(E_PopulationAttitude val) : val_(val) {}
            ~CmpAttitude(){}

            bool operator()( FireEffectInfos* tgtnfos ) const
            {   return tgtnfos->nAttitude_ == val_;}

        private:
            E_PopulationAttitude val_;
        };
    };
    TYPEDEF_FULL_DECLARATION( ADN_Type_Vector_ABC<FireEffectInfos>, FireEffectInfosVector )

// *****************************************************************************
    class FireEffectRoeInfos : public ADN_Ref_ABC
                             , public ADN_DataTreeNode_ABC
    {
    public:
        explicit FireEffectRoeInfos( E_PopulationRoe nRoe );
        virtual ~FireEffectRoeInfos();

        virtual std::string GetNodeName();
        std::string GetItemName();
        unsigned int GetRoe();

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;

    public:
        ADN_Type_String strName_;
        E_PopulationRoe nRoe_;
        ADN_Type_Double rAttritionSurface_;
        ADN_Type_Double rPH_;
    };
    TYPEDEF_FULL_DECLARATION( ADN_Type_Vector_ABC<FireEffectRoeInfos>, FireEffectRoeInfosVector )

// *****************************************************************************
    class SpeedEffectVolumeInfos : public ADN_Ref_ABC
                                 , public ADN_DataTreeNode_ABC
    {
    public:
        explicit SpeedEffectVolumeInfos( ADN_Categories_Data::SizeInfos* ptr );
        virtual ~SpeedEffectVolumeInfos();

        virtual std::string GetNodeName();
        std::string GetItemName();

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;

    public:
        ADN_Type_String                                          strName_;
        ADN_TypePtr_InVector_ABC<ADN_Categories_Data::SizeInfos> ptrVolume_;
        ADN_Type_Double                                          rDensity_;
        ADN_Type_Double                                          rMaxSpeed_;

    public:
        typedef ADN_Categories_Data::SizeInfos T_Item;

        class CmpRef : public std::unary_function< SpeedEffectVolumeInfos* , bool >
        {
        public:
            CmpRef(ADN_Categories_Data::SizeInfos* val) : val_(val) {}
            ~CmpRef(){}

            bool operator()( SpeedEffectVolumeInfos* tgtnfos ) const
            {   return tgtnfos->ptrVolume_.GetData() == val_;}

        private:
            ADN_Categories_Data::SizeInfos* val_;
        };
    };
    TYPEDEF_FULL_DECLARATION( ADN_Type_VectorFixed_ABC<SpeedEffectVolumeInfos>, SpeedEffectVolumeInfosVector )

// *****************************************************************************
    class SpeedEffectInfos : public ADN_Ref_ABC
                           , public ADN_DataTreeNode_ABC
    {
    public:
        explicit SpeedEffectInfos( E_PopulationAttitude nAttitude );
        virtual ~SpeedEffectInfos();

        virtual std::string GetNodeName();
        std::string GetItemName();

        void ReadArchive( xml::xistream& input );
        void ReadSpeedEffect( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;

    public:
        E_PopulationAttitude           nAttitude_;
        ADN_Type_String                strName_;
        T_SpeedEffectVolumeInfosVector vVolumeInfos_;
    };
    TYPEDEF_FULL_DECLARATION( ADN_Type_Vector_ABC<SpeedEffectInfos>, SpeedEffectInfosVector )

// *****************************************************************************
    class UrbanEffectInfos : public ADN_Ref_ABC
                            , public ADN_DataTreeNode_ABC
    {
    public:
        explicit UrbanEffectInfos( E_PopulationAttitude nAttitude );
        virtual ~UrbanEffectInfos();

        virtual std::string GetNodeName();
        std::string GetItemName();

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;

    public:
        E_PopulationAttitude    nAttitude_;
        ADN_Type_String         strName_;
        ADN_Type_Double         rDensity_;
        ADN_Type_Time           rTime_;
    };
    TYPEDEF_FULL_DECLARATION( ADN_Type_Vector_ABC<UrbanEffectInfos>, UrbanEffectInfosVector )

// *****************************************************************************
    class PopulationInfos : public ADN_Ref_ABC
                          , public ADN_DataTreeNode_ABC
    {
    public:
                 PopulationInfos();
        virtual ~PopulationInfos();

        virtual std::string GetNodeName();
        std::string GetItemName();

        PopulationInfos* CreateCopy();

        void ReadArchive( xml::xistream& input );
        void ReadSlowingEffect( xml::xistream& input );
        void ReadAttritionEffect( xml::xistream& input );
        void ReadFireEffect( xml::xistream& input );
        void ReadUrbanEffect( xml::xistream& input );
        void WriteArchive( xml::xostream& output, int nMosId ) const;

    public:
        ADN_Type_String                                       strName_;
        ADN_TypePtr_InVector_ABC<ADN_Models_Data::ModelInfos> ptrModel_;
        ADN_Type_Double                                       rConcentrationDensity_;
        ADN_Type_Double                                       rMoveDensity_;
        ADN_Type_Double                                       rMoveSpeed_;
        ADN_Type_Repartition                                  repartition_;
        ADN_Type_Int                                          armedIndividuals_;

        T_SpeedEffectInfosVector                              vSpeedEffectInfos_;
        T_FireEffectInfosVector                               vFireEffectInfos_;
        T_FireEffectRoeInfosVector                            vFireEffectRoeInfos_;
        T_UrbanEffectInfosVector                              vUrbanEffectInfos_;
    };
    TYPEDEF_FULL_DECLARATION( ADN_Type_Vector_ABC<PopulationInfos>, PopulationInfosVector )

// *****************************************************************************
    class ReloadingSpeedEffectInfos : public ADN_Ref_ABC
    {
    public:
                 ReloadingSpeedEffectInfos();
        virtual ~ReloadingSpeedEffectInfos();

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;

    public:
        ADN_Type_Double rDensity_;
        ADN_Type_Double rModifier_;
    };

// *****************************************************************************
public:
             ADN_Population_Data();
    virtual ~ADN_Population_Data();

    void FilesNeeded( T_StringList& l ) const;
    void Reset();

    T_PopulationInfosVector& GetPopulation();
    PopulationInfos* FindPopulation( const std::string& strName );
    QStringList GetPopulationsThatUse( ADN_Models_Data::ModelInfos& model );

private:
    void ReadArchive( xml::xistream& input );
    void ReadPopulation( xml::xistream& input );
    void WriteArchive( xml::xostream& output );

public:
    T_PopulationInfosVector   vPopulation_;
    ReloadingSpeedEffectInfos reloadingSpeedEffectInfos_;
    ADN_Type_Time timeBetweenNbcApplication_;
    ADN_Type_Time decontaminationDelay_;
};

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::GetPopulation
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
inline
ADN_Population_Data::T_PopulationInfosVector& ADN_Population_Data::GetPopulation()
{
    return vPopulation_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::FindPopulation
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
inline
ADN_Population_Data::PopulationInfos* ADN_Population_Data::FindPopulation( const std::string& strName )
{
    IT_PopulationInfosVector it = std::find_if( vPopulation_.begin(), vPopulation_.end(), ADN_Tools::NameCmp<PopulationInfos>( strName ) );
    if( it == vPopulation_.end() )
        return 0;
    return *it;
}

#endif // __ADN_Population_Data_h_
