//*****************************************************************************
//
// $Created: JDY 03-06-25 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Objects_Data.h $
// $Author: Ape $
// $Modtime: 15/04/05 18:35 $
// $Revision: 9 $
// $Workfile: ADN_Objects_Data.h $
//
//*****************************************************************************

#ifndef __ADN_Objects_Data_h_
#define __ADN_Objects_Data_h_

#include "ADN_CrossedRef.h"
#include "ADN_Data_ABC.h"
#include "ADN_Types.h"
#include "ADN_Type_Vector_ABC.h"
#include "ADN_CapacityInfos.h"
#include "ADN_Resources_Data.h"
#include "ADN_Equipments_Data.h"
#include "ADN_Fires_Data.h"
#include "ADN_Objects_Data_ObjectInfos.h"
#include "ADN_Disasters_Data.h"
#include <boost/tuple/tuple.hpp>

//*****************************************************************************
// Created: JDY 03-06-25
//*****************************************************************************
class ADN_Objects_Data : public ADN_Data_ABC
{
public:
//*****************************************************************************
    class ScoreLocationInfos : public ADN_Ref_ABC
    {
    public:
                 ScoreLocationInfos();
        virtual ~ScoreLocationInfos() {}

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;

    public:
        ADN_Type_Enum< E_Location, eNbrLocation > nLocation_;
        ADN_Type_Int nScore_;

    public:
        class Cmp : public std::unary_function< ScoreLocationInfos*, bool >
        {
        public:
            Cmp( const E_Location& val ) : val_( val ) {}
            virtual ~Cmp() {}

            bool operator()( ScoreLocationInfos* tgtnfos ) const
            {
                return tgtnfos->nLocation_.GetData() == val_;
            }

        private:
            E_Location val_;
        };
    };

    typedef ADN_Type_Vector_ABC< ScoreLocationInfos > T_ScoreLocationInfosVector;

    template< typename T >
    struct NamedField
    {
    public:
        typedef T value_type;

        NamedField( const char* n ) : name_( n ) {}
        NamedField( const std::string& n ) : name_( n ) {}

        NamedField( const NamedField& cp ) : name_( cp.name_ ) {}
        const NamedField& operator = ( const NamedField& cp )
        {
            name_ = cp.name_;
        }

        const std::string& name() const { return name_; }
        const value_type& value() const { return value_; }
        value_type& value() { return value_; }

    private:
        std::string name_;
        T value_;
    };

    typedef helpers::ADN_CapacityInfos_Default< helpers::eActivableCapacity >               ADN_CapacityInfos_Activable;
    typedef helpers::ADN_CapacityInfos_Default< helpers::eDelayCapacity >                   ADN_CapacityInfos_Delay;
    typedef helpers::ADN_CapacityInfos_Default< helpers::eInterferenceCapacity >            ADN_CapacityInfos_Interference;
    typedef helpers::ADN_CapacityInfos_Default< helpers::eLogisticCapacity >                ADN_CapacityInfos_Logistic;
    typedef helpers::ADN_CapacityInfos_Default< helpers::eSupplyCapacity >                  ADN_CapacityInfos_Supply;
    typedef helpers::ADN_CapacityInfos_Default< helpers::eTimeLimitedCapacity >             ADN_CapacityInfos_TimeLimited;
    typedef helpers::ADN_CapacityInfos_Default< helpers::eBurnCapacity >                    ADN_CapacityInfos_Burn;
    typedef helpers::ADN_CapacityInfos_Default< helpers::eUniversalCapacity >               ADN_CapacityInfos_Universal;
    typedef helpers::ADN_CapacityInfos_Default< helpers::eStockCapacity >                   ADN_CapacityInfos_Stock;
    typedef helpers::ADN_CapacityInfos_Default< helpers::eResourceNetworkCapacity >         ADN_CapacityInfos_ResourceNetwork;
    typedef helpers::ADN_CapacityInfos_Default< helpers::eLodgingCapacity >                 ADN_CapacityInfos_Lodging;
    typedef helpers::ADN_CapacityInfos_Default< helpers::eAltitudeModifierCapacity >        ADN_CapacityInfos_AltitudeModifier;
    typedef helpers::ADN_CapacityInfos_Default< helpers::eUndergroundNetworkCapacity >      ADN_CapacityInfos_UndergroundNetwork;
    typedef helpers::ADN_CapacityInfos_Default< helpers::eFireForbiddenCapacity >           ADN_CapacityInfos_FireForbidden;
    typedef helpers::ADN_CapacityInfos_Default< helpers::eBorderCapacity >                  ADN_CapacityInfos_Border;
    typedef helpers::ADN_CapacityInfos_Default< helpers::eDecontaminationCapacity >         ADN_CapacityInfos_Decontamination;
    typedef helpers::ADN_CapacityInfos_Default< helpers::eSupplyRouteCapacity >             ADN_CapacityInfos_SupplyRoute;

