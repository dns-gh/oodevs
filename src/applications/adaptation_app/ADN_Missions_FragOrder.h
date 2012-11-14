// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef ADAPTATION_ADN_MISSIONS_FRAGORDER_H
#define ADAPTATION_ADN_MISSIONS_FRAGORDER_H

#include "ADN_Data_ABC.h"
#include "ADN_RefWithName.h"
#include "ADN_Types.h"
#include "ADN_Enums.h"
#include "ADN_Missions_ParameterValue.h"
#include "ADN_Missions_Parameter.h"

namespace xml
{
    class xistream;
    class xostream;
}

class ADN_Missions_FragOrder : public ADN_RefWithName
{
public:
    typedef ADN_Type_Vector_ABC<ADN_Missions_Parameter>       T_MissionParameter_Vector;
    typedef T_MissionParameter_Vector::iterator               IT_MissionParameter_Vector;

             ADN_Missions_FragOrder();
    explicit ADN_Missions_FragOrder( unsigned int id );
    virtual ~ADN_Missions_FragOrder();

    std::string GetItemName();
    ADN_Missions_FragOrder* CreateCopy();

    void ReadArchive( xml::xistream& input, const std::string& baseDir, const std::string& missionDir );
    void ReadParameter( xml::xistream& input );
    void WriteArchive( xml::xostream& output );

    void ReadMissionSheet( const std::string& baseDir, const std::string& missionDir );
    void RemoveDifferentNamedMissionSheet( const std::string& baseDir, const std::string& missionDir );
    void WriteMissionSheet( const std::string& baseDir, const std::string& missionDir );

public:
    ADN_Type_Int              id_;
    T_MissionParameter_Vector parameters_;
    ADN_Type_String           diaType_;
    ADN_Type_Bool             isAvailableWithoutMission_;
    ADN_Type_String           missionSheetContent_;

private:
    // Relative path to base missions directory
    std::string missionSheetPath_;
};

#endif // ADAPTATION_ADN_MISSIONS_FRAGORDER_H
