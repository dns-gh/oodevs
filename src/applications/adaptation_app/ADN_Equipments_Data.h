// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Equipments_Data_h_
#define __ADN_Equipments_Data_h_

#include "ADN_Data_ABC.h"
#include "ADN_CrossedRef.h"
#include "ADN_Types.h"
#include "ADN_Enums.h"
#include "ADN_Categories_Data.h"
#include "ADN_Sensors_Data.h"
#include "ADN_Launchers_Data.h"
#include "ADN_Objects_Data_ObjectInfos.h"
#include "ADN_Resources_Data.h"
#include "ADN_Weapons_Data_WeaponInfos.h"
#include "ADN_ActiveProtections_Data.h"
#include "ADN_Natures_Data.h"
#include "ADN_Breakdowns_Data.h"
#include "ADN_Radars_Data.h"
#include "ADN_Tr.h"

// =============================================================================
/** @class  ADN_Equipments_Data
    @brief  ADN_Equipments_Data
*/
// Created: APE 2004-12-07
// =============================================================================
class ADN_Equipments_Data : public ADN_Data_ABC
{
public:
    //*****************************************************************************
    class AmbulanceInfos : public ADN_Ref_ABC
    {
    public:
                 AmbulanceInfos();
        virtual ~AmbulanceInfos() {}

        void CopyFrom( AmbulanceInfos& src );

        void ReadArchive( xml::xistream& input );
        void WriteArchive( const std::string& section, xml::xostream& output ) const;

    public:
        ADN_Type_Bool transportSkills_[ eNbrDoctorSkills ];
        ADN_Type_Bool bTransportNBC_;
        ADN_Type_Bool bTransportShock_;
        ADN_Type_Int nCapacity_;
        ADN_Type_Time loadTimePerPerson_;
        ADN_Type_Time unloadTimePerPerson_;
    };

    //*****************************************************************************
    class LogHealthInfos : public ADN_Ref_ABC
    {
    public:
                 LogHealthInfos();
        virtual ~LogHealthInfos() {}

        void CopyFrom( LogHealthInfos& src );

        void ReadArchive( xml::xistream& input );
        void ReadInfo( const std::string& type, xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;

    public:
        ADN_Type_Bool bIsAmbulance_;
        AmbulanceInfos ambulanceInfos_;
        ADN_Type_Bool bIsAmbulanceReleve_;
        AmbulanceInfos ambulanceReleveInfos_;

        ADN_Type_Bool bIsDoctor_;
        ADN_Type_Bool bIsSortingDoctor_;
        ADN_Type_Bool bIsCuringDoctor_;
        ADN_Type_Bool doctorSkills_[ eNbrDoctorSkills ];
        ADN_Type_Bool bCuresNBC_;
        ADN_Type_Bool bCuresShock_;
    };

    //*****************************************************************************
    class NTIInfos : public ADN_RefWithName
    {
    public:
        explicit NTIInfos( const std::string& strName );
        virtual ~NTIInfos() {}

        void CopyFrom( NTIInfos& src );

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;

        bool IsTypeValid() const;

    public:
        std::string strName_;
        ADN_Type_Bool bIsPresent_;
        ADN_Type_Bool bHasMaxRepairTime_;
        ADN_Type_Time maxRepairTime_;
        ADN_Type_Bool bCanRepairEA_;
        ADN_Type_Bool bCanRepairM_;
    };

    //*****************************************************************************
    class LogMaintenanceInfos : public ADN_Ref_ABC
    {
    public:
                 LogMaintenanceInfos();
        virtual ~LogMaintenanceInfos() {}

        void CopyFrom( LogMaintenanceInfos& src );

        void ReadArchive( xml::xistream& input );
        void ReadInfo( const std::string& type, xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;

        bool IsRepairTypeValid() const;

    public:
        ADN_Type_Bool bIsTower_;
        ADN_Type_Double rCapacity_;
        ADN_Type_Time loadTime_;
        ADN_Type_Time unloadTime_;

        NTIInfos NTI1Infos_;
        NTIInfos NTI2Infos_;
        NTIInfos NTI3Infos_;
    };

    //*****************************************************************************
    class LogSupplyInfos : public ADN_Ref_ABC
    {
    public:
                 LogSupplyInfos();
        virtual ~LogSupplyInfos() {}

        void CopyFrom( LogSupplyInfos& src );

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;