    class ADN_CapacityInfos_Disaster : public helpers::ADN_CapacityInfos_Default< helpers::eDisasterCapacity >
    {
    public:
                 ADN_CapacityInfos_Disaster();
        virtual ~ADN_CapacityInfos_Disaster() {}

        void ReadArchive( xml::xistream& xis );
        void WriteArchive( xml::xostream& xos ) const;

        virtual void CheckDatabaseValidity( ADN_ConsistencyChecker& checker, const ADN_Type_String& objectName ) const;

    public:
        ADN_TypePtr_InVector_ABC< ADN_Disasters_Data::DisasterInfos > disaster_;
    };

    class ADN_CapacityInfos_Attrition : public helpers::ADN_CapacityInfos_Default< helpers::eAttritionCapacity >
    {
    public:
                 ADN_CapacityInfos_Attrition();
        virtual ~ADN_CapacityInfos_Attrition() {}
        void ReadArchive( xml::xistream& xis );
        void WriteArchive( xml::xostream& xos ) const;
        void CheckDatabaseValidity( ADN_ConsistencyChecker& checker, const ADN_Type_String& objectName );
    public:
        ADN_TypePtr_InVector_ABC< ADN_Resources_Data::CategoryInfo > ammoCategory_;
        ADN_TypePtr_InVector_ABC< ADN_Resources_Data::CategoryInfo > mineCategory_;
        ADN_TypePtr_InVector_ABC< ADN_Resources_Data::CategoryInfo > explosiveCategory_;
        ADN_Type_Bool useAmmo_;
        ADN_Type_Bool useMine_;
        ADN_Type_Bool useExplo_;
        ADN_Type_Double attritionSurface_;
        ADN_Type_Double ph_;
    };

    class ADN_CapacityInfos_UrbanDestruction : public helpers::ADN_CapacityInfos_Default< helpers::eUrbanDestructionCapacity >
    {
    public:
        ADN_CapacityInfos_UrbanDestruction();
        void ReadArchive( xml::xistream& xis );
        void WriteArchive( xml::xostream& xos ) const;

    public:
        helpers::T_UrbanAttritionInfos_Vector modifUrbanBlocks_;
    };

    class ADN_CapacityInfos_Avoidable : public helpers::ADN_CapacityInfos_Default< helpers::eAvoidableCapacity, boost::tuples::tuple< NamedField< ADN_Type_Double > > >
    {
    public:
        ADN_CapacityInfos_Avoidable();
        void ReadArchive( xml::xistream& xis );
        void WriteArchive( xml::xostream& xos ) const;

    public:
        ADN_Type_Double rDistance_;
    };

    class ADN_CapacityInfos_Bridging : public helpers::ADN_CapacityInfos_Default< helpers::eBridgingCapacity >
    {
    public:
        ADN_CapacityInfos_Bridging();
        void ReadArchive( xml::xistream& xis );
        void WriteArchive( xml::xostream& xos ) const;

    public:
        ADN_Type_Enum< E_CrossingType, eNbrCrossingType > type_;
    };

    class ADN_CapacityInfos_Buildable : public helpers::ADN_CapacityInfos_Default< helpers::eBuildableCapacity >
    {
    public:
        ADN_CapacityInfos_Buildable();
        void ReadArchive( xml::xistream& xis );
        void WriteArchive( xml::xostream& xos ) const;
        void ReadDotation( xml::xistream& xis );

    public:
        ADN_Type_Vector_ABC< ADN_Equipments_Data::CategoryInfos > categories_;
    };

    class ADN_CapacityInfos_Bypassable : public helpers::ADN_CapacityInfos_Default< helpers::eBypassableCapacity, boost::tuples::tuple< NamedField< ADN_Type_Double > > >
    {
    public:
        ADN_CapacityInfos_Bypassable();
        void ReadArchive( xml::xistream& xis );
        void WriteArchive( xml::xostream& xos ) const;

    public:
        ADN_Type_Double  rSpeed_;
    };

    class ADN_CapacityInfos_Contamination : public helpers::ADN_CapacityInfos_Default< helpers::eContaminationCapacity >
    {
    public:
        ADN_CapacityInfos_Contamination();
        void ReadArchive( xml::xistream& xis );
        void WriteArchive( xml::xostream& xos ) const;

