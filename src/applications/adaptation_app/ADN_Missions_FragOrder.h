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
#include "ADN_Types.h"
#include "ADN_Enums.h"
#include "ADN_Missions_ParameterValue.h"
#include "ADN_Missions_Parameter.h"

namespace xml
{
    class xistream;
    class xostream;
}

class ADN_Missions_FragOrder : public ADN_Ref_ABC
                             , public ADN_DataTreeNode_ABC
{
public:
    typedef ADN_Type_Vector_ABC<ADN_Missions_Parameter>       T_MissionParameter_Vector;
    typedef T_MissionParameter_Vector::iterator               IT_MissionParameter_Vector;

             ADN_Missions_FragOrder();
    explicit ADN_Missions_FragOrder( unsigned int id );
    virtual ~ADN_Missions_FragOrder();

    std::string GetItemName();
    ADN_Missions_FragOrder* CreateCopy();

    void ReadArchive ( xml::xistream& input );
    void ReadParameter( xml::xistream& input );
    void WriteArchive( xml::xostream& output );

    void ReadMissionSheet ();
    void RemoveDifferentNamedMissionSheet ();
    void WriteMissionSheet ();

public:
    ADN_Type_Int              id_;
    ADN_Type_String           strName_;
    T_MissionParameter_Vector parameters_;
    ADN_Type_String           diaType_;
    ADN_Type_Bool             isAvailableWithoutMission_;
    ADN_Type_String           missionSheetContent_;
    ADN_Type_String           missionSheetPath_;
};

#endif // ADAPTATION_ADN_MISSIONS_FRAGORDER_H
