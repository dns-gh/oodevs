// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_MissionGenObjectTypes_Infos.h"
#include "ADN_Objects_Data.h"

using namespace helpers;

// -----------------------------------------------------------------------------
// Name: ADN_MissionGenObjectTypes_Infos constructor
// Created: LGY 2012-04-18
// -----------------------------------------------------------------------------
ADN_MissionGenObjectTypes_Infos::ADN_MissionGenObjectTypes_Infos( ADN_Objects_Data_ObjectInfos* ptr )
    : ptrObject_( ADN_Workspace::GetWorkspace().GetObjects().GetData().GetObjectInfos(), ptr )
    , isAllowed_( false )
    , name_     ( ptrObject_.GetData()->strName_ )
    , type_     ( ptrObject_.GetData()->strType_ )
{
    BindExistenceTo( &ptrObject_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_MissionGenObjectTypes_Infos constructor
// Created: LGY 2012-04-19
// -----------------------------------------------------------------------------
ADN_MissionGenObjectTypes_Infos::ADN_MissionGenObjectTypes_Infos()
    : ptrObject_( ADN_Workspace::GetWorkspace().GetObjects().GetData().GetObjectInfos(), 0 )
    , isAllowed_( false )
    , name_     ( ptrObject_.GetData()->strType_ )
    , type_     ( ptrObject_.GetData()->strType_ )
{
    // NOTHING
}

// ----------------------------------------------------------------------------
// Name: ADN_MissionGenObjectTypes_Infos destructor
// Created: LGY 2012-04-18
// -----------------------------------------------------------------------------
ADN_MissionGenObjectTypes_Infos::~ADN_MissionGenObjectTypes_Infos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_MissionGenObjectTypes_Infos::WriteArchive
// Created: LGY 2012-04-18
// -----------------------------------------------------------------------------
void ADN_MissionGenObjectTypes_Infos::WriteArchive( xml::xostream& output )
{
    if( isAllowed_.GetData() )
    {
        output << xml::start( "parameter" )
                  << xml::attribute( "type", type_ )
               << xml::end;
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_MissionGenObjectTypes_Infos::CreateCopy
// Created: LGY 2012-04-19
// -----------------------------------------------------------------------------
ADN_MissionGenObjectTypes_Infos* ADN_MissionGenObjectTypes_Infos::CreateCopy()
{
    ADN_MissionGenObjectTypes_Infos* infos = new ADN_MissionGenObjectTypes_Infos();
    infos->ptrObject_ = ptrObject_.GetData();
    infos->name_ = name_.GetData();
    infos->type_ = type_.GetData();
    infos->isAllowed_ = isAllowed_.GetData();
    return infos;
}

// -----------------------------------------------------------------------------
// Name: ADN_MissionGenObjectTypes_Infos::GetItemName
// Created: ABR 2013-01-08
// -----------------------------------------------------------------------------
std::string ADN_MissionGenObjectTypes_Infos::GetItemName()
{
    return "GenObjectType";
}

// -----------------------------------------------------------------------------
// Name: ADN_MissionGenObjectTypes_Infos::GetInputName
// Created: LDC 2013-07-01
// -----------------------------------------------------------------------------
const std::string& ADN_MissionGenObjectTypes_Infos::GetInputName() const
{
    return type_.GetData();
}