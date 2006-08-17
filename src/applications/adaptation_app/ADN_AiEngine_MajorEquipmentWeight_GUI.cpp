// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2004-06-15 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_AiEngine_MajorEquipmentWeight_GUI.cpp $
// $Author: Ape $
// $Modtime: 19/01/05 18:29 $
// $Revision: 2 $
// $Workfile: ADN_AiEngine_MajorEquipmentWeight_GUI.cpp $
//
// *****************************************************************************


#include "adaptation_app_pch.h"
#include "ADN_AiEngine_MajorEquipmentWeight_GUI.h"


// -----------------------------------------------------------------------------
// Name: ADN_AiEngine_MajorEquipmentWeight_GUI constructor
/** @param  pParent 
    @param  pMinorEditLine 
*/
// Created: AGN 2004-06-15
// -----------------------------------------------------------------------------
ADN_AiEngine_MajorEquipmentWeight_GUI::ADN_AiEngine_MajorEquipmentWeight_GUI( QWidget* pParent, QLineEdit* pMinorEditLine )
: ADN_EditLine_Double( pParent )
, pMinorEditLine_( pMinorEditLine )
{
    setReadOnly( true );

    connect( pMinorEditLine_, SIGNAL( textChanged( const QString & ) ), this, SIGNAL( textChanged( const QString& ) ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_AiEngine_MajorEquipmentWeight_GUI destructor
// Created: AGN 2004-06-15
// -----------------------------------------------------------------------------
ADN_AiEngine_MajorEquipmentWeight_GUI::~ADN_AiEngine_MajorEquipmentWeight_GUI()
{
    
}

// -----------------------------------------------------------------------------
// Name: ADN_AiEngine_MajorEquipmentWeight_GUI::TextChanged
// Created: AGN 2004-06-15
// -----------------------------------------------------------------------------
void ADN_AiEngine_MajorEquipmentWeight_GUI::TextChanged(const QString& )
{
    ADN_EditLine_Double::TextChanged( QString::number( 1.0 - pMinorEditLine_->text().toDouble() ) );
}
