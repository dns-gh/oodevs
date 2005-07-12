// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-04-27 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Breakdown_Wizard_Page0.cpp $
// $Author: Nld $
// $Modtime: 27/04/05 10:53 $
// $Revision: 1 $
// $Workfile: ADN_Breakdown_Wizard_Page0.cpp $
//
// *****************************************************************************

#include "ADN_pch.h"
#include "ADN_Breakdown_Wizard_Page0.h"

// -----------------------------------------------------------------------------
// Name: ADN_Breakdown_Wizard_Page0::ADN_Breakdown_Wizard_Page0
// Created: APE 2005-04-27
// -----------------------------------------------------------------------------
ADN_Breakdown_Wizard_Page0::ADN_Breakdown_Wizard_Page0( ADN_Breakdowns_Data::NTIInfo& nti, QWizard* pParent /*= 0*/, const char* szName /*= 0*/ )
: ADN_Wizard_FirstPage_Default<ADN_Breakdowns_Data::BreakdownInfo>( nti.vBreakdowns_, pParent, szName )
, nti_      ( nti )
{
}


// -----------------------------------------------------------------------------
// Name: ADN_Breakdown_Wizard_Page0::~ADN_Breakdown_Wizard_Page0
// Created: APE 2005-04-27
// -----------------------------------------------------------------------------
ADN_Breakdown_Wizard_Page0::~ADN_Breakdown_Wizard_Page0()
{
}


// -----------------------------------------------------------------------------
// Name: ADN_Breakdown_Wizard_Page0::NewT
// Created: APE 2005-04-27
// -----------------------------------------------------------------------------
ADN_Breakdowns_Data::BreakdownInfo* ADN_Breakdown_Wizard_Page0::NewT()
{
    return new ADN_Breakdowns_Data::BreakdownInfo( nti_ );
}