    public:
        ADN_TypePtr_InVector_ABC< ADN_Natures_Data::NatureInfos > ptr_;
        ADN_Type_Double rMinWeight_;
        ADN_Type_Double rMaxWeight_;
        ADN_Type_Double rMinVolume_;
        ADN_Type_Double rMaxVolume_;
    };

    //*****************************************************************************
    class LogInfos : public ADN_Ref_ABC
    {
    public:
                 LogInfos();
        virtual ~LogInfos() {}

        void CopyFrom( LogInfos& src );

        void ReadArchive( xml::xistream& input );
        void ReadLogisticFunction( const std::string& type, xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;

        bool IsRepairTypeValid() const;

    public:
        ADN_Type_Bool bHasHealthInfos_;
        LogHealthInfos healthInfos_;
        ADN_Type_Bool bHasMaintenanceInfos_;
        LogMaintenanceInfos maintenanceInfos_;
        ADN_Type_Bool bHasSupplyInfos_;
        LogSupplyInfos supplyInfos_;
    };

    //*****************************************************************************
    class BreakdownInfos : public ADN_CrossedRef< ADN_Breakdowns_Data::BreakdownInfo >
    {
    public:
                 BreakdownInfos();
        virtual ~BreakdownInfos() {}

        BreakdownInfos* CreateCopy();
        void ReadArchive( xml::xistream& input );
        void WriteArchive( const std::string& origin, xml::xostream& output ) const;

    public:
        ADN_Type_Double rPercentage_;
    };

    typedef ADN_Type_Vector_ABC< BreakdownInfos > T_BreakdownInfos_Vector;

    //*****************************************************************************
    class BreakdownGroupInfos : public ADN_Ref_ABC
    {

    public:
        explicit BreakdownGroupInfos( const std::string& strName );
        virtual ~BreakdownGroupInfos();

        bool Contains( ADN_Breakdowns_Data::BreakdownInfo& breakdown ) const;
        void CopyFrom( BreakdownGroupInfos& src );
        void ReadArchive( xml::xistream& input, const std::string& parentName );
        void ReadBreakdown( xml::xistream& input, const std::string& parentName );
        void WriteArchive( xml::xostream& output ) const;
        void CheckValidity( ADN_ConsistencyChecker& checker, const std::string& composante ) const;

    public:
        std::string strName_;
        T_BreakdownInfos_Vector vBreakdowns_;
    };

    //*****************************************************************************
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
        ADN_Type_Int nConstruction_;

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

    typedef ADN_Type_Vector_ABC<SpeedInfos> T_SpeedInfos_Vector;

    //*****************************************************************************
    class SensorInfos : public ADN_CrossedRef< ADN_Sensors_Data::SensorInfos >
    {
    public:
                 SensorInfos();
        virtual ~SensorInfos() {}

        SensorInfos* CreateCopy();

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;

    public:
        ADN_Type_Double rHeight_;

    public:
        class Cmp : public std::unary_function< SensorInfos*, bool >
        {
        public:
            Cmp( const std::string& val ) : val_( val ) {}
            ~Cmp() {}

            bool operator()( SensorInfos* tgtnfos ) const
            {
                return tgtnfos->GetCrossedElement() && tgtnfos->GetCrossedElement()->strName_ == val_;
            }

        private:
            std::string val_;
        };
    };

    typedef ADN_Type_Vector_ABC< SensorInfos > T_SensorInfos_Vector;

    //*****************************************************************************
    class RadarInfos : public ADN_CrossedRef< ADN_Radars_Data::RadarInfos >
    {
    public:
                 RadarInfos();
        virtual ~RadarInfos() {}

        RadarInfos* CreateCopy();

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;

    public:
        ADN_Type_Double rHeight_;
    };

    typedef ADN_Type_Vector_ABC< RadarInfos > T_RadarInfos_Vector;

    //*****************************************************************************
    class WeaponInfos : public ADN_CrossedRef< ADN_Weapons_Data_WeaponInfos >
    {
    public:
                 WeaponInfos();
        explicit WeaponInfos( ADN_Weapons_Data_WeaponInfos& weapon );
        virtual ~WeaponInfos() {}

        WeaponInfos* CreateCopy();
        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;
    };

    typedef ADN_Type_Vector_ABC< WeaponInfos > T_WeaponInfos_Vector;

    //*****************************************************************************
    class ActiveProtectionsInfos : public ADN_CrossedRef< ADN_ActiveProtections_Data::ActiveProtectionsInfos >
    {
    public:
                 ActiveProtectionsInfos();
        explicit ActiveProtectionsInfos( ADN_ActiveProtections_Data::ActiveProtectionsInfos& activeProtections );
        virtual ~ActiveProtectionsInfos() {}

