// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Missions_Type.h"
#include "ADN_Objects_Data.h"

ADN_Missions_Type::ADN_Missions_Type()
{
    // NOTHING
}

ADN_Missions_Type::ADN_Missions_Type( const std::string& name )
        : name_( name )
        , displayName_( qApp->translate( "ADN_Tr", name.c_str() ).toStdString() )
        , isAllowed_( false )
{
    // NOTHING
}

ADN_Missions_Type::~ADN_Missions_Type()
{
    // NOTHING
}

std::string ADN_Missions_Type::GetItemName()
{
    return name_;
}

ADN_Missions_Type* ADN_Missions_Type::CreateCopy()
{
    ADN_Missions_Type* newType = new ADN_Missions_Type();
    newType->name_ = name_;
    newType->displayName_ = displayName_;
    newType->isAllowed_ = isAllowed_.GetData();
    return newType;
}

void ADN_Missions_Type::WriteArchive( xml::xostream& output ) const
{
    if( isAllowed_.GetData() )
    {
        output << xml::start( "parameter" )
                 << xml::attribute( "type", name_ )
               << xml::end;
    }
}
