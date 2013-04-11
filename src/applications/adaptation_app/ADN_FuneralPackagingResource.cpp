// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_FuneralPackagingResource.h"
#include "ADN_Workspace.h"
#include "ADN_Resources_Data.h"
#include "ADN_Tools.h"
#include "ADN_Tr.h"

// -----------------------------------------------------------------------------
// Name: ADN_FuneralPackagingResource constructor
// Created: SBO 2006-08-04
// -----------------------------------------------------------------------------
ADN_FuneralPackagingResource::ADN_FuneralPackagingResource()
    : ADN_CrossedRef( ADN_Workspace::GetWorkspace().GetResources().GetData().GetResource( eDotationFamily_Funeraire ).GetCategories(), 0, true, "resource" )
    , processDuration_( "0s" )
    , terminal_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_FuneralPackagingResource destructor
// Created: SBO 2006-08-04
// -----------------------------------------------------------------------------
ADN_FuneralPackagingResource::~ADN_FuneralPackagingResource()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_FuneralPackagingResource::ReadArchive
// Created: SBO 2006-08-04
// -----------------------------------------------------------------------------
void ADN_FuneralPackagingResource::ReadArchive( xml::xistream& input )
{
    ADN_CrossedRef< ADN_Resources_Data::CategoryInfo >::ReadArchive( input );
    input >> xml::attribute( "process-duration", processDuration_ )
          >> xml::attribute( "terminal", terminal_ );
    if( GetCrossedElement() )
        SetVector( ADN_Workspace::GetWorkspace().GetResources().GetData().GetResource( GetCrossedElement()->parentResource_.nType_ ).GetCategories() );
}

// -----------------------------------------------------------------------------
// Name: ADN_FuneralPackagingResource::WriteArchive
// Created: SBO 2006-08-04
// -----------------------------------------------------------------------------
void ADN_FuneralPackagingResource::WriteArchive( xml::xostream& output )
{
    output << xml::start( "packaging" );
    ADN_CrossedRef< ADN_Resources_Data::CategoryInfo >::WriteArchive( output );
    output  << xml::attribute( "process-duration", processDuration_ )
            << xml::attribute( "terminal", terminal_ )
            << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_FuneralPackagingResource::ExchangeData
// Created: MMC 2011-12-07
// -----------------------------------------------------------------------------
void ADN_FuneralPackagingResource::ExchangeData( ADN_FuneralPackagingResource& packResource )
{
    ADN_Resources_Data::CategoryInfo* pcurResource = GetCrossedElement();
    ADN_Resources_Data::CategoryInfo** curResource = &pcurResource;
    std::string curProcessDuration  = processDuration_.GetData();
    bool curTerminal                = terminal_.GetData();

    if( packResource.GetCrossedElement() )
        SetVector( ADN_Workspace::GetWorkspace().GetResources().GetData().GetResource( packResource.GetCrossedElement()->parentResource_.nType_ ).GetCategories() );
    SetCrossedElement( packResource.GetCrossedElement(), false );
    processDuration_    = packResource.processDuration_.GetData();
    terminal_           = packResource.terminal_.GetData();

    packResource.SetVector( ADN_Workspace::GetWorkspace().GetResources().GetData().GetResource( ( *curResource )->parentResource_.nType_ ).GetCategories() );
    packResource.SetCrossedElement( *curResource, false );
    packResource.processDuration_   = curProcessDuration;
    packResource.terminal_          = curTerminal;
}
