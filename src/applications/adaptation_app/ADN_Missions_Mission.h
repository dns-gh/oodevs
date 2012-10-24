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

#include "ADN_Ref_ABC.h"
#include "ADN_DataTreeNode_ABC.h"
#include "ADN_Types.h"
#include "ADN_Enums.h"
#include "ADN_Drawings_Data.h"
#include "ADN_Missions_Parameter.h"

enum E_EntityType;

namespace xml { class xistream; }

class ADN_Missions_Mission : public ADN_Ref_ABC
                           , public ADN_DataTreeNode_ABC
{
public:
    typedef ADN_Type_Vector_ABC<ADN_Missions_Parameter>       T_MissionParameter_Vector;
    typedef T_MissionParameter_Vector::iterator               IT_MissionParameter_Vector;

             ADN_Missions_Mission();
    explicit ADN_Missions_Mission( unsigned int id );
    virtual ~ADN_Missions_Mission();

    std::string GetItemName();
    ADN_Missions_Mission* CreateCopy();

    void ReadArchive ( xml::xistream& input, std::size_t contextLength, E_EntityType modelType );
    void ReadParameter( xml::xistream& input, std::size_t& index, std::size_t contextLength );
    void WriteArchive( xml::xostream& output, const std::string& type, const T_MissionParameter_Vector& context );

    void ReadMissionSheet ( E_EntityType type );
    void RemoveDifferentNamedMissionSheet ( E_EntityType type );
    void WriteMissionSheet ( E_EntityType type );
    std::string FromEntityTypeToRepository( E_EntityType type);

public:
    ADN_Type_Int id_;
    ADN_Type_String strName_;
    T_MissionParameter_Vector parameters_;
    ADN_Type_String diaType_;
    ADN_Type_String diaBehavior_;
    ADN_Type_String cdtDiaBehavior_;
    ADN_Type_String mrtDiaBehavior_;
    ADN_Type_String missionSheetContent_;
    ADN_Type_String missionSheetPath_;
    ADN_Type_String strPackage_;
    ADN_TypePtr_InVector_ABC< ADN_Drawings_Data::DrawingInfo > symbol_;
};

#endif // ADAPTATION_ADN_MISSIONS_MISSION_H
