//*****************************************************************************
//
// $Created: JDY 03-07-17 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Composantes_Data.h $
// $Author: Nld $
// $Modtime: 20/07/05 13:46 $
// $Revision: 20 $
// $Workfile: ADN_Composantes_Data.h $
//
//*****************************************************************************

#ifndef __ADN_Composantes_Data_h_
#define __ADN_Composantes_Data_h_

#include "ADN_Data_ABC.h"

#include "ADN_Types.h"
#include "ADN_Enums.h"
#include "ADN_Categories_Data.h"
#include "ADN_Sensors_Data.h"
#include "ADN_Launchers_Data.h"
#include "ADN_Objects_Data.h"
#include "ADN_Equipement_Data.h"
#include "ADN_Weapons_Data.h"
#include "ADN_Breakdowns_Data.h"



// =============================================================================
/** @class  ADN_Composantes_Data
@brief  ADN_Composantes_Data
*/
// Created: APE 2004-12-07
// =============================================================================
class ADN_Composantes_Data : public ADN_Data_ABC
{
    MT_COPYNOTALLOWED( ADN_Composantes_Data )

public:
    //*****************************************************************************
    class AmbulanceInfos
        : public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( AmbulanceInfos )

    public:
        AmbulanceInfos();

        virtual std::string GetNodeName();
        std::string GetItemName();

        void CopyFrom( AmbulanceInfos& src );

        void ReadArchive( xml::xistream& input );
        void WriteArchive( const std::string& section, xml::xostream& output );

