// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef ADAPTATION_ADN_MISSIONS_TYPE_H
#define ADAPTATION_ADN_MISSIONS_TYPE_H

#include "ADN_Ref_ABC.h"
#include "ADN_Types.h"
#include <string>

namespace xml { class xostream; }

class ADN_Missions_Type : public ADN_Ref_ABC
{
public:
             ADN_Missions_Type();
    explicit ADN_Missions_Type( const std::string& name );
    virtual ~ADN_Missions_Type();

    std::string GetItemName();
    ADN_Missions_Type* CreateCopy();

    void WriteArchive( xml::xostream& output ) const;
public:
    std::string name_;
    std::string displayName_;
    ADN_Type_Bool isAllowed_;
};

#endif // ADAPTATION_ADN_MISSIONS_TYPE_H
