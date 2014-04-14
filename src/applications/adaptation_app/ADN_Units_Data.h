//*****************************************************************************
//
// $Created: JDY 03-07-24 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Units_Data.h $
// $Author: Ape $
// $Modtime: 7/06/05 10:04 $
// $Revision: 14 $
// $Workfile: ADN_Units_Data.h $
//
//*****************************************************************************

#ifndef __ADN_Units_Data_h_
#define __ADN_Units_Data_h_

#include "ADN_Equipments_Data.h"
#include "ADN_Data_ABC.h"
#include "ADN_RefWithName.h"
#include "ADN_Enums.h"
#include "ADN_Models_Data.h"
#include "ADN_Types.h"
#include "ADN_Tools.h"
#include "ADN_Type_Repartition.h"
#include "ADN_Symbols_Data.h"

//*****************************************************************************
// Created: JDY 03-07-24
//*****************************************************************************
class ADN_Units_Data : public ADN_Data_ABC
{
public:
    //*****************************************************************************
    class ComposanteInfos : public ADN_CrossedRef< ADN_Equipments_Data::EquipmentInfos >
    {
    public:
        ComposanteInfos( const ADN_Equipments_Data::T_EquipmentInfos_Vector& equipments, ADN_Equipments_Data::EquipmentInfos* equipment = 0 );

        ComposanteInfos* CreateCopy();
        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;

    public:
        ADN_Type_Bool bMajor_;
        ADN_Type_Bool bLoadable_;
        ADN_Type_Bool bConveyor_;
        ADN_Type_Int nNbrHumanInCrew_;
        ADN_Type_Int nNb_;

    public:
        class Cmp : public std::unary_function< ComposanteInfos* , bool >
        {
        public:
            Cmp( const std::string& val ) : val_(val) {}
            ~Cmp() {}

            bool operator()( ComposanteInfos* tgtnfos ) const
            {
                return tgtnfos->GetCrossedElement() && tgtnfos->GetCrossedElement()->strName_ == val_;
            }
        private:
            std::string val_;
        };
    };
    typedef ADN_Type_Vector_ABC< ComposanteInfos > T_ComposanteInfos_Vector;

    //*****************************************************************************
    typedef ADN_Equipments_Data::ResourceInfos ResourceInfos;

    //*****************************************************************************

    class StockLogThresholdInfos : public ADN_CrossedRef< ADN_LogisticSupplyClasses_Data::LogisticSupplyClass >
    {

    public:
        explicit StockLogThresholdInfos();

        StockLogThresholdInfos* CreateCopy();
        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;

    public:
        ADN_Type_Double rLogThreshold_;
    };
    typedef ADN_Type_Vector_ABC< StockLogThresholdInfos > T_StockLogThresholdInfos_Vector;

    //*****************************************************************************
    class StockInfos : public ADN_Ref_ABC
    {
    public:
        StockInfos();

        void CopyFrom( StockInfos& src );
        void ReadArchive( xml::xistream& input, ADN_Type_Bool& stockThresholds );
        void ReadStock( xml::xistream& input, ADN_Type_Bool& stockThresholds );
        void WriteArchive( const std::string& strName, xml::xostream& output ) const;

    public:
        T_StockLogThresholdInfos_Vector vLogThresholds_;
    };

    //*****************************************************************************
    class PostureInfos : public ADN_Ref_ABC
    {
    public:
        explicit PostureInfos( const E_UnitPosture& posture );

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;

    public:
        E_UnitPosture nPosture_;
        ADN_Type_Time timeToActivate_;
        ADN_Type_Time timeToDeactivate_;

    public:
        class Cmp : public std::unary_function< PostureInfos*, bool >
        {
        public:
             Cmp( const E_UnitPosture& val ) : val_( val ) {}
             Cmp( const std::string& val ) : val_( E_UnitPosture( -12 ) ), str_( val ) {}
            ~Cmp() {}

            bool operator()( PostureInfos* tgtnfos ) const
            {
                return tgtnfos->nPosture_==val_ || ADN_Tools::ComputePostureScriptName( tgtnfos->nPosture_ ) == str_;
            }

        private:
            E_UnitPosture val_;
            std::string str_;
        };
    };
    typedef ADN_Type_Vector_ABC< PostureInfos > T_PostureInfos_Vector;

    //*****************************************************************************
    class PointInfos : public ADN_Ref_ABC
    {
    public:
        PointInfos();

        PointInfos* CreateCopy();
        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;

    public:
        E_KeyPoint nTypeTerrain_;  //$$$$
        ADN_Type_Int nDistance_;
    };
    typedef ADN_Type_Vector_ABC< PointInfos > T_PointInfos_Vector;

    //*****************************************************************************
    class UnitInfos : public ADN_RefWithLocalizedName
    {
    public:
                 UnitInfos();
                 UnitInfos( unsigned int id );
        virtual ~UnitInfos();