    public:
        ADN_Type_String type_;
        ADN_Type_Int max_toxic_;
    };

    class ADN_CapacityInfos_Flood : public helpers::ADN_CapacityInfos_Default< helpers::eFloodCapacity >
    {
    public:
        ADN_CapacityInfos_Flood();
        void ReadArchive( xml::xistream& xis );
        void WriteArchive( xml::xostream& xos ) const;
        void CheckDatabaseValidity( ADN_ConsistencyChecker& checker, const ADN_Type_String& objectName ) const;

    private:
        void ReadInjury( xml::xistream& xis );

    public:
        ADN_Type_Int nNbHurtHumans1_;
        ADN_Type_Int nNbHurtHumans2_;
        ADN_Type_Int nNbHurtHumans3_;
        ADN_Type_Int nNbHurtHumansE_;
        ADN_Type_Int nNbDeadHumans_;
    };

    class ADN_CapacityInfos_Improvable : public helpers::ADN_CapacityInfos_Default< helpers::eImprovableCapacity >
    {
    public:
        ADN_CapacityInfos_Improvable();
        void ReadArchive( xml::xistream& xis );
        void WriteArchive( xml::xostream& xos ) const;

    private:
        void ReadDotation( xml::xistream& xis );

    public:
        ADN_Type_Vector_ABC< ADN_Equipments_Data::CategoryInfos > categories_;
    };

    class ADN_CapacityInfos_InteractionHeight : public helpers::ADN_CapacityInfos_Default< helpers::eInteractionHeightCapacity >
    {
    public:
        ADN_CapacityInfos_InteractionHeight();
        void ReadArchive( xml::xistream& xis );
        void WriteArchive( xml::xostream& xos ) const;

    public:
        ADN_Type_Double height_;
    };

    class ADN_CapacityInfos_Intoxication : public helpers::ADN_CapacityInfos_Default< helpers::eIntoxicationCapacity >
    {
    public:
        ADN_CapacityInfos_Intoxication();
        void ReadArchive( xml::xistream& xis );
        void WriteArchive( xml::xostream& xos ) const;

    public:
        ADN_Type_String type_;
        ADN_Type_Int max_toxic_;
    };

    class ADN_CapacityInfos_Mobility : public helpers::ADN_CapacityInfos_Default< helpers::eMobilityCapacity >
    {
    public:
        ADN_CapacityInfos_Mobility();
        void ReadArchive( xml::xistream& xis );
        void WriteArchive( xml::xostream& xos ) const;

    public:
        ADN_Type_Double rDefaultSpeed_;
        ADN_Type_Enum< E_SpeedImpact, eNbrSpeedImpact > nSpeedModifier_;
        ADN_Type_Double rMaxAgentSpeed_; // percentage
    };

    class ADN_CapacityInfos_Trafficability : public helpers::ADN_CapacityInfos_Default< helpers::eTrafficabilityCapacity >
    {
    public:
        ADN_CapacityInfos_Trafficability();
        void ReadArchive( xml::xistream& xis );
        void FixConsistency();
        void WriteArchive( xml::xostream& xos ) const;

    public:
        ADN_Type_Bool limited_;
        ADN_Type_Double maxWeight_;
    };

    class ADN_CapacityInfos_Population : public helpers::ADN_CapacityInfos_Default< helpers::ePopulationCapacity >
    {
    public:
        ADN_CapacityInfos_Population();
        void ReadArchive( xml::xistream& xis );
        void WriteArchive( xml::xostream& xos ) const;

    public:
        ADN_Type_Double density_;
    };

    class ADN_CapacityInfos_Propagation : public helpers::ADN_CapacityInfos_Default< helpers::ePropagationCapacity >
    {
    public:
        ADN_CapacityInfos_Propagation();
        void ReadArchive( xml::xistream& xis );
        void WriteArchive( xml::xostream& xos ) const;

    public:
        ADN_Type_Enum< E_PropagationModel, eNbrPropagationModel > nModel_;
    };

    class ADN_CapacityInfos_Protection : public helpers::ADN_CapacityInfos_Default< helpers::eProtectionCapacity >
    {
    public:
        ADN_CapacityInfos_Protection();
        void ReadArchive( xml::xistream& xis );
        void WriteArchive( xml::xostream& xos ) const;

    public:
        ADN_Type_Int max_size_;
        ADN_Type_Bool geniePrepared_;
    };

