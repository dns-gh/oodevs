// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_GUI_Container.h"
#include "ADN_Data_Container.h"

// -----------------------------------------------------------------------------
// Name: ADN_GUI_Container constructor
// Created: ABR 2012-01-18
// -----------------------------------------------------------------------------
ADN_GUI_Container::ADN_GUI_Container( const char* szClassName, ADN_Data_Container& data )
    : ADN_GUI_ABC( szClassName )
    , data_( data )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_GUI_Container destructor
// Created: ABR 2012-01-18
// -----------------------------------------------------------------------------
ADN_GUI_Container::~ADN_GUI_Container()
{
    for( IT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        delete it->second.second;
    elements_.clear();
}

// -----------------------------------------------------------------------------
// Name: ADN_GUI_Container::Build
// Created: ABR 2012-01-18
// -----------------------------------------------------------------------------
void ADN_GUI_Container::Build()
{
    for( IT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        it->second.second->Build();
}

// -----------------------------------------------------------------------------
// Name: ADN_GUI_Container::RegisterTable
// Created: ABR 2012-01-18
// -----------------------------------------------------------------------------
void ADN_GUI_Container::RegisterTable( ADN_MainWindow& mainWindow )
{
    for( IT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        it->second.second->RegisterTable( mainWindow );
}

// -----------------------------------------------------------------------------
// Name: ADN_GUI_Container::ExportHtml
// Created: ABR 2012-01-18
// -----------------------------------------------------------------------------
void ADN_GUI_Container::ExportHtml( ADN_HtmlBuilder& mainIndexBuilder, const QString& strPath )
{
    for( IT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        it->second.second->ExportHtml( mainIndexBuilder, strPath );
}

// -----------------------------------------------------------------------------
// Name: ADN_GUI_Container::GetTitle
// Created: ABR 2012-01-24
// -----------------------------------------------------------------------------
const QString& ADN_GUI_Container::GetTitle( int index )
{
    CIT_Elements it = elements_.find( index );
    assert( it != elements_.end() );
    return it->second.first;
}