    public:
        ADN_Type_Bool   transportSkills_[eNbrDoctorSkills];
        ADN_Type_Bool   bTransportNBC_;
        ADN_Type_Bool   bTransportShock_;
        ADN_Type_Double rCapacity_;
        ADN_Type_Time   loadTimePerPerson_;
        ADN_Type_Time   unloadTimePerPerson_;
    };


    //*****************************************************************************
    class LogHealthInfos
        : public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( LogHealthInfos )

    public:
        LogHealthInfos();

        virtual std::string GetNodeName();
        std::string GetItemName();

        void CopyFrom( LogHealthInfos& src );

        void ReadArchive( xml::xistream& input );
        void ReadInfo( const std::string& type, xml::xistream& input );
        void WriteArchive( xml::xostream& output );

    public:
        ADN_Type_Bool bIsAmbulance_;
        AmbulanceInfos ambulanceInfos_;
        ADN_Type_Bool bIsAmbulanceReleve_;
        AmbulanceInfos ambulanceReleveInfos_;

        ADN_Type_Bool bIsDoctor_;
        ADN_Type_Bool bIsSortingDoctor_;
        ADN_Type_Bool bIsCuringDoctor_;
        ADN_Type_Bool doctorSkills_[eNbrDoctorSkills];
        ADN_Type_Bool bCuresNBC_;
        ADN_Type_Bool bCuresShock_;
    };


    //*****************************************************************************
    class NTIInfos
        : public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( NTIInfos )

    public:
        NTIInfos( const std::string& strName );

        virtual std::string GetNodeName();
        std::string GetItemName();

        void CopyFrom( NTIInfos& src );

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output );

    public:
        std::string strName_;
        ADN_Type_Bool bIsPresent_;
        ADN_Type_Bool bHasMaxRepairTime_;
        ADN_Type_Time maxRepairTime_;
        ADN_Type_Bool bCanRepairEA_;
        ADN_Type_Bool bCanRepairM_;
    };


    //*****************************************************************************
    class LogMaintenanceInfos
        : public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( LogMaintenanceInfos )

    public:
        LogMaintenanceInfos();

        virtual std::string GetNodeName();
        std::string GetItemName();

        void CopyFrom( LogMaintenanceInfos& src );

        void ReadArchive( xml::xistream& input );
        void ReadInfo( const std::string& type, xml::xistream& input );
        void WriteArchive( xml::xostream& output );

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
    class LogSupplyInfos
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( LogSupplyInfos )

    public:
        LogSupplyInfos();

        virtual std::string GetNodeName();
        std::string GetItemName();

        void CopyFrom( LogSupplyInfos& src );

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output );

    public:
        ADN_Type_Bool bIsCarrier_;
        ADN_Type_Double rWeight_;
        ADN_Type_Double rVolume_;
        ADN_TypePtr_InVector_ABC<ADN_Categories_Data::DotationNatureInfos> ptrDotationNature_;
    };

    //*****************************************************************************
    class LogInfos
        : public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( LogInfos )

    public:
        LogInfos();

        virtual std::string GetNodeName();
        std::string GetItemName();

        void CopyFrom( LogInfos& src );

        void ReadArchive( xml::xistream& input );
        void ReadLogisticFunction( const std::string& type, xml::xistream& input );
        void WriteArchive( xml::xostream& output );

    public:
        ADN_Type_Bool       bHasHealthInfos_;
        LogHealthInfos      healthInfos_;
        ADN_Type_Bool       bHasMaintenanceInfos_;
        LogMaintenanceInfos maintenanceInfos_;
        ADN_Type_Bool       bHasSupplyInfos_;
        LogSupplyInfos      supplyInfos_;
    };


    //*****************************************************************************
    class BreakdownInfos
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( BreakdownInfos )

    public:
        BreakdownInfos();
        virtual std::string GetNodeName();
        std::string GetItemName();

        BreakdownInfos* CreateCopy();


        void ReadArchive( xml::xistream& input );
        void WriteArchive( const std::string& origin, xml::xostream& output );

    public:
        ADN_TypePtr_InVector_ABC<ADN_Breakdowns_Data::BreakdownInfo> ptrBreakdown_;
        ADN_Type_Double          rPercentage_; 
    };

    typedef ADN_Type_Vector_ABC<BreakdownInfos>     T_BreakdownInfos_Vector;
    typedef T_BreakdownInfos_Vector::iterator       IT_BreakdownInfos_Vector;

    //*****************************************************************************
    class BreakdownGroupInfos
        : public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( BreakdownGroupInfos )

    public:
        BreakdownGroupInfos( const std::string& strName );
        ~BreakdownGroupInfos();
        virtual std::string GetNodeName();
        std::string GetItemName();

        void CopyFrom( BreakdownGroupInfos& src );

        void ReadArchive( xml::xistream& input );
        void ReadBreakdown( xml::xistream& input );
        void WriteArchive( xml::xostream& output );

    public:
        std::string strName_;
        T_BreakdownInfos_Vector vBreakdowns_;
    };


    //*****************************************************************************
    class SpeedInfos
        : public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( SpeedInfos )

    public:
        SpeedInfos( E_Location nTypeTerrain );
        virtual std::string GetNodeName();
        std::string GetItemName();

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output );

    public:
        E_Location          nTypeTerrain_;
        ADN_Type_Double     rSpeed_;

    public:
        class Cmp : public std::unary_function< SpeedInfos* , bool >
        {
        public:
            Cmp(E_Location& val) : val_(val) {}
            ~Cmp() {}

            bool operator()( SpeedInfos* tgtnfos ) const 
            { return tgtnfos->nTypeTerrain_==val_; }

        private:

            E_Location val_;
        };   
    };

    typedef ADN_Type_Vector_ABC<SpeedInfos>    T_SpeedInfos_Vector;
    typedef T_SpeedInfos_Vector::iterator      IT_SpeedInfos_Vector;


    //*****************************************************************************
    class SensorInfos
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( SensorInfos )

    public:
        SensorInfos();

        virtual std::string GetNodeName();
        std::string GetItemName();

        SensorInfos* CreateCopy();

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output );

    public:
        ADN_TypePtr_InVector_ABC<ADN_Sensors_Data::SensorInfos> ptrSensor_;
        ADN_Type_Double                                         rHeight_;

    public:
        class Cmp : public std::unary_function< SensorInfos* , bool >
        {
        public:
            Cmp(const std::string& val) : val_(val) {}
            ~Cmp() {}

            bool operator()( SensorInfos* tgtnfos ) const 
            { return tgtnfos->ptrSensor_.GetData() && tgtnfos->ptrSensor_.GetData()->strName_==val_; }

        private:

            std::string val_;
        };
    };

    typedef ADN_Type_Vector_ABC<SensorInfos>    T_SensorInfos_Vector;
    typedef T_SensorInfos_Vector::iterator      IT_SensorInfos_Vector;


    //*****************************************************************************
    class RadarInfos
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( RadarInfos )

    public:
        RadarInfos();

        virtual std::string GetNodeName();
        std::string GetItemName();

        RadarInfos* CreateCopy();

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output );

    public:
        ADN_TypePtr_InVector_ABC<ADN_Radars_Data::RadarInfos> ptrRadar_;
        ADN_Type_String                                       strName_; //!< Not for editing.
    };

    typedef ADN_Type_Vector_ABC<RadarInfos>     T_RadarInfos_Vector;
    typedef T_RadarInfos_Vector::iterator      IT_RadarInfos_Vector;


    //*****************************************************************************
    class WeaponInfos
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( WeaponInfos )

    public:
        WeaponInfos();
        WeaponInfos( ADN_Weapons_Data::WeaponInfos& weapon );

        virtual std::string GetNodeName();
        std::string GetItemName();

        WeaponInfos* CreateCopy();

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output );

    public:
        ADN_TypePtr_InVector_ABC<ADN_Weapons_Data::WeaponInfos> ptrWeapon_;
        ADN_Type_String                                         strName_; //!< Not for editing. 

    public:
        /*
        class Cmp : public std::unary_function< WeaponInfos* , bool >
        {
        public:
        Cmp(const std::string& val) : val_(val) {}
        ~Cmp() {}

        bool operator()( WeaponInfos* tgtnfos ) const 
        { return tgtnfos->ptrLauncher_.GetData() && tgtnfos->ptrLauncher_.GetData()->strName_==val_; }

        private:

        std::string val_;
        };*/
    };

    typedef ADN_Type_Vector_ABC<WeaponInfos>    T_WeaponInfos_Vector;
    typedef T_WeaponInfos_Vector::iterator      IT_WeaponInfos_Vector;


    //*****************************************************************************
    class CategoryInfos
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( CategoryInfos )

    public:
        CategoryInfos( ADN_Equipement_Data::DotationInfos& parentDotation );

        virtual std::string GetNodeName();
        std::string GetItemName();

        CategoryInfos* CreateCopy();

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output );

    public:
        ADN_TypePtr_InVector_ABC<ADN_Equipement_Data::DotationInfos> ptrDotation_;
        ADN_TypePtr_InVector_ABC<ADN_Equipement_Data::CategoryInfo>  ptrCategory_;
        ADN_Type_Double                                              rNbr_;
        ADN_Type_Double                                              rLogThreshold_;
        ADN_Type_Double                                              rNormalizedConsumption_;
    };

    typedef ADN_Type_Vector_ABC<CategoryInfos>    T_CategoryInfos_Vector;
    typedef T_CategoryInfos_Vector::iterator      IT_CategoryInfos_Vector;


    //*****************************************************************************
    class DotationInfos
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( DotationInfos )

    public:
        DotationInfos();

        virtual std::string GetNodeName();
        std::string GetItemName();

        void CopyFrom( DotationInfos& src );

        void ReadCategory( xml::xistream& input );

        void ReadArchive( xml::xistream& input );
        void ReadDotation( xml::xistream& input, ADN_Equipement_Data::DotationInfos& dotation );
        void WriteArchive( xml::xostream& output );

    public:
        T_CategoryInfos_Vector categories_;
    };


    //*****************************************************************************
    class ObjectInfos
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( ObjectInfos )

    public:
        ObjectInfos();

        virtual std::string GetNodeName();
        std::string GetItemName();

        ObjectInfos* CreateCopy();

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output );

    public:
        ADN_TypePtr_InVector_ABC<ADN_Objects_Data::ObjectInfos>     ptrObject_;

        ADN_Type_Bool                                               bInitialBuildTime_;
        ADN_Type_Bool                                               bInitialDestructionTime_;
        ADN_Type_Bool                                               bCoeffBuildTime_;
        ADN_Type_Bool                                               bCoeffDestructionTime_;
        ADN_Type_Bool                                               bValorizationTime_;
        ADN_Type_Bool                                               bDevalorizationTime_;
        ADN_Type_Bool                                               bCoeffCircTime_;
        ADN_Type_Bool                                               bSpeedCirc_;
        ADN_Type_Bool                                               bSpeedNotCirc_;

        ADN_Type_Time                                               initialBuildTime_;
        ADN_Type_Time                                               initialDestructionTime_;
        ADN_Type_Time                                               coeffBuildTime_;
        ADN_Type_Time                                               coeffDestructionTime_;
        ADN_Type_Time                                               valorizationTime_;
        ADN_Type_Time                                               devalorizationTime_;
        ADN_Type_Double                                             rCoeffCirc_;
        ADN_Type_Double                                             rSpeedCirc_;
        ADN_Type_Double                                             rSpeedNotCirc_;

    public:
        class Cmp : public std::unary_function< ObjectInfos* , bool >
        {
        public:
            Cmp(const E_ObjectType& val) : val_(val) {}
            ~Cmp() {}

            bool operator()( ObjectInfos* tgtnfos ) const 
            { return tgtnfos->ptrObject_.GetData() && tgtnfos->ptrObject_.GetData()->nObjectType_.GetData()==val_; }

        private:

            E_ObjectType val_;
        };
    };

    typedef ADN_Type_Vector_ABC<ObjectInfos>    T_ObjectInfos_Vector;
    typedef T_ObjectInfos_Vector::iterator      IT_ObjectInfos_Vector;


    //*****************************************************************************
    class ConsumptionItem
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( ConsumptionItem )

    public:
        ConsumptionItem( E_ConsumptionType nConsumptionType, ADN_Equipement_Data::CategoryInfo& category );

        virtual std::string GetNodeName();
        std::string GetItemName();

        ConsumptionItem* CreateCopy();

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output );

    public:
        E_ConsumptionType                                           nConsumptionType_;
        ADN_TypePtr_InVector_ABC<ADN_Equipement_Data::CategoryInfo> ptrCategory_;
        ADN_Type_Double                                             nQuantityUsedPerHour_;
    };

    typedef ADN_Type_Vector_ABC<ConsumptionItem>    T_ConsumptionItem_Vector;
    typedef T_ConsumptionItem_Vector::iterator      IT_ConsumptionItem_Vector;


    //*****************************************************************************
    class ConsumptionsInfos
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( ConsumptionsInfos )

    public:
        ConsumptionsInfos();

        virtual std::string GetNodeName();
        std::string GetItemName();

        void CopyFrom( ConsumptionsInfos& source );

        void ReadArchive( xml::xistream& input );
        void ReadConsumption( xml::xistream& input );
        void ReadDotation( xml::xistream& input, const E_ConsumptionType& type );
        void WriteArchive( xml::xostream& output );

    public:
        T_ConsumptionItem_Vector vConsumptions_;
    };


    //*****************************************************************************
    class ComposanteInfos
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( ComposanteInfos )

    public:
        ComposanteInfos();
        ~ComposanteInfos();

        virtual std::string GetNodeName();
        std::string GetItemName();

        ComposanteInfos* CreateCopy();

        void ReadArchive( xml::xistream& input );
        void ReadSpeed( xml::xistream& input );
        void ReadSensor( xml::xistream& input );
        void ReadRadar( xml::xistream& input );
        void ReadWeapon( xml::xistream& input );
        void ReadObject( xml::xistream& input );
        void WriteArchive( xml::xostream& output );

    public:
        ADN_Type_String                                                           strName_;
        ADN_Type_String                                                           strAdditionalComments_;
        ADN_Type_Int                                                              nMosId_;

        ADN_Type_String                                                           strCodeEMAT6_;
        ADN_Type_String                                                           strCodeEMAT8_;
        ADN_Type_String                                                           strCodeLFRIL_;
        ADN_Type_String                                                           strCodeNNO_;

        ADN_TypePtr_InVector_ABC<ADN_Categories_Data::ArmorInfos>                 ptrArmor_;
        ADN_TypePtr_InVector_ABC<ADN_Categories_Data::SizeInfos>                  ptrSize_;
        ADN_Type_Double                                                           rWeight_;       
        ADN_Type_Double                                                           rMaxSpeed_;

        T_SpeedInfos_Vector                                                       vSpeeds_;
        T_WeaponInfos_Vector                                                      vWeapons_;
        T_SensorInfos_Vector                                                      vSensors_;
        T_RadarInfos_Vector                                                       vRadars_;
        T_ObjectInfos_Vector                                                      vObjects_;
        ConsumptionsInfos                                                         consumptions_;
        DotationInfos                                                             dotations_;

        BreakdownGroupInfos                                                       attritionBreakdowns_;
        BreakdownGroupInfos                                                       randomBreakdowns_;

        ADN_Type_Bool                                                             bTroopEmbarkingTimes_;
        ADN_Type_Time                                                             embarkingTimePerPerson_;
        ADN_Type_Time                                                             disembarkingTimePerPerson_;

        ADN_Type_Bool                                                             bCanCarryCargo_;
        ADN_Type_Double                                                           rWeightTransportCapacity_;
        ADN_Type_Time                                                             embarkingTimePerTon_;
        ADN_Type_Time                                                             disembarkingTimePerTon_;

        ADN_Type_Bool                                                             bMaxSlope_;
        ADN_Type_Double                                                           rMaxSlope_;

        LogInfos        logInfos_;


    public:
        class CmpId : public std::unary_function< ComposanteInfos* , bool >
        {
        public:
            CmpId( int val) : val_(val) {}
            ~CmpId() {}

            bool operator()( ComposanteInfos* tgtnfos ) const 
            { return tgtnfos->nMosId_.GetData() ==val_; }

        private:
            int val_;
        };
    };

    typedef ADN_Type_Vector_ABC<ComposanteInfos>    T_ComposanteInfos_Vector;
    typedef T_ComposanteInfos_Vector::iterator      IT_ComposanteInfos_Vector;


    //*****************************************************************************
