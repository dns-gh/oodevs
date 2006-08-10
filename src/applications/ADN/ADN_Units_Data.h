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

#include "ADN_Data_ABC.h"

#include "ADN_Types.h"
#include "ADN_Tools.h"
#include "ADN_Enums.h"
#include "ADN_Composantes_Data.h"
#include "ADN_Models_Data.h"

//*****************************************************************************
// Created: JDY 03-07-24
//*****************************************************************************
class ADN_Units_Data : public ADN_Data_ABC
{
    MT_COPYNOTALLOWED( ADN_Units_Data )

public:
    //*****************************************************************************
    class ComposanteInfos
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( ComposanteInfos )

    public:
        ComposanteInfos();

        virtual std::string GetNodeName();
        std::string GetItemName();

        ComposanteInfos* CreateCopy();

        void ReadArchive( ADN_XmlInput_Helper& input );
        void WriteArchive( MT_OutputArchive_ABC& output );

    public:
        ADN_TypePtr_InVector_ABC<ADN_Composantes_Data::ComposanteInfos> ptrComposante_;
        ADN_Type_Bool                                                   bMajor_;
        ADN_Type_Bool                                                   bLoadable_;
        ADN_Type_Bool                                                   bConveyor_;
        ADN_Type_Int                                                    nNbrHumanInCrew_;
        ADN_Type_Int                                                    nNb_;

    public:
        class Cmp : public std::unary_function< ComposanteInfos* , bool >
        {
        public:
            Cmp(const std::string& val) : val_(val) {}
            ~Cmp() {}

            bool operator()( ComposanteInfos* tgtnfos ) const
            { return tgtnfos->ptrComposante_.GetData() && tgtnfos->ptrComposante_.GetData()->strName_==val_; }

