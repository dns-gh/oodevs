// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Crowds_Data_h_
#define __ADN_Crowds_Data_h_

#include "ADN_Data_ABC.h"
#include "ADN_CrossedRef.h"
#include "ADN_Types.h"
#include "ADN_Type_Vector_ABC.h"
#include "ADN_Type_VectorFixed_ABC.h"
#include "ADN_Type_Repartition.h"
#include "ADN_Models_Data.h"
#include "ADN_Categories_Data.h"

// =============================================================================
/** @class  ADN_Crowds_Data
*/
// Created: APE 2004-12-02
// =============================================================================
class ADN_Crowds_Data : public ADN_Data_ABC
{
public:
// *****************************************************************************
    class FireEffectProtectionInfos : public ADN_CrossedRef< helpers::ArmorInfos >
    {
    public:
        explicit FireEffectProtectionInfos( helpers::ArmorInfos* ptr );
        virtual ~FireEffectProtectionInfos();

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;

    public:
        ADN_Type_Double rUnarmedDestruction_;
        ADN_Type_Double rUnarmedFixableWithEvacuation_;
        ADN_Type_Double rUnarmedFixableWithoutEvacuation_;
        ADN_Type_Double rArmedDestruction_;
        ADN_Type_Double rArmedFixableWithEvacuation_;
        ADN_Type_Double rArmedFixableWithoutEvacuation_;

    public:
        typedef helpers::ArmorInfos T_Item;

        class CmpRef : public std::unary_function< FireEffectProtectionInfos*, bool >
        {
        public:
             CmpRef( helpers::ArmorInfos* val ) : val_( val ) {}
            ~CmpRef(){}

            bool operator()( FireEffectProtectionInfos* tgtnfos ) const
            {
                return tgtnfos->GetCrossedElement() == val_;
            }

        private:
            helpers::ArmorInfos* val_;
        };
    };
    TYPEDEF_FULL_DECLARATION( ADN_Type_VectorFixed_ABC< FireEffectProtectionInfos >, FireEffectProtectionInfosVector )

// *****************************************************************************
    class FireEffectInfos : public ADN_RefWithName
    {
    public:
        explicit FireEffectInfos( E_PopulationAttitude nAttitude );
        virtual ~FireEffectInfos();

        void ReadArchive( xml::xistream& input );
        void ReadProtection( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;

    public:
        E_PopulationAttitude nAttitude_;
        ADN_Type_Double rIntensityDensity_;
        ADN_Type_Double rIntensityFactor_;
        T_FireEffectProtectionInfosVector vProtectionInfos_;

    public:
        class CmpAttitude : public std::unary_function< FireEffectInfos*, bool >
        {
        public:
            CmpAttitude( E_PopulationAttitude val ) : val_( val ) {}
            ~CmpAttitude() {}

            bool operator()( FireEffectInfos* tgtnfos ) const
            {
                return tgtnfos->nAttitude_ == val_;
            }

        private:
            E_PopulationAttitude val_;
        };
    };
    TYPEDEF_FULL_DECLARATION( ADN_Type_Vector_ABC< FireEffectInfos >, FireEffectInfosVector )

// *****************************************************************************
    class FireEffectRoeInfos : public ADN_RefWithName
    {
    public:
        explicit FireEffectRoeInfos( E_PopulationRoe nRoe );
        virtual ~FireEffectRoeInfos();

        unsigned int GetRoe();

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;

    public:
        E_PopulationRoe nRoe_;
        ADN_Type_Double rAttritionSurface_;
        ADN_Type_Double rPH_;
    };
    TYPEDEF_FULL_DECLARATION( ADN_Type_Vector_ABC< FireEffectRoeInfos >, FireEffectRoeInfosVector )

// *****************************************************************************
    class SpeedEffectVolumeInfos : public ADN_CrossedRef< ADN_Categories_Data::SizeInfos >
    {
    public:
        explicit SpeedEffectVolumeInfos( ADN_Categories_Data::SizeInfos* ptr );
        virtual ~SpeedEffectVolumeInfos();

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;

    public:
        ADN_Type_Double rDensity_;
        ADN_Type_Double rMaxSpeed_;

    public:
        typedef ADN_Categories_Data::SizeInfos T_Item;

        class CmpRef : public std::unary_function< SpeedEffectVolumeInfos*, bool >
        {
        public:
             CmpRef(ADN_Categories_Data::SizeInfos* val) : val_( val ) {}
            ~CmpRef(){}

            bool operator()( SpeedEffectVolumeInfos* tgtnfos ) const
            {
                return tgtnfos->GetCrossedElement() == val_;
            }

