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
#include "ADN_CrossedRef.h"
#include "ADN_Types.h"
#include "ADN_Type_Vector_ABC.h"
#include "ADN_Enums.h"
#include "ADN_Tools.h"
#include "ADN_Missions_Data.h"
#include "ADN_CrossedRef.h"

enum E_EntityType;

//*****************************************************************************
// Created: JDY 03-07-24
//*****************************************************************************
class ADN_Models_Data : public ADN_Data_ABC
{
public:
//*****************************************************************************
    class OrderInfos : public ADN_CrossedRef< ADN_Missions_ABC >
    {

    public:
        OrderInfos();
        OrderInfos( ADN_Missions_FragOrder* fragorder, const std::string& name );

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output );
        OrderInfos* CreateCopy();
    };

    typedef ADN_Type_Vector_ABC< OrderInfos > T_OrderInfos_Vector;

//*****************************************************************************
    class MissionInfos : public ADN_CrossedRef< ADN_Missions_ABC >
    {

    public:
        explicit MissionInfos( const ADN_Missions_Data::T_Mission_Vector& missions, ADN_Missions_ABC* mission = 0 );
        virtual ~MissionInfos();

        MissionInfos* CreateCopy();
        void ReadArchive( xml::xistream& input );
        void ReadFragOrder( xml::xistream& input );
        void WriteArchive( xml::xostream& output );

    public:
        T_OrderInfos_Vector  vOrders_;
    };

    typedef ADN_Type_Vector_ABC< MissionInfos > T_MissionInfos_Vector;

//*****************************************************************************
    class ModelInfos : public ADN_RefWithName
    {

    public:
                 ModelInfos();
        explicit ModelInfos( ADN_Missions_Data::T_Mission_Vector& missions );
        virtual ~ModelInfos();

        ModelInfos* CreateCopy();
        void ReadArchive( xml::xistream& input );
        void ReadMission( xml::xistream& input );
        void ReadOrder( xml::xistream& input );
        void WriteArchive( const std::string& type, xml::xostream& output );
        void AddFragOrder( ADN_Missions_FragOrder* fragorder, const std::string& order );
        void RemoveFragOder( const std::string& order );

    public:
        ADN_Missions_Data::T_Mission_Vector& missions_;
        ADN_Type_String strDiaType_;
        ADN_Type_Path strFile_;
        ADN_Type_Bool isMasalife_;
        T_MissionInfos_Vector vMissions_;
        T_OrderInfos_Vector vFragOrders_;
    };

    typedef ADN_Type_Vector_ABC<ModelInfos> T_ModelInfos_Vector;

//*****************************************************************************
public:
    explicit ADN_Models_Data();
    virtual ~ADN_Models_Data();

    virtual void FilesNeeded( tools::Path::T_Paths& l ) const;
    virtual void Reset();
    virtual void Initialize();
    virtual void CheckDatabaseValidity( ADN_ConsistencyChecker& checker ) const;
    
    QStringList GetModelsThatUse( E_EntityType type, ADN_Missions_Mission& model );
    QStringList GetModelsThatUse( E_EntityType type, ADN_Missions_FragOrder& fragOrder );

    T_ModelInfos_Vector& GetUnitModelsInfos();
    ModelInfos* FindUnitModel( const std::string& strName );

    T_ModelInfos_Vector& GetAutomataModelsInfos();
    ModelInfos* FindAutomataModel( const std::string& strName );

    T_ModelInfos_Vector& GetPopulationModelsInfos();
    ModelInfos* FindPopulationModel( const std::string& strName );

private:
    void ReadArchive( xml::xistream& input );
    void ReadAutomat( xml::xistream& input );
    void ReadPopulation( xml::xistream& input );
    void ReadUnit( xml::xistream& input );

    void WriteArchive( xml::xostream& output );

private:
    T_ModelInfos_Vector vUnitModels_;
    T_ModelInfos_Vector vAutomataModels_;
    T_ModelInfos_Vector vPopulationModels_;
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
    auto it = std::find_if( vUnitModels_.begin(), vUnitModels_.end(), ADN_Tools::NameCmp( strName ) );
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
    auto it = std::find_if( vAutomataModels_.begin(), vAutomataModels_.end(), ADN_Tools::NameCmp( strName ) );
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
    auto it = std::find_if( vPopulationModels_.begin(), vPopulationModels_.end(), ADN_Tools::NameCmp( strName ) );
    if( it == vPopulationModels_.end() )
        return 0;
    return *it;
}

#endif // __ADN_Models_Data_h_