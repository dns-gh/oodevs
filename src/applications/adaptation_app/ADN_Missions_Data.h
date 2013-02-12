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

enum E_EntityType;

namespace xml { class xistream; }

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
    typedef ADN_Type_Vector_ABC<ADN_Missions_ParameterValue>  T_MissionParameterValue_Vector;
    typedef T_MissionParameterValue_Vector::iterator         IT_MissionParameterValue_Vector;

    typedef ADN_Type_Vector_ABC<ADN_Missions_Type>            T_Choice_Vector;
    typedef T_Choice_Vector::iterator                        IT_Choice_Vector;
    typedef T_Choice_Vector::const_iterator                 CIT_Choice_Vector;

    typedef ADN_Type_Vector_ABC<ADN_Missions_Parameter>       T_MissionParameter_Vector;
    typedef T_MissionParameter_Vector::iterator              IT_MissionParameter_Vector;
    typedef T_MissionParameter_Vector::const_iterator       CIT_MissionParameter_Vector;

    typedef ADN_Type_Vector_ABC<ADN_Missions_Mission>         T_Mission_Vector;
    typedef T_Mission_Vector::iterator                       IT_Mission_Vector;
    typedef T_Mission_Vector::const_iterator                CIT_Mission_Vector;

    typedef ADN_Type_Vector_ABC<ADN_Missions_FragOrder>       T_FragOrder_Vector;
    typedef T_FragOrder_Vector::iterator                     IT_FragOrder_Vector;
    typedef T_FragOrder_Vector::const_iterator                    CIT_FragOrder_Vector;

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
    virtual void FilesNeeded( T_StringList& vFiles ) const;
    virtual void Reset();
    virtual void CheckDatabaseValidity( ADN_ConsistencyChecker& checker ) const;

    T_FragOrder_Vector&     GetFragOrders();
    T_Mission_Vector&       GetUnitMissions();
    T_Mission_Vector&       GetAutomatMissions();
    T_Mission_Vector&       GetPopulationMissions();
    ADN_Missions_FragOrder* FindFragOrder( const std::string& strName );
    ADN_Missions_Mission* FindMission( const T_Mission_Vector& missions, const std::string& strName );
    virtual void Load( const tools::Loader_ABC& fileLoader );
    virtual void Initialize();

    QStringList         GetAllMissionsThatUse( ADN_Objects_Data_ObjectInfos& object ); // $$$$ ABR 2012-08-03: Warning, return not only the name, but concatenation of tab name and mission name

    QStringList         GetUnitMissionsThatUse( ADN_Objects_Data_ObjectInfos& object );
    QStringList         GetAutomatMissionsThatUse( ADN_Objects_Data_ObjectInfos& object );
    QStringList         GetPopulationMissionsThatUse( ADN_Objects_Data_ObjectInfos& object );
    QStringList         GetFragOrdersThatUse( ADN_Objects_Data_ObjectInfos& object );

    void NotifyElementDeleted( std::string elementName, E_EntityType elementType );
    QString GenerateMissionSheet( int index, const QString& text );
    //@}

private:
    void ReadArchive( xml::xistream& input );
    void ReadFragOrder( xml::xistream& input );
    void ReadMission( xml::xistream& input, T_Mission_Vector& missions, E_EntityType modelType );
    void WriteArchive( xml::xostream& output );
    void MoveMissionSheetsToObsolete( std::string file );

public:
    T_Mission_Vector    unitMissions_;
    T_Mission_Vector    automatMissions_;
    T_Mission_Vector    populationMissions_;
    T_FragOrder_Vector  fragOrders_;
    T_StringList        toDeleteMissionSheets_;

public:
    static tools::IdManager idManager_;
};

#endif // __ADN_Missions_Data_h_
