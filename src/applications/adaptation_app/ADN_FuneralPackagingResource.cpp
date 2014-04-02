// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_FuneralPackagingResource.h"
#include "ADN_Workspace.h"
#include "ADN_Resources_Data.h"
#include "ADN_Tools.h"
#include "ADN_Tr.h"
#include "ADN_WorkspaceElement.h"
#include "protocol/Protocol.h"

// -----------------------------------------------------------------------------
// Name: ADN_FuneralPackagingResource constructor
// Created: SBO 2006-08-04
// -----------------------------------------------------------------------------
ADN_FuneralPackagingResource::ADN_FuneralPackagingResource()
    : ADN_CrossedRef( ADN_Workspace::GetWorkspace().GetResources().GetData().GetResource( sword::category_funeral ).GetCategories(), 0, true )
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
    SetCrossedElement( ADN_Workspace::GetWorkspace().GetResources().GetData().FindResourceCategory( input.attribute< std::string >( "resource" ) ) );
    input >> xml::attribute( "process-duration", processDuration_ )
          >> xml::attribute( "terminal", terminal_ );
    if( GetCrossedElement() )
        SetVector( ADN_Workspace::GetWorkspace().GetResources().GetData().GetResource( GetCrossedElement()->parentResource_.nType_ ).GetCategories() );
}

// -----------------------------------------------------------------------------
// Name: ADN_FuneralPackagingResource::WriteArchive
// Created: SBO 2006-08-04
// -----------------------------------------------------------------------------
void ADN_FuneralPackagingResource::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "packaging" )
             << xml::attribute( "resource", *this )
             << xml::attribute( "process-duration", processDuration_ )
             << xml::attribute( "terminal", terminal_ )
           << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_FuneralPackagingResource::ExchangeData
// Created: MMC 2011-12-07
// -----------------------------------------------------------------------------
void ADN_FuneralPackagingResource::ExchangeData( ADN_FuneralPackagingResource& packResource )
{
    ADN_CrossedRef< ADN_Resources_Data::CategoryInfo >::Swap( packResource );

    std::string curProcessDuration = processDuration_.GetData();
    bool curTerminal = terminal_.GetData();

    processDuration_ = packResource.processDuration_.GetData();
    terminal_ = packResource.terminal_.GetData();

    packResource.processDuration_ = curProcessDuration;
    packResource.terminal_ = curTerminal;
}
