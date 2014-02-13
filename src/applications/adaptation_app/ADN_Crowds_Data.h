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
#include "ADN_Armors_Data.h"
#include "ADN_Volumes_Data.h"

// =============================================================================
/** @class  ADN_Crowds_Data
*/
// Created: APE 2004-12-02
// =============================================================================
class ADN_Crowds_Data : public ADN_Data_ABC
{
public:
// *****************************************************************************
    class SpeedInfos : public ADN_Ref_ABC
    {
    public:
        explicit SpeedInfos( E_Location nTypeTerrain );
        virtual ~SpeedInfos() {}

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;

    public:
        E_Location nTypeTerrain_;
        ADN_Type_Double rSpeed_;

    public:
        class Cmp : public std::unary_function< SpeedInfos*, bool >
        {
        public:
            Cmp( E_Location& val ) : val_( val ) {}
            ~Cmp() {}

            bool operator()( SpeedInfos* tgtnfos ) const
            {
                return tgtnfos->nTypeTerrain_ == val_;
            }

        private:
            E_Location val_;
        };
    };

    typedef ADN_Type_Vector_ABC< SpeedInfos > T_SpeedInfos_Vector;

    // *****************************************************************************
    class FireEffectProtectionInfos : public ADN_CrossedRef< ADN_Armors_Data::ArmorInfos >
    {
    public:
        explicit FireEffectProtectionInfos( ADN_Armors_Data::ArmorInfos* ptr );
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
        typedef ADN_Armors_Data::ArmorInfos T_Item;

        class CmpRef : public std::unary_function< FireEffectProtectionInfos*, bool >
        {
        public:
             CmpRef( ADN_Armors_Data::ArmorInfos* val ) : val_( val ) {}
            ~CmpRef(){}

            bool operator()( FireEffectProtectionInfos* tgtnfos ) const
            {
                return tgtnfos->GetCrossedElement() == val_;
            }

        private:
            ADN_Armors_Data::ArmorInfos* val_;
        };
    };
    typedef ADN_Type_VectorFixed_ABC< FireEffectProtectionInfos > T_FireEffectProtectionInfosVector;

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
    typedef ADN_Type_Vector_ABC< FireEffectInfos > T_FireEffectInfosVector;

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
    typedef ADN_Type_Vector_ABC< FireEffectRoeInfos > T_FireEffectRoeInfosVector;

// *****************************************************************************
    class SpeedEffectVolumeInfos : public ADN_CrossedRef< ADN_Volumes_Data::VolumeInfos >
    {
    public:
        explicit SpeedEffectVolumeInfos( ADN_Volumes_Data::VolumeInfos* ptr );
        virtual ~SpeedEffectVolumeInfos();

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;

    public:
        ADN_Type_Double rDensity_;
        ADN_Type_Double rMaxSpeed_;

    public:
        typedef ADN_Volumes_Data::VolumeInfos T_Item;

        class CmpRef : public std::unary_function< SpeedEffectVolumeInfos*, bool >
        {
        public:
             CmpRef(ADN_Volumes_Data::VolumeInfos* val) : val_( val ) {}
            ~CmpRef(){}

            bool operator()( SpeedEffectVolumeInfos* tgtnfos ) const
            {
                return tgtnfos->GetCrossedElement() == val_;
            }

        private:
            ADN_Volumes_Data::VolumeInfos* val_;
        };
    };
    typedef ADN_Type_VectorFixed_ABC< SpeedEffectVolumeInfos > T_SpeedEffectVolumeInfosVector;

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
    typedef ADN_Type_Vector_ABC< SpeedEffectInfos > T_SpeedEffectInfosVector;

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
    typedef ADN_Type_Vector_ABC< UrbanEffectInfos > T_UrbanEffectInfosVector;

// *****************************************************************************
    class CrowdsInfos : public ADN_RefWithLocalizedName
    {
    public:
                 CrowdsInfos();
                 CrowdsInfos( unsigned int id );
        virtual ~CrowdsInfos();

        void Initialize();
        CrowdsInfos* CreateCopy();
        void ReadArchive( xml::xistream& input );
        void ReadSpeed( xml::xistream& input );
        void ReadSlowingEffect( xml::xistream& input );
        void ReadAttritionEffect( xml::xistream& input );
        void ReadFireEffect( xml::xistream& input );
        void ReadUrbanEffect( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;
        using ADN_RefWithLocalizedName::CheckValidity;
        virtual void CheckValidity( ADN_ConsistencyChecker& checker, const std::string& name, int tab, int subTab = -1, const std::string& optional = "" );

    public:
        ADN_Type_Int nId_;
        ADN_TypePtr_InVector_ABC< ADN_Models_Data::ModelInfos > ptrModel_;
        ADN_Type_Bool bCrowdCollision_;
        ADN_Type_Double rConcentrationDensity_;
        ADN_Type_Double rMoveDensity_;
        ADN_Type_Double rMoveSpeed_;
        ADN_Type_Repartition repartition_;
        ADN_Type_Int armedIndividuals_;
        ADN_Type_Time decontaminationDelay_;
        ADN_Type_Bool bHasSpeeds_;
        T_SpeedInfos_Vector vSpeedInfos_;
        T_SpeedEffectInfosVector vSpeedEffectInfos_;
        T_FireEffectInfosVector vFireEffectInfos_;
        T_FireEffectRoeInfosVector vFireEffectRoeInfos_;
        T_UrbanEffectInfosVector vUrbanEffectInfos_;
    };
    typedef ADN_Type_Vector_ABC< CrowdsInfos > T_CrowdsInfosVector;

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

    T_CrowdsInfosVector& GetCrowds();
    CrowdsInfos* FindCrowd( const std::string& strName );
    QStringList GetCrowdsThatUse( ADN_Models_Data::ModelInfos& model );
    virtual void CheckDatabaseValidity( ADN_ConsistencyChecker& checker ) const;

private:
    void ReadArchive( xml::xistream& input );
    void ReadPopulation( xml::xistream& input );
    void WriteArchive( xml::xostream& output ) const;

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
    auto it = std::find_if( vCrowds_.begin(), vCrowds_.end(), ADN_Tools::NameCmp( strName ) );
    if( it == vCrowds_.end() )
        return 0;
    return *it;
}

#endif // __ADN_Crowds_Data_h_
