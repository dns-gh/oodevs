// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-03-18 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Log_GUI.cpp $
// $Author: Ape $
// $Modtime: 21/04/05 16:51 $
// $Revision: 6 $
// $Workfile: ADN_Log_GUI.cpp $
//
// *****************************************************************************

#include "ADN_pch.h"
#include "ADN_Log_GUI.h"

#include "ADN_Log_Data.h"
#include "ADN_GuiBuilder.h"
#include "ADN_EditLine.h"

#include "ENT/ENT_Tr.h"

#include <qgroupbox.h>
#include <qhgroupbox.h>
#include <qhbox.h>
#include <qgrid.h>
#include <qlabel.h>
#include <qlayout.h>


// -----------------------------------------------------------------------------
// Name: ADN_Log_GUI constructor
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
ADN_Log_GUI::ADN_Log_GUI( ADN_Log_Data& data )
: ADN_GUI_ABC( "ADN_Log_GUI" )
, data_      ( data )
{
}


// -----------------------------------------------------------------------------
// Name: ADN_Log_GUI destructor
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
ADN_Log_GUI::~ADN_Log_GUI()
{
}


// -----------------------------------------------------------------------------
// Name: ADN_Log_GUI::Build
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
void ADN_Log_GUI::Build()
{
    assert( pMainWidget_ == 0 );
    ADN_GuiBuilder builder;

    // Create the main widget.
    pMainWidget_ = new QWidget( 0 );

    QGroupBox* pGroup = new QHGroupBox( tr( "Temps de bordée" ), pMainWidget_ );

    for( ADN_Log_Data::IT_WorkTimeModifiersInfo_Vector it = data_.vWorkTimeModifiers_.begin(); it != data_.vWorkTimeModifiers_.end(); ++it )
        this->BuildWorkTimeModifiers( pGroup, **it );

    // Layout
    QVBoxLayout* pLayout = new QVBoxLayout( pMainWidget_, 10, 10 );
    pLayout->addWidget( pGroup );
}


// -----------------------------------------------------------------------------
// Name: ADN_Log_GUI::BuildWorkTimeModifiers
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
QWidget* ADN_Log_GUI::BuildWorkTimeModifiers( QWidget* pParent, ADN_Log_Data::WorkTimeModifiersInfo& modifiers )
{
    ADN_GuiBuilder builder;
    QGroupBox* pGroupBox = new QGroupBox( 3, Qt::Horizontal, ENT_Tr::ConvertFromTempsBordee( modifiers.nType_, ENT_Tr::eToTr ).c_str(), pParent );

    builder.AddField<ADN_EditLine_Double>( pGroupBox, tr( "Speed factor" ), modifiers.rRepairModifier_, 0, eGreaterZero );
    builder.AddField<ADN_EditLine_Double>( pGroupBox, tr( "Sorting time factor" ), modifiers.rSortModifier_, 0, eGreaterZero );
    builder.AddField<ADN_EditLine_Double>( pGroupBox, tr( "Treating time factor" ), modifiers.rTreatModifier_, 0, eGreaterZero );

    return pGroupBox;
}
