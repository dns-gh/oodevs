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
#include "ADN_Type_Vector_ABC.h"
#include "ADN_Models_ModelInfos.h"

enum E_EntityType;
class ADN_Missions_FragOrder;
class ADN_Missions_Mission;

//*****************************************************************************
// Created: JDY 03-07-24
//*****************************************************************************
class ADN_Models_Data : public ADN_Data_ABC
{
public:
    typedef std::vector< std::wstring > T_SourcePaths;

public:
    explicit ADN_Models_Data();
    virtual ~ADN_Models_Data();

    virtual void FilesNeeded( tools::Path::T_Paths& l ) const;
    virtual void Initialize();
    virtual void CheckDatabaseValidity( ADN_ConsistencyChecker& checker ) const;

    const T_SourcePaths& GetSourcePaths() const;
    QStringList GetModelsThatUse( E_EntityType type, ADN_Missions_Mission& model );
    QStringList GetModelsThatUse( E_EntityType type, ADN_Missions_FragOrder& fragOrder );

    ADN_Type_Vector_ABC< ADN_Models_ModelInfos >& GetModels( E_EntityType type );
    ADN_Models_ModelInfos* FindModel( E_EntityType type, const std::string& strName );

private:
    void ReadArchive( xml::xistream& input );
    void ReadModels( xml::xistream& input, E_EntityType type );
    void ReadSourcePath( xml::xistream& xis );
    void WriteArchive( xml::xostream& output ) const;

private:
    ADN_Type_Vector_ABC< ADN_Models_ModelInfos > vModels_[ eNbrEntityType ];
    std::vector< std::wstring > sourcePaths_;
};

#endif // __ADN_Models_Data_h_