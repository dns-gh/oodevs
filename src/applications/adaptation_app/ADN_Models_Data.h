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
        explicit OrderInfos( const ADN_Missions_Data::T_Mission_Vector& missions, ADN_Missions_ABC* mission = 0 );
                 OrderInfos( ADN_Missions_FragOrder* fragorder, const std::string& name );

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;
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
        void WriteArchive( xml::xostream& output ) const;

    public:
        T_OrderInfos_Vector  vOrders_;
    };

    typedef ADN_Type_Vector_ABC< MissionInfos > T_MissionInfos_Vector;

//*****************************************************************************
    class ModelInfos : public ADN_RefWithLocalizedName
    {

    public:
                 ModelInfos();
        explicit ModelInfos( E_EntityType type );
        virtual ~ModelInfos();

        ModelInfos* CreateCopy();
        void ReadArchive( xml::xistream& input );
        void ReadMission( xml::xistream& input );
        void ReadOrder( xml::xistream& input );
        void WriteArchive( const std::string& type, xml::xostream& output );
        void AddFragOrder( ADN_Missions_FragOrder* fragorder, const std::string& order );
        void RemoveFragOder( const std::string& order );

    public:
        ADN_Type_String strDiaType_;
        ADN_Type_Path strFile_;
        ADN_Type_Bool isMasalife_;
        T_MissionInfos_Vector vMissions_;
        T_OrderInfos_Vector vFragOrders_;
        E_EntityType type_;
    };

    typedef ADN_Type_Vector_ABC<ModelInfos> T_ModelInfos_Vector;

//*****************************************************************************
    typedef std::vector< std::wstring > T_SourcePaths;
//*****************************************************************************

public:
    explicit ADN_Models_Data();
    virtual ~ADN_Models_Data();

    virtual void FilesNeeded( tools::Path::T_Paths& l ) const;
    virtual void Initialize();
    virtual void CheckDatabaseValidity( ADN_ConsistencyChecker& checker ) const;

    const T_SourcePaths& GetSourcePaths() const;
    QStringList GetModelsThatUse( E_EntityType type, ADN_Missions_Mission& model );
    QStringList GetModelsThatUse( E_EntityType type, ADN_Missions_FragOrder& fragOrder );

    T_ModelInfos_Vector& GetModels( E_EntityType type );
    ModelInfos* FindModel( E_EntityType type, const std::string& strName );

private:
    void ReadArchive( xml::xistream& input );
    void ReadModels( xml::xistream& input, E_EntityType type );
    void ReadSourcePath( xml::xistream& xis );
    void WriteArchive( xml::xostream& output ) const;

private:
    T_ModelInfos_Vector vModels_[ eNbrEntityType ];
    std::vector< std::wstring > sourcePaths_;
};

// -----------------------------------------------------------------------------
// Name: ADN_Models_Data::GetModels
// Created: ABR 2013-08-23
// -----------------------------------------------------------------------------
inline
ADN_Models_Data::T_ModelInfos_Vector& ADN_Models_Data::GetModels( E_EntityType type )
{
    return vModels_[ type ];
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_Data::FindModel
// Created: ABR 2013-08-23
// -----------------------------------------------------------------------------
inline
ADN_Models_Data::ModelInfos* ADN_Models_Data::FindModel( E_EntityType type, const std::string& strName )
{
    auto it = std::find_if( vModels_[ type ].begin(), vModels_[ type ].end(), ADN_Tools::NameCmp( strName ) );
    if( it == vModels_[ type ].end() )
        return 0;
    return *it;
}

// -----------------------------------------------------------------------------
// Name: ADN_Models_Data::GetSourcePath
// Created: ABR 2013-09-13
// -----------------------------------------------------------------------------
inline
const ADN_Models_Data::T_SourcePaths& ADN_Models_Data::GetSourcePaths() const
{
    return sourcePaths_;
}

#endif // __ADN_Models_Data_h_