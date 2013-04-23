// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef ADAPTATION_ADN_MISSIONS_FRAGORDER_H
#define ADAPTATION_ADN_MISSIONS_FRAGORDER_H

#include "ADN_Missions_ABC.h"

class ADN_Missions_FragOrder : public ADN_Missions_ABC
{
public:
             ADN_Missions_FragOrder();
    explicit ADN_Missions_FragOrder( unsigned int id );
    virtual ~ADN_Missions_FragOrder();

    ADN_Missions_FragOrder* CreateCopy();

    void ReadArchive( xml::xistream& input, const tools::Path& missionDir );
    void WriteArchive( xml::xostream& output, E_MissionType type );

public:
    ADN_Type_Bool isAvailableWithoutMission_;
};

#endif // ADAPTATION_ADN_MISSIONS_FRAGORDER_H
