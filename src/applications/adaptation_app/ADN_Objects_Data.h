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

#include "ADN_Data_ABC.h"
#include "ADN_Types.h"
#include "ADN_Type_Vector_ABC.h"
#include "ADN_Equipement_Data.h"
#include "ADN_DataTreeNode_ABC.h"
#include "ADN_Type_Choice.h"
#include "ADN_FireClass_Data.h"
#include "ADN_Symbols_Data.h"
#include <string>
#include <map>
#include <memory>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/shared_ptr.hpp>
#include "ADN_CapacityInfos.h"
namespace xml { class xistream; }

//*****************************************************************************
// Created: JDY 03-06-25
//*****************************************************************************
class ADN_Objects_Data : public ADN_Data_ABC
{

public:
//*****************************************************************************
    class ScoreLocationInfos : public ADN_Ref_ABC
                             , public ADN_DataTreeNode_ABC
    {

    public:
        ScoreLocationInfos();

        virtual std::string GetNodeName();
        std::string GetItemName();

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output );

    public:
        ADN_Type_Enum< E_Location, eNbrLocation > nLocation_;
        ADN_Type_Int nScore_;

    public:
        class Cmp : public std::unary_function< ScoreLocationInfos* , bool >
        {
        public:
            Cmp(const E_Location& val) : val_(val) {}
            virtual ~Cmp() {}

            bool operator()( ScoreLocationInfos* tgtnfos ) const
            {   return tgtnfos->nLocation_.GetData()==val_;}

        private:
            E_Location val_;
        };
    };

    typedef ADN_Type_Vector_ABC< ScoreLocationInfos > T_ScoreLocationInfosVector;
    typedef T_ScoreLocationInfosVector::iterator      IT_ScoreLocationInfosVector;


//*****************************************************************************
//    typedef helpers::AttritionInfos AttritionInfos;
//    typedef ADN_Type_VectorFixed_ABC<AttritionInfos> T_AttritionInfosVector;
//    typedef T_AttritionInfosVector::iterator        IT_AttritionInfosVector;


