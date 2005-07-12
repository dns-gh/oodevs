// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-03-22 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Health_GUI.cpp $
// $Author: Ape $
// $Modtime: 21/04/05 16:50 $
// $Revision: 5 $
// $Workfile: ADN_Health_GUI.cpp $
//
// *****************************************************************************

#include "ADN_pch.h"
#include "ADN_Health_GUI.h"

#include "ADN_Health_Data.h"
#include "ADN_GuiBuilder.h"
#include "ADN_EditLine.h"
#include "ADN_Tr.h"

#include <qlayout.h>
#include <qlabel.h>
#include <qgroupbox.h>
#include <qhbox.h>
#include <qvbox.h>


// -----------------------------------------------------------------------------
// Name: ADN_Health_GUI constructor
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
ADN_Health_GUI::ADN_Health_GUI( ADN_Health_Data& data )
: ADN_GUI_ABC( "ADN_Health_GUI" )
, data_      ( data )
{
}


// -----------------------------------------------------------------------------
// Name: ADN_Health_GUI destructor
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
ADN_Health_GUI::~ADN_Health_GUI()
{
}


// -----------------------------------------------------------------------------
// Name: ADN_Health_GUI::Build
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
void ADN_Health_GUI::Build()
{
    if( pMainWidget_ != 0 )
        return;

    ADN_GuiBuilder builder;

    // Create the top widget.
    pMainWidget_ = new QWidget( 0 );
    QGroupBox* pGroup = new QGroupBox( 1, Qt::Horizontal, tr( "Health" ), pMainWidget_ );

    QWidget* pHolder = builder.AddFieldHolder( pGroup );

    builder.AddField<ADN_EditLine_Double>( pHolder, tr( "Diagnostic time" ), data_.rDiagnosticTime_, tr( "s" ), eGreaterEqualZero );
    builder.AddField<ADN_EditLine_Double>( pHolder, tr( "Sorting time" ), data_.rSortingTime_, tr( "s" ), eGreaterEqualZero );

    QHBox* pWoundsGroup = new QHBox( pGroup );
    pWoundsGroup->setSpacing( 5 );
    
    for( int n = 0; n < eNbrDoctorSkills; ++n )
        this->BuildWound( pWoundsGroup, (E_DoctorSkills)n );
    
    QGroupBox* pShockGroup = new QGroupBox( 3, Qt::Horizontal, tr( "Shock" ), pWoundsGroup );
    builder.AddField<ADN_EditLine_Double>( pShockGroup, tr( "Treating time" ), data_.rShockTreatTime_, tr( "s" ), eGreaterZero );
    builder.AddField<ADN_EditLine_Double>( pShockGroup, tr( "Percentage" ), data_.rShockPercentage_, tr( "%" ), ePercentage );

    QGroupBox* pContaminationGroup = new QGroupBox( 3, Qt::Horizontal, tr( "Contamination" ), pWoundsGroup );
    builder.AddField<ADN_EditLine_Double>( pContaminationGroup, tr( "Treating time" ), data_.rContaminationTreatTime_, tr( "s" ), eGreaterZero );

    // Layout
    QVBoxLayout* pLayout = new QVBoxLayout( pMainWidget_, 10, 5 );
    pLayout->addWidget( pGroup );
    builder.AddStretcher( pLayout, Qt::Vertical );
}


// -----------------------------------------------------------------------------
// Name: ADN_Health_GUI::BuildWound
// Created: APE 2005-03-22
// -----------------------------------------------------------------------------
QWidget* ADN_Health_GUI::BuildWound( QWidget* pParent, E_DoctorSkills nWound )
{
    ADN_GuiBuilder builder;
    ADN_Health_Data::WoundInfo& wound = data_.wounds[nWound];
    QGroupBox* pGroup = new QGroupBox( 3, Qt::Horizontal, ADN_Tr::ConvertFromDoctorSkills( wound.nType_ ).c_str(), pParent );
    builder.AddField<ADN_EditLine_Double>( pGroup, tr( "Treating time" ), wound.rTreatTime_, tr( "s" ), eGreaterZero );
    builder.AddField<ADN_EditLine_Double>( pGroup, tr( "Percentage" ), wound.rPercentage_, tr( "%" ), ePercentage );
    builder.AddField<ADN_EditLine_Double>( pGroup, tr( "Life expectency" ), wound.rLifeExpectancy_, tr( "s" ), eGreaterZero );

    return pGroup;
}
