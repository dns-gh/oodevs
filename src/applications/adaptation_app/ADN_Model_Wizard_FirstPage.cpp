// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Model_Wizard_FirstPage.h"

// -----------------------------------------------------------------------------
// Name: ADN_Model_Wizard_FirstPage constructor
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Model_Wizard_FirstPage::ADN_Model_Wizard_FirstPage( ADN_Missions_Data::T_Mission_Vector& missions, T_ItemVector& existingItemsList, Q3Wizard* pParent, const char* szName /* = 0*/, const char* szImageName /* = "adn_wizard.bmp"*/ )
    : ADN_Wizard_FirstPage_Default< ADN_Models_Data::ModelInfos >( existingItemsList, pParent, szName, szImageName )
    , missions_( missions )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Model_Wizard_FirstPage destructor
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Model_Wizard_FirstPage::~ADN_Model_Wizard_FirstPage()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Model_Wizard_FirstPage::NewT
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Models_Data::ModelInfos* ADN_Model_Wizard_FirstPage::NewT()
{
    return new ADN_Models_Data::ModelInfos( missions_ );
}
