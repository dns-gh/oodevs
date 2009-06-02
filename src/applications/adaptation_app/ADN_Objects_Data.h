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

#include <string>
#include <map>
#include <memory>

#include "boost/mpl/vector.hpp"
#include "boost/mpl/transform.hpp"
#include "boost/tuple/tuple.hpp"
#include "boost/shared_ptr.hpp"

#include "ADN_Data_ABC.h"

#include "ADN_Types.h"
#include "ADN_Type_Vector_ABC.h"
#include "ADN_Equipement_Data.h"
#include "ADN_DataTreeNode_ABC.h"
#include "ADN_Type_Choice.h"


class xml::xistream;


//*****************************************************************************
// Created: JDY 03-06-25
//*****************************************************************************
class ADN_Objects_Data : public ADN_Data_ABC
{    
    MT_COPYNOTALLOWED( ADN_Objects_Data )

public:
//*****************************************************************************
    class ScoreLocationInfos
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( ScoreLocationInfos )

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
    typedef ADN_Equipement_Data::AttritionInfos AttritionInfos; 
    typedef ADN_Type_VectorFixed_ABC<AttritionInfos> T_AttritionInfosVector;
    typedef T_AttritionInfosVector::iterator        IT_AttritionInfosVector;


//*****************************************************************************
   /* class PopulationAttritionInfos
    {
    public:
        PopulationAttritionInfos();
        ~PopulationAttritionInfos();

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output );

    public:
        ADN_Type_Double rSurface_;
        ADN_Type_Double rPh_;
    };*/
        
    struct NullType {};

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

    class ADN_TypeCapacity_Infos
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {
    public:
        ADN_TypeCapacity_Infos() : bPresent_ ( false ) {}
        virtual ~ADN_TypeCapacity_Infos() {}
        
        virtual void ReadArchive( xml::xistream& )
        {
            bPresent_ = true;
        }
        virtual void WriteArchive( xml::xostream& ) {}        
    public:
        ADN_Type_Bool   bPresent_;        
    };
    
    enum E_Capacities
    {   
        eActivableCapacity = 0,
        eAttritionCapacity,
        eAvoidableCapacity,
        eBridgingCapacity,
        eBuildableCapacity,
        eBypassableCapacity,
        eConstructorCapacity,
        eContaminationCapacity,
        eDecontaminationCapacity,
        eDetectionCapacity,
        eExtinguishableCapacity,
        eHealableCapacity,
        eImprovableCapacity,
        eInteractionHeightCapacity,
        eInteractWithEnemyCapacity,
        eInterferenceCapacity,
        eIntoxicationCapacity,
        eLogisticCapacity,
        eMobilityCapacity,
        eOccupableCapacity,
        ePropagationCapacity,
        eProtectionCapacity,
        eSupplyCapacity,
        eSupplyRouteCapacity,
        eTerrainHeuristicCapacity,
        eTimeLimitedCapacity,
        eWorkableCapacity	
    };

    template< E_Capacities T, typename DefaultFieldsHolderType = NullType >
    class ADN_CapacityInfos_Default
        : public ADN_TypeCapacity_Infos
    {
    public:
        typedef typename DefaultFieldsHolderType FieldsHolderType;
        enum { type = T };
    
    public:        
        static const std::string DISPLAY_NAME;
        static const std::string TAG;

        ADN_CapacityInfos_Default() {}

        virtual void ReadArchive( xml::xistream& xis )
        {
            ADN_TypeCapacity_Infos::ReadArchive( xis );
        }
        virtual void WriteArchive( xml::xostream& /*xos*/ ) {}
    };

    typedef ADN_CapacityInfos_Default< eActivableCapacity >         ADN_CapacityInfos_Activable;
    typedef ADN_CapacityInfos_Default< eDetectionCapacity >         ADN_CapacityInfos_Detection;
    typedef ADN_CapacityInfos_Default< eExtinguishableCapacity >    ADN_CapacityInfos_Extinguishable;
    typedef ADN_CapacityInfos_Default< eInteractWithEnemyCapacity > ADN_CapacityInfos_InteractWithEnemy;
    typedef ADN_CapacityInfos_Default< eInterferenceCapacity >      ADN_CapacityInfos_Interference;
    typedef ADN_CapacityInfos_Default< eLogisticCapacity >          ADN_CapacityInfos_Logistic;
    typedef ADN_CapacityInfos_Default< eSupplyCapacity >            ADN_CapacityInfos_Supply;
    typedef ADN_CapacityInfos_Default< eTimeLimitedCapacity >       ADN_CapacityInfos_TimeLimited;
 