public:
    explicit ADN_Composantes_Data();
    virtual ~ADN_Composantes_Data();

    void            FilesNeeded(T_StringList& l) const;
    void            Reset();

    T_ComposanteInfos_Vector&            GetComposantes();
    ComposanteInfos*                     FindComposante( const std::string& strName );

    int GetNextId();

    std::string GetComposantesThatUse( ADN_Sensors_Data::SensorInfos& sensor );
    std::string GetComposantesThatUse( ADN_Weapons_Data::WeaponInfos& weapon );


private:
    void ReadElement( xml::xistream& input );
    void ReadArchive( xml::xistream& input );
    void WriteArchive( xml::xostream& output );

public:
    int nNextId_;

    T_ComposanteInfos_Vector            vComposantes_;
};


//-----------------------------------------------------------------------------
// Name: ADN_Composantes_Data::T_ComposanteInfos_Vector
// Created: JDY 03-07-17
//-----------------------------------------------------------------------------
inline
ADN_Composantes_Data::T_ComposanteInfos_Vector&  ADN_Composantes_Data::GetComposantes()
{
    return vComposantes_;
}


// -----------------------------------------------------------------------------
// Name: ADN_Composantes_Data::FindComposante
// Created: APE 2004-12-01
// -----------------------------------------------------------------------------
inline
ADN_Composantes_Data::ComposanteInfos* ADN_Composantes_Data::FindComposante( const std::string& strName )
{
    IT_ComposanteInfos_Vector it = std::find_if( vComposantes_.begin(), vComposantes_.end(), ADN_Tools::NameCmp<ComposanteInfos>( strName ) );
    if( it == vComposantes_.end() )
        return 0;
    return *it;
}


#endif // __ADN_Composantes_Data_h_