        private:
            std::string val_;
        };
    };

    typedef ADN_Type_Vector_ABC<ComposanteInfos>    T_ComposanteInfos_Vector;
    typedef T_ComposanteInfos_Vector::iterator      IT_ComposanteInfos_Vector;


    //*****************************************************************************
    typedef ADN_Composantes_Data::DotationInfos DotationInfos;

    //*****************************************************************************

    class StockLogThresholdInfos
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( StockLogThresholdInfos )

    public:
        StockLogThresholdInfos( E_StockCategory eCategory = ( E_StockCategory )-1 );

        virtual std::string GetNodeName();
        std::string GetItemName();

        StockLogThresholdInfos* CreateCopy();

        void ReadArchive( ADN_XmlInput_Helper& input );
        void WriteArchive( MT_OutputArchive_ABC& output );

    public:
        ADN_Type_Enum< E_StockCategory, eNbrStockCategory > eCategory_;
        ADN_Type_Double                                     rLogThreshold_;
    };

    typedef ADN_Type_Vector_ABC< StockLogThresholdInfos > T_StockLogThresholdInfos_Vector;
    typedef T_StockLogThresholdInfos_Vector::iterator     IT_StockLogThresholdInfos_Vector;

    //*****************************************************************************
    class StockInfos
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( StockInfos )

    public:
        StockInfos();

        void CopyFrom( StockInfos& src );

        virtual std::string GetNodeName();
        std::string GetItemName();

        void ReadArchive( ADN_XmlInput_Helper& input );
        void WriteArchive( const std::string& strName, MT_OutputArchive_ABC& output );

    public:
        T_StockLogThresholdInfos_Vector vLogThresholds_;
    };

    //*****************************************************************************
    class PostureInfos
        : public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( PostureInfos )

    public:
        PostureInfos(const E_UnitPosture& posture);

        virtual std::string GetNodeName();
        std::string GetItemName();

        void ReadArchive( ADN_XmlInput_Helper& input );
        void WriteArchive( MT_OutputArchive_ABC& output );

    public:
        E_UnitPosture  nPosture_;
        ADN_Type_Time  timeToActivate_;

    public:
        class Cmp : public std::unary_function< PostureInfos* , bool >
        {
        public:
            Cmp(const E_UnitPosture& val) : val_(val) {}
            ~Cmp() {}

            bool operator()( PostureInfos* tgtnfos ) const
            { return tgtnfos->nPosture_==val_; }

        private:
            E_UnitPosture val_;
        };
    };

    typedef ADN_Type_Vector_ABC<PostureInfos>    T_PostureInfos_Vector;
    typedef T_PostureInfos_Vector::iterator      IT_PostureInfos_Vector;


    //*****************************************************************************
    class PointInfos
        : public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( PointInfos )

    public:
        PointInfos();

        virtual std::string GetNodeName();
        std::string GetItemName();

        PointInfos* CreateCopy();

        void ReadArchive( ADN_XmlInput_Helper& input );
        void WriteArchive( MT_OutputArchive_ABC& output );

    public:
        E_KeyPoint   nTypeTerrain_;  //$$$$
        ADN_Type_Int nDistance_;
    };

    typedef ADN_Type_Vector_ABC< PointInfos >    T_PointInfos_Vector;
    typedef T_PointInfos_Vector::iterator        IT_PointInfos_Vector;


    //*****************************************************************************
    class UnitInfos
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( UnitInfos )

    public:
        UnitInfos();
        virtual ~UnitInfos();

        virtual std::string GetNodeName();
        std::string GetItemName();

        UnitInfos* CreateCopy();

        void ReadArchive( ADN_XmlInput_Helper& input );
        void WriteArchive( MT_OutputArchive_ABC& output );

    public:
        ADN_Type_Enum<E_AgentTypePion,eNbrAgentTypePion>            eTypeId_;
        ADN_Type_String                                             strName_;
        ADN_TypePtr_InVector_ABC<ADN_Models_Data::ModelInfos>       ptrModel_;
        ADN_Type_Int                                                nMosId_;
        ADN_Type_Enum<E_NatureLevel,eNbrNatureLevel>                eNatureLevel_;
        ADN_Type_Enum<E_UnitNatureWeapon,eNbrUnitNatureWeapon>          eNatureWeapon_;
        ADN_Type_Enum<E_UnitNatureSpecialization,eNbrUnitNatureSpecialization>    eNatureSpec_;
        ADN_Type_Enum<E_UnitNatureQualifier,eNbrUnitNatureQualifier>    eNatureQualifier_;
        ADN_Type_Enum<E_UnitNatureCategory,eNbrUnitNatureCategory>      eNatureCategory_;
        ADN_Type_Enum<E_UnitNatureMobility,eNbrUnitNatureMobility>      eNatureMobility_;
        ADN_Type_Enum<E_NatureAtlasType,eNbrNatureAtlas>            eNatureAtlas_;
        ADN_Type_Enum<E_CapacityMission,eNbrCapacityMission>        eMissionCapacity_;    
        ADN_Type_Int                                                nNbOfficer_;        // officer
        ADN_Type_Int                                                nNbNCOfficer_;      // non-commisioned officer
        ADN_Type_Time                                               decontaminationDelay_;
        ADN_Type_Double                                             rWeaponsReach_;
        ADN_Type_Double                                             rSensorsReach_;
        ADN_Type_Bool                                               bCanFly_;
        ADN_Type_Bool                                               bIsAutonomous_;

        T_ComposanteInfos_Vector                                    vComposantes_;
        T_PostureInfos_Vector                                       vPostures_;
        T_PointInfos_Vector                                         vPointInfos_;

        ADN_Type_Bool                                               bTC1_;
        DotationInfos                                               contenancesTC1_;
        ADN_Type_Bool                                               bStock_;
        StockInfos                                                  stocks_;

        ADN_Type_Bool                                               bProbe_;
        ADN_Type_Double                                             rProbeWidth_;
        ADN_Type_Double                                             rProbeLength_;

        ADN_Type_Bool                                               bStrengthRatioFeedbackTime_;
        ADN_Type_Time                                               strengthRatioFeedbackTime_;

        ADN_Type_Bool                                               bInstallationDelay_;
        ADN_Type_Time                                               installationDelay_;
        ADN_Type_Time                                               uninstallationDelay_;
    };

    typedef ADN_Type_Vector_ABC<UnitInfos>    T_UnitInfos_Vector;
    typedef T_UnitInfos_Vector::iterator      IT_UnitInfos_Vector;


    //*****************************************************************************
public:
    explicit ADN_Units_Data();
    virtual ~ADN_Units_Data();

    void            FilesNeeded(T_StringList& l) const;
    void            Reset();

    T_UnitInfos_Vector& GetUnitsInfos();
    UnitInfos*          FindUnit( const std::string strName );

    int GetNextId();

    std::string GetUnitsThatUse( ADN_Composantes_Data::ComposanteInfos& composante );

private:
    void ReadArchive( ADN_XmlInput_Helper& input );
    void WriteArchive( MT_OutputArchive_ABC& output );

private:
    int nNextId_;

    T_UnitInfos_Vector  vUnits_;
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
ADN_Units_Data::UnitInfos* ADN_Units_Data::FindUnit( const std::string strName )
{
    IT_UnitInfos_Vector it = std::find_if( vUnits_.begin(), vUnits_.end(), ADN_Tools::NameCmp<UnitInfos>( strName ) );
    if( it == vUnits_.end() )
        return 0;
    return *it;
}


#endif // __ADN_Units_Data_h_