        private:
            ADN_Categories_Data::SizeInfos* val_;
        };
    };
    TYPEDEF_FULL_DECLARATION( ADN_Type_VectorFixed_ABC< SpeedEffectVolumeInfos >, SpeedEffectVolumeInfosVector )

// *****************************************************************************
    class SpeedEffectInfos : public ADN_RefWithName
    {
    public:
        explicit SpeedEffectInfos( E_PopulationAttitude nAttitude );
        virtual ~SpeedEffectInfos();

        void ReadArchive( xml::xistream& input );
        void ReadSpeedEffect( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;

    public:
        E_PopulationAttitude nAttitude_;
        T_SpeedEffectVolumeInfosVector vVolumeInfos_;
    };
    TYPEDEF_FULL_DECLARATION( ADN_Type_Vector_ABC< SpeedEffectInfos >, SpeedEffectInfosVector )

// *****************************************************************************
    class UrbanEffectInfos : public ADN_RefWithName
    {
    public:
        explicit UrbanEffectInfos( E_PopulationAttitude nAttitude );
        virtual ~UrbanEffectInfos();

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;

    public:
        E_PopulationAttitude nAttitude_;
        ADN_Type_Double rDensity_;
        ADN_Type_Time rTime_;
    };
    TYPEDEF_FULL_DECLARATION( ADN_Type_Vector_ABC< UrbanEffectInfos >, UrbanEffectInfosVector )

// *****************************************************************************
    class CrowdsInfos : public ADN_RefWithName
    {
    public:
                 CrowdsInfos();
                 CrowdsInfos( unsigned int id );
        virtual ~CrowdsInfos();

        void Initialize();
        CrowdsInfos* CreateCopy();
        void ReadArchive( xml::xistream& input );
        void ReadSlowingEffect( xml::xistream& input );
        void ReadAttritionEffect( xml::xistream& input );
        void ReadFireEffect( xml::xistream& input );
        void ReadUrbanEffect( xml::xistream& input );
        void WriteArchive( xml::xostream& output );
        using ADN_RefWithName::CheckValidity;
        virtual void CheckValidity( ADN_ConsistencyChecker& checker, const std::string& name, int tab, int subTab = -1, const std::string& optional = "" );

    public:
        ADN_Type_Int nId_;
        ADN_TypePtr_InVector_ABC< ADN_Models_Data::ModelInfos > ptrModel_;
        ADN_Type_Double rConcentrationDensity_;
        ADN_Type_Double rMoveDensity_;
        ADN_Type_Double rMoveSpeed_;
        ADN_Type_Repartition repartition_;
        ADN_Type_Int armedIndividuals_;
        ADN_Type_Time decontaminationDelay_;
        T_SpeedEffectInfosVector vSpeedEffectInfos_;
        T_FireEffectInfosVector vFireEffectInfos_;
        T_FireEffectRoeInfosVector vFireEffectRoeInfos_;
        T_UrbanEffectInfosVector vUrbanEffectInfos_;
    };
    TYPEDEF_FULL_DECLARATION( ADN_Type_Vector_ABC< CrowdsInfos >, CrowdsInfosVector )

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
             ADN_Crowds_Data();
    virtual ~ADN_Crowds_Data();

    void FilesNeeded( tools::Path::T_Paths& l ) const;
    void Reset();

    T_CrowdsInfosVector& GetCrowds();
    CrowdsInfos* FindCrowd( const std::string& strName );
    QStringList GetCrowdsThatUse( ADN_Models_Data::ModelInfos& model );
    virtual void CheckDatabaseValidity( ADN_ConsistencyChecker& checker ) const;

private:
    void ReadArchive( xml::xistream& input );
    void ReadPopulation( xml::xistream& input );
    void WriteArchive( xml::xostream& output );

public:
    T_CrowdsInfosVector vCrowds_;
    ReloadingSpeedEffectInfos reloadingSpeedEffectInfos_;
    ADN_Type_Time timeBetweenNbcApplication_;

private:
    static tools::IdManager idManager_;
};

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_Data::GetCrowds
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
inline
ADN_Crowds_Data::T_CrowdsInfosVector& ADN_Crowds_Data::GetCrowds()
{
    return vCrowds_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Crowds_Data::FindCrowd
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
inline
ADN_Crowds_Data::CrowdsInfos* ADN_Crowds_Data::FindCrowd( const std::string& strName )
{
    IT_CrowdsInfosVector it = std::find_if( vCrowds_.begin(), vCrowds_.end(), ADN_Tools::NameCmp< CrowdsInfos >( strName ) );
    if( it == vCrowds_.end() )
        return 0;
    return *it;
}

#endif // __ADN_Crowds_Data_h_
