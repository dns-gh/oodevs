// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "LanguageController.h"
#include "tools/Language.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: LanguageController constructor
// Created: ABR 2013-10-15
// -----------------------------------------------------------------------------
LanguageController::LanguageController()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LanguageController destructor
// Created: ABR 2013-10-15
// -----------------------------------------------------------------------------
LanguageController::~LanguageController()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LanguageController::Register
// Created: ABR 2013-10-15
// -----------------------------------------------------------------------------
void LanguageController::Register( tools::Observer_ABC& observer )
{
    tools::SortedInterfaceContainer< tools::Observer_ABC >::Register( observer );
}

// -----------------------------------------------------------------------------
// Name: LanguageController::Unregister
// Created: ABR 2013-10-15
// -----------------------------------------------------------------------------
void LanguageController::Unregister( tools::Observer_ABC& observer )
{
    tools::SortedInterfaceContainer< tools::Observer_ABC >::Unregister( observer );
}

// -----------------------------------------------------------------------------
// Name: LanguageController::ChangeLanguage
// Created: ABR 2013-10-15
// -----------------------------------------------------------------------------
void LanguageController::ChangeLanguage( const std::string& language )
{
    tools::Language::SetCurrent( language );
    Apply( &tools::LanguageObserver_ABC::OnLanguageChanged );
};
