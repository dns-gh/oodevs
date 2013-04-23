// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Missions_Data_h_
#define __ADN_Missions_Data_h_

#include "ADN_Data_ABC.h"
#include "ADN_Types.h"
#include "ADN_Enums.h"
#include "ADN_Drawings_Data.h"
#include "ADN_MissionGenObjectTypes_Infos.h"
#include "ADN_Missions_ParameterValue.h"
#include "ADN_Missions_Parameter.h"
#include "ADN_Missions_Type.h"
#include "ADN_Missions_Mission.h"
#include "ADN_Missions_FragOrder.h"
#include "tools/IdManager.h"

enum E_MissionType;

// =============================================================================
/** @class  ADN_Missions_Data
    @brief  ADN_Missions_Data
*/
// Created: APE 2005-03-14
// =============================================================================
class ADN_Missions_Data : public ADN_Data_ABC
{
// =============================================================================
// Mission parameters
// =============================================================================
public:
    typedef ADN_Type_Vector_ABC< ADN_Missions_ParameterValue > T_MissionParameterValue_Vector;
    typedef ADN_Type_Vector_ABC< ADN_Missions_Type> T_Choice_Vector;
    typedef ADN_Type_Vector_ABC< ADN_Missions_Parameter > T_MissionParameter_Vector;
    typedef ADN_Type_Vector_ABC< ADN_Missions_ABC > T_Mission_Vector;

// =============================================================================
// Main data
// =============================================================================
public:
    //! @name Constructors/Destructor
    //@{
             ADN_Missions_Data();
    virtual ~ADN_Missions_Data();
    //@}

    //! @name Operations
    //@{
    virtual void FilesNeeded( tools::Path::T_Paths& vFiles ) const;
    virtual void Reset();
    virtual void CheckDatabaseValidity( ADN_ConsistencyChecker& checker ) const;

    T_Mission_Vector& GetMissions( E_MissionType type );

    ADN_Missions_ABC* FindMission( const T_Mission_Vector& missions, const std::string& strName ) const;
    ADN_Missions_ABC* FindMission( int missionType, const std::string& strName ) const;
    virtual void Initialize();

    QStringList GetAllMissionsThatUse( ADN_Objects_Data_ObjectInfos& object ); // $$$$ ABR 2012-08-03: Warning, return not only the name, but concatenation of tab name and mission name
    QStringList GetMissionsThatUse( E_MissionType type, ADN_Objects_Data_ObjectInfos& object );

    void NotifyElementDeleted( std::string elementName, E_MissionType missionType );
    tools::Path GenerateMissionSheet( int index, const QString& text );
    //@}

private:
    void ReadArchive( xml::xistream& input );
    void ReadMission( xml::xistream& input, E_MissionType modelType );
    void WriteArchive( xml::xostream& output );
    void MoveMissionSheetsToObsolete( const tools::Path& file ) const;
    void CheckAndFixLoadingErrors() const;

public:
    std::vector< std::pair< std::string, T_Mission_Vector > > missionsVector_;
    tools::Path::T_Paths toDeleteMissionSheets_;

public:
    static tools::IdManager idManager_;
    static tools::Path imagePath_;
    static tools::Path imageTemporaryPath_;
    static tools::Path xslTemporaryFile_;
    static tools::Path missionSheetTemporaryFile_;
};

#endif // __ADN_Missions_Data_h_
