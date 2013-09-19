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

#include "ADN_Missions_ABC.h"

enum E_EntityType;

namespace xml { class xistream; }

class ADN_Missions_Mission : public ADN_Missions_ABC
{
public:
             ADN_Missions_Mission();
    explicit ADN_Missions_Mission( E_MissionType type );
             ADN_Missions_Mission( E_MissionType type, unsigned int id );
    virtual ~ADN_Missions_Mission();

    virtual ADN_Missions_Mission* CreateCopy();

    virtual void ReadArchive( xml::xistream& input );
    virtual void WriteArchive( xml::xostream& output );

public:
    ADN_Type_String diaBehavior_;
    ADN_Type_String cdtDiaBehavior_;
    ADN_Type_String mrtDiaBehavior_;
    ADN_Type_String strPackage_;
    ADN_TypePtr_InVector_ABC< ADN_Drawings_Data::DrawingInfo > symbol_;
    std::string doctrine_;
    std::string usage_;
};

#endif // ADAPTATION_ADN_MISSIONS_MISSION_H
