// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Objects_Wizard.h"
#include "ADN_Project_Data.h"
#include "ADN_Workspace.h"
#include "ADN_Objects_Data.h"
#include "ADN_Wizard_FirstPage_Default.h"
#include "ADN_Wizard_LastPage.h"

typedef ADN_Objects_Data_ObjectInfos ObjectInfos;

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Wizard constructor
// Created: LDC 2010-06-21
// -----------------------------------------------------------------------------
ADN_Objects_Wizard::ADN_Objects_Wizard( QWidget* pParent, const char* szName )
: ADN_Wizard_ABC<ObjectInfos>( pParent, szName )
{
    setCaption( qApp->translate( "ADN_Objects_Wizard", "New object creation" ) );

    ADN_Objects_Data::T_ObjectsInfos_Vector& objects = ADN_Workspace::GetWorkspace().GetObjects().GetData().GetObjectInfos();
    ADN_Wizard_FirstPage_Default<ObjectInfos>* pFirstPage = new ADN_Wizard_FirstPage_Default<ObjectInfos>( objects, this );
    pFirstPage->SetCaptions( qApp->translate( "ADN_Objects_Wizard", "New object creation" ), qApp->translate( "ADN_Objects_Wizard", "Object" ) );
    pFirstPage_ = pFirstPage;

    new ADN_Wizard_LastPage( this, qApp->translate( "ADN_Objects_Wizard",  "Creation finished" ),
        qApp->translate( "ADN_Objects_Wizard", "Click \"Done\" to create the new object." ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Wizard destructor
// Created: LDC 2010-06-21
// -----------------------------------------------------------------------------
ADN_Objects_Wizard::~ADN_Objects_Wizard()
{
}

// -----------------------------------------------------------------------------
// Name: ADN_Objects_Wizard::CreateObject
// Created: LDC 2010-06-21
// -----------------------------------------------------------------------------
ADN_Ref_ABC* ADN_Objects_Wizard::CreateObject()
{
    ADN_Ref_ABC* object = ADN_Wizard_ABC_ADN_Object_Data_ObjectInfos::CreateObject();
    if( object )
    {
        ADN_Objects_Data_ObjectInfos* info = static_cast<ADN_Objects_Data_ObjectInfos*>( object );
        if( !info->strType_.GetData().empty() )
            ADN_Workspace::GetWorkspace().GetProject().addedObjects_[ info->strName_.GetData() ] = info->strType_.GetData();
        info->strType_ = info->strName_.GetData();
    }
    return object;
}