    class ADN_CapacityInfos_TerrainHeuristic : public helpers::ADN_CapacityInfos_Default< helpers::eTerrainHeuristicCapacity >
    {
    public:
        ADN_CapacityInfos_TerrainHeuristic();
        void ReadArchive( xml::xistream& xis );
        void WriteArchive( xml::xostream& xos ) const;
        void ReadTerrain( xml::xistream& xis );

    public:
        T_ScoreLocationInfosVector scores_;
    };

    class ADN_CapacityInfos_Workable : public helpers::ADN_CapacityInfos_Default< helpers::eWorkableCapacity >
    {
    public:
        ADN_CapacityInfos_Workable();
        void ReadArchive( xml::xistream& xis );
        void WriteArchive( xml::xostream& xos ) const;

    public:
        ADN_Type_Int worker_;
    };

    class ADN_CapacityInfos_Constructor : public helpers::ADN_CapacityInfos_Default< helpers::eConstructorCapacity >
    {
    public:
        ADN_CapacityInfos_Constructor();
        void ReadArchive( xml::xistream& xis );
        void WriteArchive( xml::xostream& xos ) const;

    public:
        ADN_Type_Enum< E_ConsumptionType, eNbrConsumptionType > nDefaultConsumption_;
        ADN_Type_Enum< E_ConstructorType, eNbrConstructorType > nUnitType_;
        std::unique_ptr< ADN_CapacityInfos_Buildable > ptrBuildable_;
        std::unique_ptr< ADN_CapacityInfos_Improvable > ptrImprovable_;
    };

    class ADN_CapacityInfos_Detection : public helpers::ADN_CapacityInfos_Default< helpers::eDetectionCapacity >
    {
    public:
        ADN_CapacityInfos_Detection();
        void ReadArchive( xml::xistream& input ); // LTO
        void ReadAcquisitionTime( xml::xistream& input ); // LTO
        void WriteArchive( xml::xostream& output ) const; // LTO

    public:
        ADN_Type_Bool bDetectTime_;
        ADN_Type_Time detectTime_; // LTO
        ADN_Type_Bool bIdentTime_; // LTO
        ADN_Type_Time recoTime_; // LTO
        ADN_Type_Bool bRecoTime_; // LTO
        ADN_Type_Time identTime_; // LTO
    };

    class ADN_CapacityInfos_Spawn : public helpers::ADN_CapacityInfos_Default< helpers::eSpawnCapacity >
    {
    public:
        ADN_CapacityInfos_Spawn();
        void Load( const std::string& parentName );
        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;
        void CheckDatabaseValidity( ADN_ConsistencyChecker& checker, const ADN_Type_String& objectName );

    public:
        ADN_TypePtr_InVector_ABC< ADN_Objects_Data_ObjectInfos > object_;
        std::string objectName_;
        ADN_Type_Double rActionRange_;
        ADN_Type_Bool nbc_;
    };

    class ADN_CapacityInfos_Structural : public helpers::ADN_CapacityInfos_Default< helpers::eStructuralCapacity >
    {
    public:
        ADN_CapacityInfos_Structural();
        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;

    public:
        ADN_Type_Int rStructuralState_;
    };

    class ADN_CapacityInfos_AttitudeModifier : public helpers::ADN_CapacityInfos_Default< helpers::eAttitudeModifierCapacity >
    {
    public:
        ADN_CapacityInfos_AttitudeModifier();
        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;

    public:
        ADN_Type_Enum< E_PopulationAttitude, eNbrPopulationAttitude > attitude_;
    };

    class ADN_CapacityInfos_Perception : public helpers::ADN_CapacityInfos_Default< helpers::ePerceptionCapacity >
    {
    public:
        ADN_CapacityInfos_Perception();
        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;

    public:
        ADN_Type_Bool blinded_;
    };

    class ADN_CapacityInfos_Scattering : public helpers::ADN_CapacityInfos_Default< helpers::eScatteringCapacity >
    {
    public:
        ADN_CapacityInfos_Scattering();
        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;

    public:
        ADN_Type_Int humanByTimeStep_;
    };

    class ADN_CapacityInfos_FirePropagationModifier : public helpers::ADN_CapacityInfos_Default< helpers::eFirePropagationModifierCapacity >
    {
    public:
        ADN_CapacityInfos_FirePropagationModifier();
        void ReadArchive( xml::xistream& xis );
        void WriteArchive( xml::xostream& xos ) const;
        void CheckDatabaseValidity( ADN_ConsistencyChecker& checker, const ADN_Type_String& objectName );