//*****************************************************************************
    //struct NullType {};

    template <typename T> struct NamedField
    {
    public:
        typedef T value_type;

        NamedField(const char* n) : name_ (n) {}
        NamedField(const std::string& n) : name_ (n) {}

        NamedField(const NamedField& cp) : name_(cp.name_){}
        const NamedField& operator = (const NamedField& cp)
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

    typedef helpers::ADN_CapacityInfos_Default< helpers::eActivableCapacity >         ADN_CapacityInfos_Activable;
    typedef helpers::ADN_CapacityInfos_Default< helpers::eDelayCapacity >             ADN_CapacityInfos_Delay;
    typedef helpers::ADN_CapacityInfos_Default< helpers::eExtinguishableCapacity >    ADN_CapacityInfos_Extinguishable;
    typedef helpers::ADN_CapacityInfos_Default< helpers::eInteractWithEnemyCapacity > ADN_CapacityInfos_InteractWithEnemy;
    typedef helpers::ADN_CapacityInfos_Default< helpers::eInterferenceCapacity >      ADN_CapacityInfos_Interference;
    typedef helpers::ADN_CapacityInfos_Default< helpers::eLogisticCapacity >          ADN_CapacityInfos_Logistic;
    typedef helpers::ADN_CapacityInfos_Default< helpers::eSupplyCapacity >            ADN_CapacityInfos_Supply;
    typedef helpers::ADN_CapacityInfos_Default< helpers::eTimeLimitedCapacity >       ADN_CapacityInfos_TimeLimited;
    typedef helpers::ADN_CapacityInfos_Default< helpers::eBurnCapacity >              ADN_CapacityInfos_Burn;
    typedef helpers::ADN_CapacityInfos_Default< helpers::eUniversalCapacity >         ADN_CapacityInfos_Universal;
    typedef helpers::ADN_CapacityInfos_Default< helpers::eStockCapacity >             ADN_CapacityInfos_Stock;
    typedef helpers::ADN_CapacityInfos_Default< helpers::eResourceNetworkCapacity >   ADN_CapacityInfos_ResourceNetwork;
    typedef helpers::ADN_CapacityInfos_Default< helpers::eLodgingCapacity >           ADN_CapacityInfos_Lodging;

    class ADN_CapacityInfos_Attrition
        : public helpers::ADN_CapacityInfos_Default< helpers::eAttritionCapacity >
    {
    public:
        ADN_CapacityInfos_Attrition();

        void ReadArchive( xml::xistream& xis );
        void WriteArchive( xml::xostream& xos );
    public:
        ADN_TypePtr_InVector_ABC< ADN_Equipement_Data::CategoryInfo > category_;
        ADN_TypePtr_InVector_ABC< ADN_Equipement_Data::AmmoCategoryInfo > ammoCategory_;
        ADN_Type_Bool useAmmo_;  // $$$$ LDC: Hack: No time to write a single category_ checkbox with all ammos available. Actually, the mines and explosives categories should vanish but no time to change the sim...
    };

     class ADN_CapacityInfos_Avoidable
        : public helpers::ADN_CapacityInfos_Default< helpers::eAvoidableCapacity,
                            boost::tuples::tuple<NamedField<ADN_Type_Double> > >
    {
    public:
        ADN_CapacityInfos_Avoidable();
        void ReadArchive( xml::xistream& xis );
        void WriteArchive( xml::xostream& xos );

    public:
        FieldsHolderType fields_;
        ADN_Type_Double  rDistance_;
    };

    class ADN_CapacityInfos_Bridging
        : public helpers::ADN_CapacityInfos_Default< helpers::eBridgingCapacity >
    {
    public:
        ADN_CapacityInfos_Bridging();

        void ReadArchive( xml::xistream& xis );
        void WriteArchive( xml::xostream& xos );
    public:

    };

    class ADN_CapacityInfos_Buildable
        : public helpers::ADN_CapacityInfos_Default< helpers::eBuildableCapacity >
    {
    public:
        ADN_CapacityInfos_Buildable();
        void ReadArchive( xml::xistream& xis );
        void WriteArchive( xml::xostream& xos );

    private:
        void ReadDotation( xml::xistream& xis );

    private:
        typedef ADN_Type_Vector_ABC< ADN_Equipement_Data::CategoryInfo > ::const_iterator CIT_Categories;

    public:
        ADN_Type_Vector_ABC< ADN_Equipement_Data::CategoryInfo > categories_;
    };

    class ADN_CapacityInfos_Bypassable
        : public helpers::ADN_CapacityInfos_Default< helpers::eBypassableCapacity,
                                            boost::tuples::tuple<NamedField<ADN_Type_Double> > >
    {
    public:
        ADN_CapacityInfos_Bypassable();
        void ReadArchive( xml::xistream& xis );
        void WriteArchive( xml::xostream& xos );

    public:
        FieldsHolderType fields_;
        ADN_Type_Double  rSpeed_;
    };

    class ADN_CapacityInfos_Contamination
        : public helpers::ADN_CapacityInfos_Default< helpers::eContaminationCapacity >
    {
    public:
        ADN_CapacityInfos_Contamination();

        void ReadArchive( xml::xistream& xis );
        void WriteArchive( xml::xostream& xos );

    public:
        ADN_Type_String type_;
        ADN_Type_Int max_toxic_;
    };

    class ADN_CapacityInfos_Decontamination
        : public helpers::ADN_TypeCapacity_Infos
    {
    public:
        static const std::string TAG;
        static const std::string DISPLAY_NAME;

        ADN_CapacityInfos_Decontamination();

        void ReadArchive( xml::xistream& xis );
        void WriteArchive( xml::xostream& xos );

    public:
        typedef helpers::NullType FieldsHolderType;
    };

    class ADN_CapacityInfos_Flood
        : public helpers::ADN_CapacityInfos_Default< helpers::eFloodCapacity >
    {
    public:
        ADN_CapacityInfos_Flood();

        void ReadArchive( xml::xistream& xis );
        void WriteArchive( xml::xostream& xos );

    private:
        void ReadInjury( xml::xistream& xis );

    public:
        ADN_Type_Int nNbHurtHumans1_;
        ADN_Type_Int nNbHurtHumans2_;
        ADN_Type_Int nNbHurtHumans3_;
        ADN_Type_Int nNbHurtHumansE_;
        ADN_Type_Int nNbDeadHumans_;
    };

    class ADN_CapacityInfos_Medical
        : public helpers::ADN_CapacityInfos_Default< helpers::eMedicalCapacity >
    {
    public:
        ADN_CapacityInfos_Medical();

        void ReadArchive( xml::xistream& xis );
        void WriteArchive( xml::xostream& xos );

    public:
        typedef helpers::NullType FieldsHolderType;
        ADN_Type_Int emergencyBedsRate_;
        ADN_Type_Int emergencyDoctorsRate_;
        ADN_Type_Int nightDoctorsRate_;
    };

    class ADN_CapacityInfos_Improvable
        : public helpers::ADN_CapacityInfos_Default< helpers::eImprovableCapacity >
    {
    public:
        ADN_CapacityInfos_Improvable();

        void ReadArchive( xml::xistream& xis );
        void WriteArchive( xml::xostream& xos );

    private:
        void ReadDotation( xml::xistream& xis );

    private:
        typedef ADN_Type_Vector_ABC< ADN_Equipement_Data::CategoryInfo > ::const_iterator CIT_Categories;

    public:
        ADN_Type_Vector_ABC< ADN_Equipement_Data::CategoryInfo > categories_;
    };

    class ADN_CapacityInfos_InteractionHeight
        : public helpers::ADN_CapacityInfos_Default< helpers::eInteractionHeightCapacity >
    {
    public:
        ADN_CapacityInfos_InteractionHeight();

        void ReadArchive( xml::xistream& xis );
        void WriteArchive( xml::xostream& xos );

    public:
        ADN_Type_Double height_;
    };

    class ADN_CapacityInfos_Intoxication
        : public helpers::ADN_CapacityInfos_Default< helpers::eIntoxicationCapacity >
    {
    public:
        ADN_CapacityInfos_Intoxication();

        void ReadArchive( xml::xistream& xis );
        void WriteArchive( xml::xostream& xos );

    public:
        ADN_Type_String type_;
        ADN_Type_Int max_toxic_;
    };

    class ADN_CapacityInfos_Mobility
        : public helpers::ADN_CapacityInfos_Default< helpers::eMobilityCapacity >
    {
    public:
        ADN_CapacityInfos_Mobility();
        void ReadArchive( xml::xistream& xis );
        void WriteArchive( xml::xostream& xos );

    public:
        ADN_Type_Double rDefaultSpeed_;
        ADN_Type_Enum< E_SpeedImpact, eNbrSpeedImpact > nSpeedModifier_;
        ADN_Type_Double rMaxAgentSpeed_; // percentage
    };

    class ADN_CapacityInfos_Occupable
        : public helpers::ADN_TypeCapacity_Infos
    {
    public:
        static const std::string TAG;
        static const std::string DISPLAY_NAME;

        ADN_CapacityInfos_Occupable();

        void ReadArchive( xml::xistream& xis );
        void WriteArchive( xml::xostream& xos );

    public:
        typedef helpers::NullType FieldsHolderType;
    };

    class ADN_CapacityInfos_Population
        : public helpers::ADN_TypeCapacity_Infos
    {
    public:

        static const std::string TAG;
        static const std::string DISPLAY_NAME;

        ADN_CapacityInfos_Population();

        void ReadArchive( xml::xistream& xis );
        void WriteArchive( xml::xostream& xos );
    private:
        static const char* choices_[];
    public:
        typedef helpers::NullType FieldsHolderType;
        ADN_Type_Double density_;
    };

    class ADN_CapacityInfos_Propagation
        : public helpers::ADN_TypeCapacity_Infos
    {
    public:

        static const std::string TAG;
        static const std::string DISPLAY_NAME;

        ADN_CapacityInfos_Propagation();

        void ReadArchive( xml::xistream& xis );
        void WriteArchive( xml::xostream& xos );
    private:
        static const char* choices_[];
    public:
        typedef helpers::NullType FieldsHolderType;
        ADN_Type_Choice< std::string > model_;
    };

    class ADN_CapacityInfos_Protection
        : public helpers::ADN_TypeCapacity_Infos
    {
    public:
        static const std::string TAG;
        static const std::string DISPLAY_NAME;
    public:
        ADN_CapacityInfos_Protection();
        void ReadArchive( xml::xistream& xis );
        void WriteArchive( xml::xostream& xos );

    public:
        ADN_Type_Int max_size_;
        ADN_Type_Bool geniePrepared_;
    };

    class ADN_CapacityInfos_SupplyRoute
        : public helpers::ADN_TypeCapacity_Infos
    {
    public:
        static const std::string TAG;
        static const std::string DISPLAY_NAME;

        ADN_CapacityInfos_SupplyRoute();

        void ReadArchive( xml::xistream& xis );
        void WriteArchive( xml::xostream& xos );

    public:
        typedef helpers::NullType FieldsHolderType;
    };

    class ADN_CapacityInfos_TerrainHeuristic
        : public helpers::ADN_TypeCapacity_Infos
    {
    public:
        static const std::string TAG;
        static const std::string DISPLAY_NAME;

        ADN_CapacityInfos_TerrainHeuristic();

        void ReadArchive( xml::xistream& xis );
        void WriteArchive( xml::xostream& xos );

    public:
        void ReadTerrain( xml::xistream& xis );

    public:
        typedef helpers::NullType FieldsHolderType;
        T_ScoreLocationInfosVector scores_;
    };

    class ADN_CapacityInfos_Workable
        : public helpers::ADN_CapacityInfos_Default< helpers::eWorkableCapacity >
    {
    public:
        ADN_CapacityInfos_Workable();
        void ReadArchive( xml::xistream& xis );
        void WriteArchive( xml::xostream& xos );

    public:
        ADN_Type_Int worker_;
    };

    class ADN_CapacityInfos_Constructor
        : public helpers::ADN_CapacityInfos_Default< helpers::eConstructorCapacity >
    {
    public:
        ADN_CapacityInfos_Constructor();
        void ReadArchive( xml::xistream& xis );
        void WriteArchive( xml::xostream& xos );

    private:
        static const char* choices_[];

    public:
        ADN_Type_Enum< E_ConsumptionType, eNbrConsumptionType> nDefaultConsumption_;
        ADN_Type_Choice< std::string > unitType_;
        std::auto_ptr< ADN_CapacityInfos_Buildable > ptrBuildable_;
        std::auto_ptr< ADN_CapacityInfos_Improvable > ptrImprovable_;
    };

    class ADN_CapacityInfos_Detection
        : public helpers::ADN_CapacityInfos_Default< helpers::eDetectionCapacity >
    {
    public:
        static const std::string TAG;
        static const std::string DISPLAY_NAME;
        ADN_CapacityInfos_Detection();

        void ReadArchive( xml::xistream& input ); // LTO
        void ReadAcquisitionTime( xml::xistream& input ); // LTO
        void WriteArchive( xml::xostream& output ); // LTO

    public:
        ADN_Type_Bool bDetectTime_;
        ADN_Type_Time detectTime_; // LTO
        ADN_Type_Bool bIdentTime_; // LTO
        ADN_Type_Time recoTime_; // LTO
        ADN_Type_Bool bRecoTime_; // LTO
        ADN_Type_Time identTime_; // LTO
    };

    class ObjectInfos;
    class ADN_CapacityInfos_Spawn : public helpers::ADN_CapacityInfos_Default< helpers::eSpawnCapacity >
    {
    public:
        static const std::string TAG;
        static const std::string DISPLAY_NAME;
        ADN_CapacityInfos_Spawn();

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output );

    public:
        ADN_Type_String strObjectType_;
        ADN_Type_Double rActionRange_;
    };

    class ADN_CapacityInfos_Structural : public helpers::ADN_CapacityInfos_Default< helpers::eStructuralCapacity >
    {
    public:
        static const std::string TAG;
        static const std::string DISPLAY_NAME;
        ADN_CapacityInfos_Structural();

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output );

    public:
        ADN_Type_Int rStructuralState_;
    };

    class ADN_CapacityInfos_AttitudeModifier : public helpers::ADN_CapacityInfos_Default< helpers::eAttitudeModifierCapacity >
    {
    public:
        static const std::string TAG;
        static const std::string DISPLAY_NAME;
        ADN_CapacityInfos_AttitudeModifier();

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output );

    public:
        ADN_Type_Enum< E_PopulationAttitude, eNbrPopulationAttitude > attitude_;
    };

    class ADN_CapacityInfos_Perception : public helpers::ADN_CapacityInfos_Default< helpers::ePerceptionCapacity >
    {
    public:
        static const std::string TAG;
        static const std::string DISPLAY_NAME;
        ADN_CapacityInfos_Perception();

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output );

    public:
        ADN_Type_Bool blinded_;
    };

    class ADN_CapacityInfos_Scattering
        : public helpers::ADN_CapacityInfos_Default< helpers::eScatteringCapacity >
    {
    public:
        static const std::string TAG;
        static const std::string DISPLAY_NAME;
        ADN_CapacityInfos_Scattering();

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output );

    public:
        ADN_Type_Int humanByTimeStep_;
    };

    class ADN_CapacityInfos_FirePropagationModifier : public helpers::ADN_CapacityInfos_Default< helpers::eFirePropagationModifierCapacity >
    {
    public:
        ADN_CapacityInfos_FirePropagationModifier();
        void ReadArchive( xml::xistream& xis );
        void WriteArchive( xml::xostream& xos );
    public:

        class ModifierByFireClass : public ADN_Ref_ABC
                                  , public ADN_DataTreeNode_ABC
        {
        public:
            explicit ModifierByFireClass( ADN_FireClass_Data::FireClassInfos* );
            ADN_TypePtr_InVector_ABC< ADN_FireClass_Data::FireClassInfos > ptrFireClass_;
            ADN_Type_Int ignitionThreshold_;
            ADN_Type_Int maxCombustionEnergy_;

            std::string GetItemName();

        public:
            void ReadArchive( xml::xistream& xis );
            void WriteArchive( xml::xostream& xos );

            typedef ADN_FireClass_Data::FireClassInfos T_Item;

            class CmpRef : public std::unary_function< ADN_FireClass_Data::FireClassInfos* , bool >
            {
            public:
                void ReadArchive( xml::xistream& xis );
                void WriteArchive( xml::xostream& xos );

                CmpRef( ADN_FireClass_Data::FireClassInfos* val ) : val_( val ){}
                bool operator()( ModifierByFireClass* other ) const
                {
                    return other->ptrFireClass_.GetData() == val_;
                }

            private:
                ADN_FireClass_Data::FireClassInfos* val_;
            };

            class Cmp : public std::unary_function< ADN_FireClass_Data::FireClassInfos* , bool >
            {
            public:
                Cmp( const std::string& name ) : name_( name ) {}
                bool operator()( ModifierByFireClass* other ) const
                {
                    return other->ptrFireClass_.GetData()->strName_ == name_;
                }

            private:
                std::string name_;
            };
        };

        typedef ADN_Type_VectorFixed_ABC< ModifierByFireClass > T_ModifierByFireClass_Vector;
        typedef T_ModifierByFireClass_Vector::iterator         IT_ModifierByFireClass_Vector;

        T_ModifierByFireClass_Vector modifiers_;

    private:
        void ReadModifier( xml::xistream& xis );
    };

