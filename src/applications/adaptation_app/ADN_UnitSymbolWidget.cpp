// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_UnitSymbolWidget.h"
#include "moc_ADN_UnitSymbolWidget.cpp"
#include "ADN_Symbols_Data.h"
#include "ADN_Project_Data.h"
#include "ADN_WorkspaceElement.h"

// -----------------------------------------------------------------------------
// Name: ADN_SymbolWidget constructor
// Created: MMC 2013-04-09
// -----------------------------------------------------------------------------
ADN_UnitSymbolWidget::ADN_UnitSymbolWidget( QWidget* parent, T_PixmapExtractor pixmapAccessor, T_StringExtractor symbolAccessor /*= &ADN_Symbols_Data::SymbolsUnit::GetSymbol*/ )
    : ADN_SymbolWidget_ABC( parent )
{
    pixmapAccessor_ = pixmapAccessor;
    symbolAccessor_ = symbolAccessor;
}

// -----------------------------------------------------------------------------
// Name: ADN_SymbolWidget destructor
// Created: MMC 2013-04-09
// -----------------------------------------------------------------------------
ADN_UnitSymbolWidget::~ADN_UnitSymbolWidget()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_SymbolWidget::OnNatureChanged
// Created: MMC 2013-04-09
// -----------------------------------------------------------------------------
void ADN_UnitSymbolWidget::OnNatureChanged( const QString& nature )
{
    ADN_Symbols_Data::SymbolsUnit* symbol = ADN_Workspace::GetWorkspace().GetSymbols().GetData().FindSymbolUnit( nature.toStdString() );
    if( !symbol )
        symbol = ADN_Workspace::GetWorkspace().GetSymbols().GetData().FindSymbolUnit( "undefined" );
    setPixmap( pixmapAccessor_( symbol ) );
    emit SymbolChanged( symbolAccessor_( symbol ).c_str() );
}