    public:
        class ModifierByFireClass : public ADN_CrossedRef< ADN_Fires_Data::FireClassInfos >
        {
        public:
            explicit ModifierByFireClass( ADN_Fires_Data::FireClassInfos* );
            ADN_Type_Int ignitionThreshold_;
            ADN_Type_Int maxCombustionEnergy_;

            std::string GetItemName();

        public:
            void ReadArchive( xml::xistream& xis );
            void WriteArchive( xml::xostream& xos ) const;

            typedef ADN_Fires_Data::FireClassInfos T_Item;

            class CmpRef : public std::unary_function< ADN_Fires_Data::FireClassInfos* , bool >
            {
            public:
                void ReadArchive( xml::xistream& xis );
                void WriteArchive( xml::xostream& xos ) const;

                CmpRef( ADN_Fires_Data::FireClassInfos* val ) : val_( val ){}
                bool operator()( ModifierByFireClass* other ) const
                {
                    return other->GetCrossedElement() == val_;
                }

            private:
                ADN_Fires_Data::FireClassInfos* val_;
            };

            class Cmp : public std::unary_function< ADN_Fires_Data::FireClassInfos* , bool >
            {
            public:
                Cmp( const std::string& name ) : name_( name ) {}
                bool operator()( ModifierByFireClass* other ) const
                {
                    return other->GetCrossedElement() && other->GetCrossedElement()->strName_ == name_;
                }

            private:
                std::string name_;
            };
        };

        typedef ADN_Type_VectorFixed_ABC< ModifierByFireClass > T_ModifierByFireClass_Vector;

        T_ModifierByFireClass_Vector modifiers_;

    private:
        void ReadModifier( xml::xistream& xis );
    };

    class ADN_CapacityInfos_InteractWithSide : public helpers::ADN_CapacityInfos_Default< helpers::eInteractWithSideCapacity >
    {
    public:
        ADN_CapacityInfos_InteractWithSide();
        void ReadArchive( xml::xistream& input );
        void ReadSide( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;

    public:
        ADN_Type_Bool bFriendSide_;
        ADN_Type_Bool bEnemySide_;
        ADN_Type_Bool bNeutralSide_;
        ADN_Type_Bool bCivilian_;
    };

//*****************************************************************************

    typedef ADN_Type_Vector_ABC< ADN_Objects_Data_ObjectInfos > T_ObjectsInfos_Vector;

//*****************************************************************************
public:
             ADN_Objects_Data();
    virtual ~ADN_Objects_Data();

    virtual void CheckDatabaseValidity( ADN_ConsistencyChecker& checker ) const;
    void FilesNeeded( tools::Path::T_Paths& l ) const;
    void SaveAttritionInfos( xml::xostream& output );

    T_ObjectsInfos_Vector& GetObjectInfos();
    ADN_Objects_Data_ObjectInfos* FindObject( const std::string& strName );

    QStringList GetObjectsThatUse( ADN_Objects_Data_ObjectInfos& object );
    QStringList GetObjectsThatUse( ADN_Resources_Data::CategoryInfo& object );
    QStringList GetObjectsThatUse( ADN_Disasters_Data::DisasterInfos& disaster );
    QStringList GetObjectsWithCapacity( const std::string& tag );

private:
    void ReadArchive( xml::xistream& input );
    void ReadObject( xml::xistream& input );
    bool FixConsistency();
    void WriteArchive( xml::xostream& output ) const;

private:
    T_ObjectsInfos_Vector vObjectInfos_;
};

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::GetObjectInfos
// Created: APE 2004-11-24
// -----------------------------------------------------------------------------
inline
ADN_Objects_Data::T_ObjectsInfos_Vector& ADN_Objects_Data::GetObjectInfos()
{
    return vObjectInfos_;
}

namespace
{
    template< typename T >
    class TypeCmp : public std::unary_function< T* , bool >
    {
    public:
        TypeCmp( const std::string& strType ) : strType_( strType ) {}
        ~TypeCmp() {}

        template < typename T >
        bool operator()( T* tgtnfos ) const
        {
            return tgtnfos->strType_ == strType_;
        }

    private:
        std::string strType_;
    };
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::FindObject
// Created: APE 2004-11-30
// -----------------------------------------------------------------------------
inline
ADN_Objects_Data_ObjectInfos* ADN_Objects_Data::FindObject( const std::string& strName )
{
    auto it = std::find_if( vObjectInfos_.begin(), vObjectInfos_.end(), TypeCmp< ADN_Objects_Data_ObjectInfos >( strName ) );
    if( it == vObjectInfos_.end() )
        return 0;
    return *it;
}

#endif // __ADN_Objects_Data_h_
