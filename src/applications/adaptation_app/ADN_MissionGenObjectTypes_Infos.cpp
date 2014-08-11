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
#include "ADN_WorkspaceElement.h"

using namespace helpers;

// -----------------------------------------------------------------------------
// Name: ADN_MissionGenObjectTypes_Infos constructor
// Created: LGY 2012-04-18
// -----------------------------------------------------------------------------
ADN_MissionGenObjectTypes_Infos::ADN_MissionGenObjectTypes_Infos( ADN_Objects_Data_ObjectInfos* ptr )
    : ADN_CrossedRef< ADN_Objects_Data_ObjectInfos >( ADN_Workspace::GetWorkspace().GetObjects().GetData().GetObjectInfos(), ptr, true )
    , isAllowed_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_MissionGenObjectTypes_Infos constructor
// Created: LGY 2012-04-19
// -----------------------------------------------------------------------------
ADN_MissionGenObjectTypes_Infos::ADN_MissionGenObjectTypes_Infos()
    : ADN_CrossedRef< ADN_Objects_Data_ObjectInfos >( ADN_Workspace::GetWorkspace().GetObjects().GetData().GetObjectInfos(), 0, true )
    , isAllowed_( false )
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
void ADN_MissionGenObjectTypes_Infos::WriteArchive( xml::xostream& output ) const
{
    if( isAllowed_.GetData() )
    {
        output << xml::start( "parameter" )
                  << xml::attribute( "type", GetCrossedElement()->strType_ )
               << xml::end;
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_MissionGenObjectTypes_Infos::CreateCopy
// Created: LGY 2012-04-19
// -----------------------------------------------------------------------------
ADN_MissionGenObjectTypes_Infos* ADN_MissionGenObjectTypes_Infos::CreateCopy()
{
    ADN_MissionGenObjectTypes_Infos* infos = new ADN_MissionGenObjectTypes_Infos( GetCrossedElement() );
    infos->isAllowed_ = isAllowed_.GetData();
    return infos;
}

// -----------------------------------------------------------------------------
// Name: T_MissionGenObjectTypes_Infos_Vector::T_MissionGenObjectTypes_Infos_Vector
// Created: ABR 2013-09-25
// -----------------------------------------------------------------------------
T_MissionGenObjectTypes_Infos_Vector::T_MissionGenObjectTypes_Infos_Vector()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: T_MissionGenObjectTypes_Infos_Vector::~T_MissionGenObjectTypes_Infos_Vector
// Created: ABR 2013-09-25
// -----------------------------------------------------------------------------
T_MissionGenObjectTypes_Infos_Vector::~T_MissionGenObjectTypes_Infos_Vector()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: T_MissionGenObjectTypes_Infos_Vector::AutoCreatePrivate
// Created: ABR 2013-09-25
// -----------------------------------------------------------------------------
void T_MissionGenObjectTypes_Infos_Vector::AutoCreatePrivate( void* ptr )
{
    if( ptr && std::find_if( begin(), end(), ADN_MissionGenObjectTypes_Infos::CmpRef( ( ADN_MissionGenObjectTypes_Infos::T_Item* ) ptr ) ) == end() )
    {
        ADN_MissionGenObjectTypes_Infos* pNewItem = new ADN_MissionGenObjectTypes_Infos( ( ADN_MissionGenObjectTypes_Infos::T_Item* ) ptr );
        pNewItem->isAllowed_ = IsAllChecked();
        AddItemPrivate( pNewItem );
        AddItem( 0 );
        emit ItemAdded( pNewItem );
    }
}

// -----------------------------------------------------------------------------
// Name: T_MissionGenObjectTypes_Infos_Vector::IsAllChecked
// Created: ABR 2013-09-25
// -----------------------------------------------------------------------------
bool T_MissionGenObjectTypes_Infos_Vector::IsAllChecked() const
{
    for( auto it = begin(); it != end(); ++it )
        if( !( *it )->isAllowed_.GetData() )
            return false;
    return true;
}
