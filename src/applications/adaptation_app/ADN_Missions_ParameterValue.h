// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef ADAPTATION_ADN_MISSIONS_DATA_PARAMETERVALUE_H
#define ADAPTATION_ADN_MISSIONS_DATA_PARAMETERVALUE_H

#include "ADN_RefWithName.h"
#include "ADN_Types.h"
#include "ADN_Type_LocalizedString.h"

namespace xml
{
    class xistream;
    class xostream;
}

class ADN_Missions_ParameterValue : public ADN_RefWithLocalizedName
{
public:
    explicit ADN_Missions_ParameterValue( E_MissionType type );
    virtual ~ADN_Missions_ParameterValue();

    std::string GetItemName();
    ADN_Missions_ParameterValue* CreateCopy();

    void ReadArchive ( xml::xistream& input );
    void WriteArchive( xml::xostream& output ) const;

public:
    ADN_Type_Int id_;
    E_MissionType missionType_;
};

#endif // ADAPTATION_ADN_MISSIONS_DATA_PARAMETERVALUE_H