    class ADN_CapacityInfos_Attrition
        : public ADN_CapacityInfos_Default< eAttritionCapacity >
    {
    public:		
        ADN_CapacityInfos_Attrition();

        void ReadArchive( xml::xistream& xis );
        void WriteArchive( xml::xostream& xos );
    public:
        ADN_TypePtr_InVector_ABC< ADN_Equipement_Data::CategoryInfo > category_;        
    };

     class ADN_CapacityInfos_Avoidable
        : public ADN_CapacityInfos_Default< eAvoidableCapacity, 
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
        : public ADN_CapacityInfos_Default< eBridgingCapacity >
    {
    public:
        ADN_CapacityInfos_Bridging();

        void ReadArchive( xml::xistream& xis );
        void WriteArchive( xml::xostream& xos );		
    public:
        
    };

    class ADN_CapacityInfos_Buildable
        : public ADN_CapacityInfos_Default< eBuildableCapacity >
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
        : public ADN_CapacityInfos_Default< eBypassableCapacity, 
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
        : public ADN_CapacityInfos_Default< eContaminationCapacity >
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
        : public ADN_TypeCapacity_Infos
    {
    public:
        static const std::string TAG;
        static const std::string DISPLAY_NAME;

        ADN_CapacityInfos_Decontamination();

        void ReadArchive( xml::xistream& xis );
        void WriteArchive( xml::xostream& xos );

    public:
        typedef NullType FieldsHolderType;
    };

    class ADN_CapacityInfos_Healable
        : public ADN_TypeCapacity_Infos
    {
    public:
        static const std::string TAG;
        static const std::string DISPLAY_NAME;

        ADN_CapacityInfos_Healable();

        void ReadArchive( xml::xistream& xis );
        void WriteArchive( xml::xostream& xos );

    public:
        typedef NullType FieldsHolderType;
        ADN_Type_Double emergencyBedsRate_;
        ADN_Type_Double emergencyDoctorsRate_;
        ADN_Type_Double nightDoctorsRate_;
    };

    class ADN_CapacityInfos_Improvable
        : public ADN_CapacityInfos_Default< eImprovableCapacity >
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
        : public ADN_CapacityInfos_Default< eInteractionHeightCapacity > 
    {
    public:
        ADN_CapacityInfos_InteractionHeight();

        void ReadArchive( xml::xistream& xis );
        void WriteArchive( xml::xostream& xos );		

    public:		
        ADN_Type_Double height_;
    };

    class ADN_CapacityInfos_Intoxication
        : public ADN_CapacityInfos_Default< eIntoxicationCapacity >
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
        : public ADN_CapacityInfos_Default< eMobilityCapacity >
    {
    public:		
        ADN_CapacityInfos_Mobility();
        void ReadArchive( xml::xistream& xis );
        void WriteArchive( xml::xostream& xos );		
    
    public:
        ADN_Type_Double     rDefaultSpeed_;        
        ADN_Type_Enum< E_SpeedImpact, eNbrSpeedImpact > nSpeedModifier_;        
        ADN_Type_Double     rMaxAgentSpeed_; // percentage
    };

    class ADN_CapacityInfos_Occupable
        : public ADN_TypeCapacity_Infos
    {
    public:
        static const std::string TAG;
        static const std::string DISPLAY_NAME;

        ADN_CapacityInfos_Occupable();

        void ReadArchive( xml::xistream& xis );
        void WriteArchive( xml::xostream& xos );

    public:
        typedef NullType FieldsHolderType;
    };