        ActiveProtectionsInfos* CreateCopy();
        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;
        void ReadProtection( xml::xistream& input );
    };

    typedef ADN_Type_Vector_ABC< ActiveProtectionsInfos > T_ActiveProtectionsInfos_Vector;


    //*****************************************************************************
    class ActiveProtectionInfos : public ADN_Ref_ABC
    {
    public:
                 ActiveProtectionInfos();
        virtual ~ActiveProtectionInfos() {}

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;
        void ReadProtection( xml::xistream& input );

        std::vector< std::string > protections_;
    };

    //*****************************************************************************
    class CategoryInfos : public ADN_CrossedRef< ADN_Resources_Data::CategoryInfo >
    {
    public:
        explicit CategoryInfos( ADN_Resources_Data::ResourceInfos& parentDotation );
        virtual ~CategoryInfos() {}

        CategoryInfos* CreateCopy();
        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;

    public:
        ADN_TypePtr_InVector_ABC< ADN_Resources_Data::ResourceInfos > ptrDotation_;
        ADN_Type_Int rNbr_;
        ADN_Type_Double rLogThreshold_;
        ADN_Type_Double rNormalizedConsumption_;
    };

    typedef ADN_Type_Vector_ABC< CategoryInfos > T_CategoryInfos_Vector;

    //*****************************************************************************
    class ResourceInfos : public ADN_Ref_ABC
    {
    public:
                 ResourceInfos();
        virtual ~ResourceInfos() {}

        void CopyFrom( ResourceInfos& src );
        void ReadCategory( xml::xistream& input, const std::string& parentName );
        void ReadArchive( xml::xistream& input, const std::string& parentName );
        void ReadDotation( xml::xistream& input, ADN_Resources_Data::ResourceInfos& dotation, const std::string& parentName );
        void WriteArchive( xml::xostream& output ) const;

    public:
        T_CategoryInfos_Vector categories_;
    };

    //*****************************************************************************
    class ObjectInfos : public ADN_CrossedRef< ADN_Objects_Data_ObjectInfos >
    {
    public:
                 ObjectInfos();
        virtual ~ObjectInfos() {}

        ObjectInfos* CreateCopy();
        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;

    public:
        ADN_Type_Bool bInitialBuildTime_;
        ADN_Type_Bool bInitialDestructionTime_;
        ADN_Type_Bool bCoeffBuildTime_;
        ADN_Type_Bool bCoeffDestructionTime_;
        ADN_Type_Bool bValorizationTime_;
        ADN_Type_Bool bDevalorizationTime_;
        ADN_Type_Bool bExtinguishingTime_;
        ADN_Type_Bool bCoeffCircTime_;
        ADN_Type_Bool bSpeedCirc_;
        ADN_Type_Bool bSpeedNotCirc_;

        ADN_Type_Time initialBuildTime_;
        ADN_Type_Time initialDestructionTime_;
        ADN_Type_Time coeffBuildTime_;
        ADN_Type_Time coeffDestructionTime_;
        ADN_Type_Time valorizationTime_;
        ADN_Type_Time devalorizationTime_;
        ADN_Type_Time extinguishingTime_;
        ADN_Type_Double rCoeffCirc_;
        ADN_Type_Double rSpeedCirc_;
        ADN_Type_Double rSpeedNotCirc_;
    };

    typedef ADN_Type_Vector_ABC< ObjectInfos > T_ObjectInfos_Vector;

    //*****************************************************************************
    class DisasterImpactInfos : public ADN_Ref_ABC
    {
    public:
        DisasterImpactInfos();

        DisasterImpactInfos* CreateCopy();
        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;

    public:
        ADN_Type_Double threshold_;
        ADN_Type_Double modifier_;
    };

    typedef ADN_Type_Vector_ABC< DisasterImpactInfos > T_DisasterImpactInfos_Vector;

    //*****************************************************************************
    class ConsumptionItem : public ADN_CrossedRef< ADN_Equipments_Data::CategoryInfos >
    {
    public:
                 ConsumptionItem( E_ConsumptionType nConsumptionType, const T_CategoryInfos_Vector& resources, ADN_Equipments_Data::CategoryInfos* equipmentCategory );
        virtual ~ConsumptionItem() {}

        ConsumptionItem* CreateCopy();
        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;

    public:
        E_ConsumptionType nConsumptionType_;
        ADN_Type_Double nQuantityUsedPerHour_;
    };

