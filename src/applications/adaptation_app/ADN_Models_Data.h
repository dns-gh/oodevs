// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Models_Data_h_
#define __ADN_Models_Data_h_

#include "ADN_Data_ABC.h"

#include "ADN_Types.h"
#include "ADN_Type_Vector_ABC.h"
#include "ADN_Enums.h"
#include "ADN_Tools.h"
#include "ADN_Missions_Data.h"

namespace xml { class xistream; }

//*****************************************************************************
// Created: JDY 03-07-24
//*****************************************************************************
class ADN_Models_Data : public ADN_Data_ABC
{

public:
//*****************************************************************************
    class OrderInfos
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {

    public:
        OrderInfos();
        OrderInfos( ADN_Missions_Data::FragOrder* fragorder, const std::string& name );

        std::string GetItemName();

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output );
        OrderInfos* CreateCopy();

    public:
        ADN_TypePtr_InVector_ABC< ADN_Missions_Data::FragOrder > fragOrder_;
        ADN_Type_String                                          strName_; // do not use directly !!!
    };

    typedef ADN_Type_Vector_ABC<OrderInfos> T_OrderInfos_Vector;
    typedef T_OrderInfos_Vector::iterator   IT_OrderInfos_Vector;


//*****************************************************************************
    class MissionInfos
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {

    public:
        explicit MissionInfos( ADN_Missions_Data::T_Mission_Vector& missions );
        virtual ~MissionInfos();

        std::string GetItemName();
        virtual std::string GetNodeName();

        MissionInfos* CreateCopy();

        void ReadArchive( xml::xistream& input );
        void ReadFragOrder( xml::xistream& input );
        void WriteArchive( xml::xostream& output );

    public:
        ADN_TypePtr_InVector_ABC< ADN_Missions_Data::Mission > mission_;
        ADN_Type_String                          strName_; // do not use directly !!!
        T_OrderInfos_Vector                      vOrders_;
    };

    typedef ADN_Type_Vector_ABC<MissionInfos> T_MissionInfos_Vector;
    typedef T_MissionInfos_Vector::iterator   IT_MissionInfos_Vector;


//*****************************************************************************
    class ModelInfos
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {

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
        explicit ModelInfos( ADN_Missions_Data::T_Mission_Vector& missions );
        virtual ~ModelInfos();

        virtual std::string GetNodeName();
        std::string GetItemName();

        ModelInfos* CreateCopy();

        void ReadArchive( xml::xistream& input );
        void ReadMission( xml::xistream& input );
        void ReadOrder( xml::xistream& input );
        void WriteArchive( const std::string& type, xml::xostream& output );
        void AddFragOrder( ADN_Missions_Data::FragOrder* fragorder, const std::string& order );
        void RemoveFragOder( const std::string& order );

    public:
        ADN_Missions_Data::T_Mission_Vector& missions_;
        ADN_Type_String                     strName_;
        ADN_Type_String                     strDiaType_;
        ADN_Type_String                     strFile_;
        ADN_Type_Bool                       isMasalife_;
        T_MissionInfos_Vector               vMissions_;
        T_OrderInfos_Vector                 vFragOrders_;
    };

    typedef ADN_Type_Vector_ABC<ModelInfos> T_ModelInfos_Vector;
    typedef T_ModelInfos_Vector::iterator   IT_ModelInfos_Vector;


//*****************************************************************************
public:
    explicit ADN_Models_Data();
    virtual ~ADN_Models_Data();

    void            FilesNeeded(T_StringList& l) const;
    void            Reset();
    std::string     GetModelsThatUse( ModelInfos::E_ModelEntityType type, ADN_Missions_Data::Mission& model );

    T_ModelInfos_Vector&    GetUnitModelsInfos();
    ModelInfos*             FindUnitModel( const std::string& strName );

    T_ModelInfos_Vector&    GetAutomataModelsInfos();
    ModelInfos*             FindAutomataModel( const std::string& strName );

    T_ModelInfos_Vector&    GetPopulationModelsInfos();
    ModelInfos*             FindPopulationModel( const std::string& strName );

private:
    void ReadArchive( xml::xistream& input );
    void ReadAutomat( xml::xistream& input );
    void ReadPopulation( xml::xistream& input );
    void ReadUnit( xml::xistream& input );

    void WriteArchive( xml::xostream& output );

private:
    T_ModelInfos_Vector     vUnitModels_;
    T_ModelInfos_Vector     vAutomataModels_;
    T_ModelInfos_Vector     vPopulationModels_;
};


// -----------------------------------------------------------------------------
// Name: ADN_Models_Data::GetModelsThatUse
// Created: ABR 2011-09-29
// -----------------------------------------------------------------------------
inline
std::string ADN_Models_Data::GetModelsThatUse( ModelInfos::E_ModelEntityType type, ADN_Missions_Data::Mission& mission )
{
    std::string strResult = "";
    T_ModelInfos_Vector* currentVector = 0;
    if( type == ModelInfos::ePawn )
        currentVector = &vUnitModels_;
    else if( type == ModelInfos::eAutomat )
        currentVector = &vAutomataModels_;
    else
        currentVector = &vPopulationModels_;
    for( IT_ModelInfos_Vector it = currentVector->begin(); it != currentVector->end(); ++it )
    {
        ModelInfos* pModel = *it;
        if( !pModel )
            continue;
        for( IT_MissionInfos_Vector missionIt = pModel->vMissions_.begin(); missionIt != pModel->vMissions_.end(); ++missionIt )
            if( ( *missionIt )->mission_ == &mission )
            {
                if( strResult != "" )
                    strResult += "<br>";
                strResult += "<nobr>" + pModel->strName_.GetData() + "</nobr>";
                break;
            }
    }
    return strResult;
}

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