        void Initialize();
        UnitInfos* CreateCopy();
        void ReadArchive( xml::xistream& input );
        void ReadEquipment( xml::xistream& input );
        void ReadCrew( xml::xistream& input );
        void ReadPosture( xml::xistream& input );
        void ReadPointDistance( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;
        void CleanupNature();
        void CheckDatabaseValidity( ADN_ConsistencyChecker& checker ) const;

    public:
        ADN_Type_Int nId_;
        ADN_Type_Enum< E_AgentTypePion, eNbrAgentTypePion > eTypeId_;
        ADN_Type_Enum< E_AgentNbcSuit, eNbrAgentNbcSuit > eNbcSuit_;
        ADN_TypePtr_InVector_ABC< ADN_Models_ModelInfos > ptrModel_;
        ADN_Type_Enum< E_NatureLevel, eNbrNatureLevel > eNatureLevel_;
        ADN_Type_Enum< E_NatureAtlasType, eNbrNatureAtlasType > eNatureAtlas_;
        ADN_Type_String strNature_;
        ADN_Type_Int nNbOfficer_;        // officer
        ADN_Type_Int nNbNCOfficer_;      // non-commisioned officer
        ADN_Type_Time decontaminationDelay_;
        ADN_Type_Bool bCanFly_;
        ADN_Type_Int standardFlyingHeight_;
        ADN_Type_Int tacticalFlyingHeight_;
        ADN_Type_Enum< E_CrossingHeight, eNbrCrossingHeight > eCrossingHeight_;
        ADN_Type_Bool bIsAutonomous_;
        ADN_TypePtr_InVector_ABC< ADN_Symbols_Data::SymbolsUnit > ptrNatureSymbol_;
        ADN_Type_Double footprint_;

        T_ComposanteInfos_Vector vComposantes_;
        T_PostureInfos_Vector vPostures_;
        T_PointInfos_Vector vPointInfos_;

        ADN_Type_Bool bTC1_;
        ResourceInfos contenancesTC1_;
        ADN_Type_Bool bStock_;
        StockInfos stocks_;

        ADN_Type_Bool bProbe_;
        ADN_Type_Double rProbeWidth_;
        ADN_Type_Double rProbeLength_;

        ADN_Type_Bool bRanges_;
        ADN_Type_Int nSensorRange_;
        ADN_Type_Int nEquipmentRange_;

        ADN_Type_Bool bStrengthRatioFeedbackTime_;
        ADN_Type_Time strengthRatioFeedbackTime_;

        ADN_Type_Bool bInstallationDelay_;
        ADN_Type_Time installationDelay_;
        ADN_Type_Time uninstallationDelay_;

        ADN_Type_Int nFootprintRadius_;
        ADN_Type_Double rSpeedModifier_;

        ADN_Type_Int nReconEfficiency_;
        ADN_Type_Int nCombatSupportEfficiency_;
        ADN_Type_Int nCombatEfficiency_;
        ADN_Type_Int nMobilitySupportEfficiency_;
        ADN_Type_Int nCounterMobilitySupportEfficiency_;
        ADN_Type_Int nProtectionSupportEfficiency_;
        ADN_Type_Int nEngineeringReconEfficiency_;
        ADN_Type_Int nUrbanAreaEfficiency_;

        ADN_Type_Bool bIsCivilian_;
        ADN_Type_Repartition repartition_;
    };
    typedef ADN_Type_Vector_ABC< UnitInfos > T_UnitInfos_Vector;

    //*****************************************************************************
public:
             ADN_Units_Data();
    virtual ~ADN_Units_Data();

    void FilesNeeded( tools::Path::T_Paths& l ) const;

    T_UnitInfos_Vector& GetUnitsInfos();
    UnitInfos* FindUnit( const std::string& strName );

    QStringList GetUnitsThatUse( ADN_Equipments_Data::EquipmentInfos& composante );
    QStringList GetUnitsThatUse( ADN_Models_ModelInfos& model );
    QStringList GetUnitsThatUse( ADN_LogisticSupplyClasses_Data::LogisticSupplyClass& supply );

    virtual void CheckDatabaseValidity( ADN_ConsistencyChecker& checker ) const;

private:
    void ReadArchive( xml::xistream& input );
    void ReadUnit( xml::xistream& input );
    void WriteArchive( xml::xostream& output ) const;

private:
    T_UnitInfos_Vector  vUnits_;

private:
    static tools::IdManager idManager_;
};

//-----------------------------------------------------------------------------
// Name: ADN_Units_Data::T_UnitInfos_Vector
// Created: JDY 03-07-28
//-----------------------------------------------------------------------------
inline
ADN_Units_Data::T_UnitInfos_Vector& ADN_Units_Data::GetUnitsInfos()
{
    return vUnits_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Units_Data::FindUnit
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
inline
ADN_Units_Data::UnitInfos* ADN_Units_Data::FindUnit( const std::string& strName )
{
    auto it = std::find_if( vUnits_.begin(), vUnits_.end(), ADN_Tools::NameCmp( strName ) );
    if( it == vUnits_.end() )
        return 0;
    return *it;
}

#endif // __ADN_Units_Data_h_