    typedef ADN_Type_Vector_ABC< ConsumptionItem > T_ConsumptionItem_Vector;

    //*****************************************************************************
    class ConsumptionsInfos : public ADN_Ref_ABC
    {
    public:
                 ConsumptionsInfos();
        virtual ~ConsumptionsInfos() {}

        void CopyFrom( ConsumptionsInfos& source );
        void ReadArchive( xml::xistream& input, T_CategoryInfos_Vector& equipmentCategories, const std::string& parentName );
        void ReadConsumption( xml::xistream& input, T_CategoryInfos_Vector& equipmentCategories, const std::string& parentName );
        void ReadDotation( xml::xistream& input, const E_ConsumptionType& type, T_CategoryInfos_Vector& equipmentCategories, const std::string& parentName );
        void FillMissingConsumptions( T_CategoryInfos_Vector& equipmentCategories );
        void WriteArchive( xml::xostream& output ) const;

    public:
        T_ConsumptionItem_Vector vConsumptions_;
    };

    //*****************************************************************************
    class AviationResourceQuotasInfos : public ADN_Ref_ABC
    {
    public:
        explicit AviationResourceQuotasInfos( E_AviationRange nRange );
        virtual ~AviationResourceQuotasInfos() {}

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output );

    public:
        E_AviationRange nRange_;
        ADN_Type_Int resourceQuotas_[ eNbrAmmunitionType + 1 ];

    private:
        void ReadQuota( xml::xistream& input );

    public:
        class Cmp : public std::unary_function< AviationResourceQuotasInfos*, bool >
        {
        public:
            Cmp( E_AviationRange& val ) : val_( val ) {}
            ~Cmp() {}

            bool operator()( AviationResourceQuotasInfos* tgtnfos ) const
            {
                return tgtnfos->nRange_ == val_;
            }

        private:
            E_AviationRange val_;
        };
    };

    typedef ADN_Type_Vector_ABC< AviationResourceQuotasInfos > T_AviationResourceQuotasInfos_Vector;


    //*****************************************************************************
    class EquipmentInfos : public ADN_RefWithLocalizedName
    {
    public:
                 EquipmentInfos();
        explicit EquipmentInfos( unsigned int id );
        virtual ~EquipmentInfos();

        void Initialize();
        EquipmentInfos* CreateCopy();
        void ReadArchive( xml::xistream& input );
        void ReadSpeed( xml::xistream& input );
        void ReadAviationQuota( xml::xistream& input );
        void ReadSensor( xml::xistream& input );
        void ReadRadar( xml::xistream& input );
        void ReadWeapon( xml::xistream& input );
        void ReadActiveProtection( xml::xistream& input );
        void ReadObject( xml::xistream& input );
        void ReadDisasterImpact( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;
        virtual void CheckValidity();
        void CheckDatabaseValidity( ADN_ConsistencyChecker& checker );
        void FillMissingConsumptions();

    public:
        ADN_Type_Int nId_;
        ADN_Type_LocalizedString strAdditionalComments_;
        ADN_Type_String strCodeEMAT6_;
        ADN_Type_String strCodeEMAT8_;
        ADN_Type_String strCodeLFRIL_;
        ADN_Type_String strCodeNNO_;
        ADN_Type_LocalizedString strNativeCountry_;

        ADN_Type_String strStartingCountry_;
        ADN_Type_String strStartingDate_;
        ADN_Type_LocalizedString strInformationOrigin_;
        ADN_Type_LocalizedString equipmentCategory_;
        ADN_TypePtr_InVector_ABC< ADN_Armors_Data::ArmorInfos > ptrArmor_;
        ADN_TypePtr_InVector_ABC< ADN_Volumes_Data::VolumeInfos > ptrSize_;
        ADN_Type_Double rWeight_;
        ADN_Type_Double rMaxSpeed_;
        T_SpeedInfos_Vector vSpeeds_;
        T_WeaponInfos_Vector vWeapons_;
        T_SensorInfos_Vector vSensors_;
        T_RadarInfos_Vector vRadars_;
        T_ObjectInfos_Vector vObjects_;
        T_DisasterImpactInfos_Vector vDisasterImpacts_;
        ConsumptionsInfos consumptions_;
        ResourceInfos resources_;
        ADN_Type_Bool bAviationResourcesQuotas_;
        T_AviationResourceQuotasInfos_Vector vAviationResourceQuotas_;
        T_ActiveProtectionsInfos_Vector vActiveProtections_;
        BreakdownGroupInfos attritionBreakdowns_;
        BreakdownGroupInfos randomBreakdowns_;
        ADN_Type_Bool bTroopEmbarkingTimes_;
        ADN_Type_Time embarkingTimePerPerson_;
        ADN_Type_Time disembarkingTimePerPerson_;
        ADN_Type_Bool bCanCarryCargo_;
        ADN_Type_Double rWeightTransportCapacity_;
        ADN_Type_Time embarkingTimePerTon_;
        ADN_Type_Time disembarkingTimePerTon_;
        ADN_Type_Bool bCanTransportDestroyed_;
        ADN_Type_Bool bCanCarryCrowd_;
        ADN_Type_Int nCrowdTransportCapacity_;
        ADN_Type_Time crowdEmbarkingTimePerPerson_;
        ADN_Type_Time crowdDisembarkingTimePerPerson_;
        ADN_Type_Int nMaxSlope_;
        ADN_Type_Int nSlopeDeceleration_;
        LogInfos logInfos_;
        ADN_Type_Int nPowerDirectFire_;
        ADN_Type_Int nPowerIndirectFire_;
        ADN_Type_Int nPowerCloseCombat_;
        ADN_Type_Int nPowerEngineering_;
        ADN_Type_Double length_;
        ADN_Type_Double width_;
        ADN_Type_Double frontSeparationDistance_;
        ADN_Type_Double safetyDistance_;
        ADN_Type_Double speedSafetyDistance_;
    public:
        class CmpId : public std::unary_function< EquipmentInfos*, bool >
        {
        public:
            CmpId( int val) : val_( val ) {}
            ~CmpId() {}

            bool operator()( EquipmentInfos* tgtnfos ) const
            {
                return tgtnfos->nId_.GetData() == val_;
            }

        private:
            int val_;
        };
    };

