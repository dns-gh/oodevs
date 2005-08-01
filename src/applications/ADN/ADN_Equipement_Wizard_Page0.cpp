// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-01-20 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Equipement_Wizard_Page0.cpp $
// $Author: Ape $
// $Modtime: 18/02/05 15:13 $
// $Revision: 2 $
// $Workfile: ADN_Equipement_Wizard_Page0.cpp $
//
// *****************************************************************************

#include "ADN_pch.h"
#include "ADN_Equipement_Wizard_Page0.h"


// -----------------------------------------------------------------------------
// Name: ADN_Equipement_Wizard_Page0 constructor
// Created: APE 2005-01-20
// -----------------------------------------------------------------------------
ADN_Equipement_Wizard_Page0::ADN_Equipement_Wizard_Page0( ADN_Equipement_Data::DotationInfos& parentDotation, QWizard* pParent, const char* szName )
: ADN_Wizard_FirstPage_Default<ADN_Equipement_Data::CategoryInfo>( parentDotation.categories_, pParent, szName )
, parentDotation_   ( parentDotation )
{
}


// -----------------------------------------------------------------------------
// Name: ADN_Equipement_Wizard_Page0 destructor
// Created: APE 2005-01-20
// -----------------------------------------------------------------------------
ADN_Equipement_Wizard_Page0::~ADN_Equipement_Wizard_Page0()
{
}


// -----------------------------------------------------------------------------
// Name: ADN_Equipement_Wizard_Page0::NewT
// Created: APE 2005-02-15
// -----------------------------------------------------------------------------
ADN_Equipement_Data::CategoryInfo* ADN_Equipement_Wizard_Page0::NewT()
{
    if( parentDotation_.nType_ == eFamilleDotation_Munition )
        return new ADN_Equipement_Data::AmmoCategoryInfo( parentDotation_ );
    else
        return new ADN_Equipement_Data::CategoryInfo( parentDotation_ );
}
