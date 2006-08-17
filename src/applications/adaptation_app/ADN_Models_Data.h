//*****************************************************************************
//
// $Created: JDY 03-07-24 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Models_Data.h $
// $Author: Ape $
// $Modtime: 7/04/05 11:21 $
// $Revision: 7 $
// $Workfile: ADN_Models_Data.h $
//
//*****************************************************************************

#ifndef __ADN_Models_Data_h_
#define __ADN_Models_Data_h_

#include "ADN_Data_ABC.h"

#include "ADN_Types.h"
#include "ADN_Type_Vector_ABC.h"
#include "ADN_Enums.h"
#include "ADN_Tools.h"

class ADN_XmlInput_Helper;

//*****************************************************************************
// Created: JDY 03-07-24
//*****************************************************************************
class ADN_Models_Data : public ADN_Data_ABC
{
    MT_COPYNOTALLOWED( ADN_Models_Data )
public:
//*****************************************************************************
    class OrderInfos
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( OrderInfos )

    public:
        OrderInfos();
        
        std::string GetItemName();

        void ReadArchive( ADN_XmlInput_Helper& input );
        void WriteArchive( MT_OutputArchive_ABC& output );

    public:
        ADN_Type_Enum< E_FragOrder, eNbrFragOrder >    nOrderType_;
        ADN_Type_String                     strName_;           // do not use directly !!!

    public:
        class Cmp : public std::unary_function< OrderInfos* , bool >
        {
        public:
            Cmp(E_FragOrder val) : val_(val) {}
            ~Cmp() {}

            bool operator()( OrderInfos* tgtnfos ) const 
            { return tgtnfos->nOrderType_.GetData()==val_; }

        private:
            E_FragOrder val_;
        };
    };
    
    typedef ADN_Type_Vector_ABC<OrderInfos> T_OrderInfos_Vector;
    typedef T_OrderInfos_Vector::iterator   IT_OrderInfos_Vector;


//*****************************************************************************
    class MissionInfos
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( MissionInfos )
            
    public:
        MissionInfos();
        ~MissionInfos();

        std::string GetItemName();
        virtual std::string GetNodeName();

        MissionInfos* CreateCopy();

        void ReadArchive( ADN_XmlInput_Helper& input );
        void WriteArchive( MT_OutputArchive_ABC& output );

    public:
        ADN_Type_Enum< E_SMission,eNbrSMissionTest> nMissionType_;
        ADN_Type_String                          strName_;           // do not use directly !!!
        T_OrderInfos_Vector                      vOrders_;

    public:
        class Cmp : public std::unary_function< MissionInfos* , bool >
        {
        public:
            Cmp(E_SMission val) : val_(val) {}
            ~Cmp() {}

            bool operator()( MissionInfos* tgtnfos ) const 
            { return tgtnfos->nMissionType_.GetData()==val_; }

        private:
            E_SMission val_;
        };
    };

    typedef ADN_Type_Vector_ABC<MissionInfos> T_MissionInfos_Vector;
    typedef T_MissionInfos_Vector::iterator   IT_MissionInfos_Vector;


//*****************************************************************************
    class ModelInfos
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( ModelInfos )

    public:
        enum E_ModelEntityType
        {
            ePawn,
            eAutomat,
            ePopulation,
            eNbrModelEntityTypes
        };

    public:
        ModelInfos();
        virtual ~ModelInfos();

        virtual std::string GetNodeName();
        std::string GetItemName();

        ModelInfos* CreateCopy();

        void ReadArchive( ADN_XmlInput_Helper& input );
        void WriteArchive( MT_OutputArchive_ABC& output );

    public:
        ADN_Type_String                     strName_;
        ADN_Type_String                     strDiaType_;
        ADN_Type_String                     strFile_;

        T_MissionInfos_Vector               vMissions_;
    };

    typedef ADN_Type_Vector_ABC<ModelInfos> T_ModelInfos_Vector;
    typedef T_ModelInfos_Vector::iterator   IT_ModelInfos_Vector;


//*****************************************************************************
public:
    explicit ADN_Models_Data();
    virtual ~ADN_Models_Data();

    void            FilesNeeded(T_StringList& l) const;
    void            Reset();

    T_ModelInfos_Vector&    GetUnitModelsInfos();
    ModelInfos*             FindUnitModel( const std::string& strName );

    T_ModelInfos_Vector&    GetAutomataModelsInfos();
    ModelInfos*             FindAutomataModel( const std::string& strName );

    T_ModelInfos_Vector&    GetPopulationModelsInfos();
    ModelInfos*             FindPopulationModel( const std::string& strName );

private:
    void ReadArchive( ADN_XmlInput_Helper& input );
    void WriteArchive( MT_OutputArchive_ABC& output );

    std::string ComputeFileName() const;

private:
    T_ModelInfos_Vector     vUnitModels_;
    T_ModelInfos_Vector     vAutomataModels_;
    T_ModelInfos_Vector     vPopulationModels_;
};


//-----------------------------------------------------------------------------
// Name: ADN_Models_Data::GetUnitModelsInfos
// Created: JDY 03-07-24
//-----------------------------------------------------------------------------
inline
ADN_Models_Data::T_ModelInfos_Vector& ADN_Models_Data::GetUnitModelsInfos()
{
    return vUnitModels_;
}


// -----------------------------------------------------------------------------
// Name: ADN_Models_Data::FindUnitModel
// Created: APE 2004-12-01
// -----------------------------------------------------------------------------
inline
ADN_Models_Data::ModelInfos* ADN_Models_Data::FindUnitModel( const std::string& strName )
{
    IT_ModelInfos_Vector it = std::find_if( vUnitModels_.begin(), vUnitModels_.end(), ADN_Tools::NameCmp<ModelInfos>( strName ) );
    if( it == vUnitModels_.end() )
        return 0;
    return *it;
}


// -----------------------------------------------------------------------------
// Name: ADN_Models_Data::GetAutomataModelsInfos
// Created: APE 2004-12-01
// -----------------------------------------------------------------------------
inline
ADN_Models_Data::T_ModelInfos_Vector& ADN_Models_Data::GetAutomataModelsInfos()
{
    return vAutomataModels_;
}


// -----------------------------------------------------------------------------
// Name: ADN_Models_Data::FindAutomataModel
// Created: APE 2004-12-01
// -----------------------------------------------------------------------------
inline
ADN_Models_Data::ModelInfos* ADN_Models_Data::FindAutomataModel( const std::string& strName )
{
    IT_ModelInfos_Vector it = std::find_if( vAutomataModels_.begin(), vAutomataModels_.end(), ADN_Tools::NameCmp<ModelInfos>( strName ) );
    if( it == vAutomataModels_.end() )
        return 0;
    return *it;
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_Data::GetPopulationModelsInfos
// Created: APE 2004-12-01
// -----------------------------------------------------------------------------
inline
ADN_Models_Data::T_ModelInfos_Vector& ADN_Models_Data::GetPopulationModelsInfos()
{
    return vPopulationModels_;
}


// -----------------------------------------------------------------------------
// Name: ADN_Models_Data::FindPopulationModel
// Created: APE 2004-12-01
// -----------------------------------------------------------------------------
inline
ADN_Models_Data::ModelInfos* ADN_Models_Data::FindPopulationModel( const std::string& strName )
{
    IT_ModelInfos_Vector it = std::find_if( vPopulationModels_.begin(), vPopulationModels_.end(), ADN_Tools::NameCmp<ModelInfos>( strName ) );
    if( it == vPopulationModels_.end() )
        return 0;
    return *it;
}


#endif // __ADN_Models_Data_h_