    typedef ADN_Type_Vector_ABC< EquipmentInfos > T_EquipmentInfos_Vector;

    //*****************************************************************************
public:
             ADN_Equipments_Data();
    virtual ~ADN_Equipments_Data();

    void FilesNeeded( tools::Path::T_Paths& l ) const;
    virtual void CheckDatabaseValidity( ADN_ConsistencyChecker& checker ) const;
    T_EquipmentInfos_Vector& GetEquipments();
    EquipmentInfos* FindEquipment( const std::string& strName );

    QStringList GetEquipmentsThatUse( ADN_Radars_Data::RadarInfos& sensor );
    QStringList GetEquipmentsThatUse( ADN_Sensors_Data::SensorInfos& sensor );
    QStringList GetEquipmentsThatUse( ADN_Weapons_Data_WeaponInfos& weapon );
    QStringList GetEquipmentsThatUse( ADN_Breakdowns_Data::BreakdownInfo& breakdown );
    QStringList GetEquipmentsThatUse( ADN_Objects_Data_ObjectInfos& object );
    QStringList GetEquipmentsThatUse( ADN_Armors_Data::ArmorInfos& armor );
    QStringList GetEquipmentsThatUse( ADN_Volumes_Data::VolumeInfos& size );
    QStringList GetEquipmentsThatUse( ADN_ActiveProtections_Data::ActiveProtectionsInfos& activeProtection );
    QStringList GetEquipmentsThatUse( ADN_Resources_Data::CategoryInfo& category );

private:
    void ReadElement( xml::xistream& input );
    void ReadArchive( xml::xistream& input );
    void WriteArchive( xml::xostream& output ) const;

public:
    T_EquipmentInfos_Vector vEquipments_;

private:
    static tools::IdManager idManager_;
};

//-----------------------------------------------------------------------------
// Name: ADN_Equipments_Data::T_ComposanteInfos_Vector
// Created: JDY 03-07-17
//-----------------------------------------------------------------------------
inline
ADN_Equipments_Data::T_EquipmentInfos_Vector&  ADN_Equipments_Data::GetEquipments()
{
    return vEquipments_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_Data::FindComposante
// Created: APE 2004-12-01
// -----------------------------------------------------------------------------
inline
ADN_Equipments_Data::EquipmentInfos* ADN_Equipments_Data::FindEquipment( const std::string& strName )
{
    auto it = std::find_if( vEquipments_.begin(), vEquipments_.end(), ADN_Tools::NameCmp( strName ) );
    if( it == vEquipments_.end() )
        return 0;
    return *it;
}

#endif // __ADN_Equipments_Data_h_
