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
#include "ADN_Equipement_Data.h"
#include "ADN_DataException.h"
#include "ADN_Tools.h"
#include "ADN_Tr.h"

// -----------------------------------------------------------------------------
// Name: ADN_FuneralPackagingResource constructor
// Created: SBO 2006-08-04
// -----------------------------------------------------------------------------
ADN_FuneralPackagingResource::ADN_FuneralPackagingResource()
    : processDuration_( "0s" )
    , terminal_( false )
{
    BindExistenceTo( &resource_ );
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
// Name: ADN_FuneralPackagingResource::GetNodeName
// Created: SBO 2006-08-04
// -----------------------------------------------------------------------------
std::string ADN_FuneralPackagingResource::GetNodeName()
{
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: ADN_FuneralPackagingResource::GetItemName
// Created: SBO 2006-08-04
// -----------------------------------------------------------------------------
std::string ADN_FuneralPackagingResource::GetItemName()
{
    return std::string();
}

// -----------------------------------------------------------------------------
// Name: ADN_FuneralPackagingResource::ReadArchive
// Created: SBO 2006-08-04
// -----------------------------------------------------------------------------
void ADN_FuneralPackagingResource::ReadArchive( xml::xistream& input )
{
    std::string resourceName;
    input >> xml::attribute( "resource", resourceName )
          >> xml::attribute( "process-duration", processDuration_ )
          >> xml::attribute( "terminal", terminal_ );
    ADN_Equipement_Data::CategoryInfo* resource = ADN_Workspace::GetWorkspace().GetEquipements().GetData().FindEquipementCategory( resourceName );
    if( resource == 0 )
        throw ADN_DataException( tools::translate( "Funeral_Data", "Invalid data" ).toAscii().constData(), tools::translate( "Funeral_Data", "Invalid resource '%1'" ).arg( resourceName.c_str() ).toAscii().constData() );
    resource_ = resource;
}

// -----------------------------------------------------------------------------
// Name: ADN_FuneralPackagingResource::WriteArchive
// Created: SBO 2006-08-04
// -----------------------------------------------------------------------------
void ADN_FuneralPackagingResource::WriteArchive( xml::xostream& output )
{
    output << xml::start( "packaging" )
            << xml::attribute( "resource", resource_.GetData()->strName_ )
            << xml::attribute( "process-duration", processDuration_ )
            << xml::attribute( "terminal", terminal_ )
            << xml::end();
}

// -----------------------------------------------------------------------------
// Name: ADN_FuneralPackagingResource::ExchangeData
// Created: MMC 2011-12-07
// -----------------------------------------------------------------------------
void ADN_FuneralPackagingResource::ExchangeData( ADN_FuneralPackagingResource& packResource )
{
    ADN_Equipement_Data::CategoryInfo* curResource = resource_.GetData();
    std::string curProcessDuration  = processDuration_.GetData();
    bool curTerminal                = terminal_.GetData();

    resource_.SetData( packResource.resource_.GetData(), true );
    processDuration_    = packResource.processDuration_.GetData();
    terminal_           = packResource.terminal_.GetData();

    packResource.resource_.SetData( curResource, true );
    packResource.processDuration_   = curProcessDuration;
    packResource.terminal_          = curTerminal;
}
