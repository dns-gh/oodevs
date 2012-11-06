// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef ADAPTATION_ADN_MISSIONS_MISSION_H
#define ADAPTATION_ADN_MISSIONS_MISSION_H

#include "ADN_RefWithName.h"
#include "ADN_Types.h"
#include "ADN_Enums.h"
#include "ADN_Drawings_Data.h"
#include "ADN_Missions_Parameter.h"

enum E_EntityType;

namespace xml { class xistream; }

class ADN_Missions_Mission : public ADN_RefWithName
{
public:
    typedef ADN_Type_Vector_ABC<ADN_Missions_Parameter>       T_MissionParameter_Vector;
    typedef T_MissionParameter_Vector::iterator               IT_MissionParameter_Vector;

             ADN_Missions_Mission();
    explicit ADN_Missions_Mission( unsigned int id );
    virtual ~ADN_Missions_Mission();

    std::string GetItemName();
    ADN_Missions_Mission* CreateCopy();

    void ReadArchive ( xml::xistream& input, std::size_t contextLength, ADN_Drawings_Data& drawings,
        const std::string& baseDir, const std::string& missionDir );
    void ReadParameter( xml::xistream& input, std::size_t& index, std::size_t contextLength );
    void WriteArchive( xml::xostream& output, const std::string& type, const T_MissionParameter_Vector& context );

    void ReadMissionSheet ( const std::string& baseDir, const std::string& missionDir );
    void RemoveDifferentNamedMissionSheet ( const std::string& baseDir, const std::string& missionDir );
    void WriteMissionSheet ( const std::string& baseDir, const std::string& missionDir );
    bool IsValidDatabase();

public:
    ADN_Type_Int id_;
    T_MissionParameter_Vector parameters_;
    ADN_Type_String diaType_;
    ADN_Type_String diaBehavior_;
    ADN_Type_String cdtDiaBehavior_;
    ADN_Type_String mrtDiaBehavior_;
    ADN_Type_String missionSheetContent_;
    ADN_Type_String strPackage_;
    ADN_TypePtr_InVector_ABC< ADN_Drawings_Data::DrawingInfo > symbol_;

private:
    // Relative path to base missions directory
    std::string missionSheetPath_;
};

#endif // ADAPTATION_ADN_MISSIONS_MISSION_H
