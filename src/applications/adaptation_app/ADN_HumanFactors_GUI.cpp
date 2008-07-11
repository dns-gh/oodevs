// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-03-18 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_HumanFactors_GUI.cpp $
// $Author: Ape $
// $Modtime: 13/04/05 17:49 $
// $Revision: 6 $
// $Workfile: ADN_HumanFactors_GUI.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_HumanFactors_GUI.h"

#include "ADN_HumanFactors_Data.h"
#include "ADN_GuiBuilder.h"
#include "ADN_EditLine.h"

#include <qgroupbox.h>
#include <qvbox.h>
#include <qlabel.h>
#include <qlayout.h>


// -----------------------------------------------------------------------------
// Name: ADN_HumanFactors_GUI constructor
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
ADN_HumanFactors_GUI::ADN_HumanFactors_GUI( ADN_HumanFactors_Data& data )
: ADN_GUI_ABC( "ADN_HumanFactors_GUI" )
, data_      ( data )
{
}


// -----------------------------------------------------------------------------
// Name: ADN_HumanFactors_GUI destructor
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
ADN_HumanFactors_GUI::~ADN_HumanFactors_GUI()
{
}


// -----------------------------------------------------------------------------
// Name: ADN_HumanFactors_GUI::Build
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
void ADN_HumanFactors_GUI::Build()
{
    assert( pMainWidget_ == 0 );

    ADN_GuiBuilder builder;

    // Create the main widget.
    pMainWidget_ = new QWidget( 0 );

    QGroupBox* pGroupBox1 = new QGroupBox( 3, Qt::Horizontal, tr( "Experience modifiers" ), pMainWidget_ );
    this->BuildModifiers( pGroupBox1, data_.newbieModifiers_, tr( "Newbie" ) );
    this->BuildModifiers( pGroupBox1, data_.xpModifiers_, tr( "Experienced" ) );
    this->BuildModifiers( pGroupBox1, data_.veteranModifiers_, tr( "Veteran" ) );

    QGroupBox* pGroupBox2 = new QGroupBox( 3, Qt::Horizontal, tr( "Tiredness modifiers" ), pMainWidget_ );
    this->BuildModifiers( pGroupBox2, data_.normalModifiers_, tr( "Not tired" ) );
    this->BuildModifiers( pGroupBox2, data_.tiredModifiers_, tr( "Tired" ) );
    this->BuildModifiers( pGroupBox2, data_.exhaustedModifiers_, tr( "Exhausted" ) );

    // Layout
    QVBoxLayout* pLayout = new QVBoxLayout( pMainWidget_, 10, 10 );
    pLayout->setAlignment( Qt::AlignTop );
    pLayout->addWidget( pGroupBox1 );
    pLayout->addWidget( pGroupBox2 );
}


// -----------------------------------------------------------------------------
// Name: ADN_HumanFactors_GUI::BuildModifiers
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
QWidget* ADN_HumanFactors_GUI::BuildModifiers( QWidget* pParent, ADN_HumanFactors_Data::ModifiersInfo& modifiers, const char* szName )
{
    ADN_GuiBuilder builder;

    QGroupBox* pGroupBox = new QGroupBox( 3, Qt::Horizontal, szName, pParent );
    builder.AddField<ADN_EditLine_Double>( pGroupBox, tr( "Effect on movement speed" ), modifiers.rSpeedModifier_, 0, eGreaterZero );
    builder.AddField<ADN_EditLine_Double>( pGroupBox, tr( "Effect on reloading duration" ), modifiers.rReloadModifier_, 0, eGreaterZero );
    builder.AddField<ADN_EditLine_Double>( pGroupBox, tr( "Effect on stance changes duration" ), modifiers.rStanceModifier_, 0, eGreaterZero );
    builder.AddField<ADN_EditLine_Double>( pGroupBox, tr( "Effect on PH" ), modifiers.rPHModifier_, 0, eGreaterZero );
    builder.AddField<ADN_EditLine_Double>( pGroupBox, tr( "Effect on detection ranges" ), modifiers.rSensorsModifier_, 0, eGreaterZero );

    return pGroupBox;
}