    class ADN_CapacityInfos_Population
        : public ADN_TypeCapacity_Infos
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
        typedef NullType FieldsHolderType;
        ADN_Type_Double density_;
    };

    class ADN_CapacityInfos_Propagation
        : public ADN_TypeCapacity_Infos
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
        typedef NullType FieldsHolderType;
        ADN_Type_Choice< std::string > model_;
    };

    class ADN_CapacityInfos_Protection
        : public ADN_TypeCapacity_Infos
    {
    public:
        static const std::string TAG;
        static const std::string DISPLAY_NAME;
    public:
        ADN_CapacityInfos_Protection();
        void ReadArchive( xml::xistream& xis );
        void WriteArchive( xml::xostream& xos );

    public:
        ADN_Type_Int  max_size_; 
        ADN_Type_Bool geniePrepared_;       
    };

    class ADN_CapacityInfos_SupplyRoute
        : public ADN_TypeCapacity_Infos
    {
    public:
        static const std::string TAG;
        static const std::string DISPLAY_NAME;

        ADN_CapacityInfos_SupplyRoute();

        void ReadArchive( xml::xistream& xis );
        void WriteArchive( xml::xostream& xos );

        const std::string& displayName() const
        {
            return DISPLAY_NAME;
        }
        const std::string& tag() const
        {
            return TAG;
        }
    public:
        typedef NullType FieldsHolderType;
    };

    class ADN_CapacityInfos_TerrainHeuristic
        : public ADN_TypeCapacity_Infos
    {
    public:
        static const std::string TAG;
        static const std::string DISPLAY_NAME;

        ADN_CapacityInfos_TerrainHeuristic();

        void ReadArchive( xml::xistream& xis );
        void WriteArchive( xml::xostream& xos );
        
        const std::string& displayName() const
        {
            return DISPLAY_NAME;
        }
        const std::string& tag() const
        {
            return TAG;
        }
    public:
        void ReadTerrain( xml::xistream& xis );

    public:
        typedef NullType FieldsHolderType;
        T_ScoreLocationInfosVector scores_;
    };

    class ADN_CapacityInfos_Workable
        : public ADN_CapacityInfos_Default< eWorkableCapacity >
    {
    public:
        ADN_CapacityInfos_Workable();
        void ReadArchive( xml::xistream& xis );
        void WriteArchive( xml::xostream& xos );

    public:		
        ADN_Type_Int  worker_;        
    };

    class ADN_CapacityInfos_Constructor
        : public ADN_CapacityInfos_Default< eConstructorCapacity >
    {
    public:
        ADN_CapacityInfos_Constructor();
        void ReadArchive( xml::xistream& xis );
        void WriteArchive( xml::xostream& xos );		
    
    private:
        static const char* choices_[];

    public:
        ADN_Type_Enum< E_ConsumptionType, eNbrConsumptionType>      nDefaultConsumption_;
        ADN_Type_Choice< std::string > unitType_;   
        std::auto_ptr< ADN_CapacityInfos_Buildable >     ptrBuildable_;
        std::auto_ptr< ADN_CapacityInfos_Improvable >    ptrImprovable_;
    };
    
//*****************************************************************************
    class ObjectInfos
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( ObjectInfos )

    public:
                 ObjectInfos();
        explicit ObjectInfos( const std::string& name );
        ~ObjectInfos();

        virtual std::string GetNodeName();
        std::string GetItemName();

        void ReadArchive         ( xml::xistream& input );
        void ReadSensiblePosition( xml::xistream& input );
        void ReadDotation        ( const std::string& type, xml::xistream& input );
        void ReadUnitAttrition   ( xml::xistream& input );
        void WriteArchive( xml::xostream& output );

    private:
        void ReadCapacityArchive ( const std::string& type, xml::xistream& input );
        void InitializeCapacities();

    public:
        ADN_Type_String strName_;
        ADN_Type_String strType_;
        ADN_Type_String geometries_;
        ADN_Type_String symbol_;

        typedef std::map< std::string, boost::shared_ptr< ADN_TypeCapacity_Infos > > T_CapacityMap;
        typedef T_CapacityMap::iterator IT_CapacityMap;
        typedef T_CapacityMap::const_iterator CIT_CapacityMap;
        T_CapacityMap capacities_;
        
        static int VAL;
        template <typename T> struct Enumerator
        {
            enum E { value = VAL++ };
        };
    };
    
    typedef ADN_Type_Vector_ABC< ObjectInfos > T_ObjectsInfos_Vector;
    typedef T_ObjectsInfos_Vector::iterator  IT_ObjectsInfos_Vector;


//*****************************************************************************
public:    
             ADN_Objects_Data();
    virtual ~ADN_Objects_Data();

    void FilesNeeded(T_StringList& l) const;
    void Reset();
    void SaveAttritionInfos( xml::xostream& output );


    T_ObjectsInfos_Vector& GetObjectInfos();
    ObjectInfos*           FindObject( const std::string& strName );

private:
    void ReadArchive( xml::xistream& input );
    void ReadObject ( xml::xistream& input );
    void ReadShotPercentage( xml::xistream& input );
    
    void WriteArchive( xml::xostream& output );

private:
    T_ObjectsInfos_Vector     vObjectInfos_;	
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
        { return ADN_Tools::CaselessCompare( tgtnfos->strType_.GetData(), strType_ ); }

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