//*****************************************************************************
    class ObjectInfos : public ADN_Ref_ABC
                      , public ADN_DataTreeNode_ABC
    {

    public:
                 ObjectInfos();
        explicit ObjectInfos( const std::string& name );
        virtual ~ObjectInfos();

        virtual std::string GetNodeName();
        std::string GetItemName();
        ObjectInfos* CreateCopy();
        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output );

    private:
        void ReadCapacityArchive( const std::string& type, xml::xistream& input );
        void InitializeCapacities();

    public:
        ADN_Type_String strName_;
        ADN_Type_String strType_;
        ADN_Type_String geometries_;
        ADN_TypePtr_InVector_ABC< ADN_Symbols_Data::SymbolInfo > symbol_;

        typedef std::map< std::string, boost::shared_ptr< helpers::ADN_TypeCapacity_Infos > > T_CapacityMap;
        typedef T_CapacityMap::iterator                                                      IT_CapacityMap;
        typedef T_CapacityMap::const_iterator                                               CIT_CapacityMap;
        T_CapacityMap capacities_;

        static int VAL;
        template< typename T > struct Enumerator
        {
            enum E { value = VAL++ };
        };
    };

    typedef ADN_Type_Vector_ABC< ObjectInfos > T_ObjectsInfos_Vector;
    typedef T_ObjectsInfos_Vector::iterator   IT_ObjectsInfos_Vector;


//*****************************************************************************
public:
             ADN_Objects_Data();
    virtual ~ADN_Objects_Data();

    void FilesNeeded( T_StringList& l ) const;
    void Reset();
    void SaveAttritionInfos( xml::xostream& output );


    T_ObjectsInfos_Vector& GetObjectInfos();
    ObjectInfos* FindObject( const std::string& strName );

private:
    void ReadArchive( xml::xistream& input );
    void ReadObject( xml::xistream& input );
    void WriteArchive( xml::xostream& output );

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
            return ADN_Tools::CaselessCompare( tgtnfos->strType_.GetData(), strType_ );
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
ADN_Objects_Data::ObjectInfos* ADN_Objects_Data::FindObject( const std::string& strName )
{
    IT_ObjectsInfos_Vector it = std::find_if( vObjectInfos_.begin(), vObjectInfos_.end(), TypeCmp< ADN_Objects_Data::ObjectInfos>( strName ) );
    if( it == vObjectInfos_.end() )
        return 0;
    return *it;
}

#endif // __ADN_Objects_Data_h_
