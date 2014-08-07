// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
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
    Q_OBJECT

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
    virtual void CheckDatabaseValidity( ADN_ConsistencyChecker& checker ) const;

    T_Mission_Vector& GetMissions( E_MissionType type );

    ADN_Missions_ABC* FindMission( const T_Mission_Vector& missions, const std::string& strName ) const;
    ADN_Missions_ABC* FindMission( int missionType, const std::string& strName ) const;
    virtual void Initialize();

    QStringList GetAllMissionsThatUse( ADN_Objects_Data_ObjectInfos& object ); // $$$$ ABR 2012-08-03: Warning, return not only the name, but concatenation of tab name and mission name
    QStringList GetMissionsThatUse( E_MissionType type, ADN_Objects_Data_ObjectInfos& object );

    void GenerateMissionSheet( int index, boost::shared_ptr< kernel::LocalizedString > text );
    const boost::shared_ptr< kernel::Context >& GetMissionSheetPathContext() const;
    const boost::shared_ptr< kernel::Context >& GetMissionSheetContext( E_MissionType type ) const;
    //@}

    //! @name Static Helpers
    //@{
    static tools::Path GetMissionSheetsPath();
    static tools::Path GetMissionSheetsPath( int index );
    static tools::Path GetLocalizedMissionSheetsPath( const std::string& language, const tools::Path& basePath );
    static tools::Path GetMissionSheetsImagesPath( int index );
    static tools::Path GetCssFile();

    static tools::Path GetTemporaryPath();
    static tools::Path GetTemporaryPath( int index );
    static tools::Path GetTemporaryImagesPath( int index );
    static tools::Path GetTemporaryCssFile();
    //@}

public slots:
    //! @name Slots
    //@{
    void OnElementDeleted( boost::shared_ptr< kernel::LocalizedString > elementName, E_MissionType missionType );
    //@}

private:
    virtual bool ApplyOnTranslations( const std::function< bool( kernel::LocalizedString& ) >& functor ) const;

    void AddMissionType( const std::string& xmltag );
    void ReadArchive( xml::xistream& input );
    void ReadMission( xml::xistream& input, E_MissionType modelType );
    bool FixConsistency();
    void WriteArchive( xml::xostream& output ) const;
    void MoveMissionSheetsToObsolete( const tools::Path& file ) const;
    void CheckAndFixLoadingErrors() const;
    void DeleteMissionSheet( const tools::Path& filename );

private:
    std::vector< std::pair< std::string, T_Mission_Vector > > missionsVector_;
    std::vector< boost::shared_ptr< kernel::Context > > missionSheetContexts_;
    boost::shared_ptr< kernel::Context > missionSheetPathContext_;

public:
    static tools::IdManager idManager_;
    static tools::Path xslTemporaryFile_;
    static tools::Path imagePath_;
    static tools::Path temporaryPath_;
    static tools::Path cssFile_;
    static tools::Path obsoletePath_;
};

#endif // __ADN_Missions_Data_h_
