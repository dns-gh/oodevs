// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "Filter.h"
#include "moc_Filter_ABC.cpp"
#include "clients_kernel/Tools.h"

namespace
{
    std::string ReadLang()
    {
        QSettings settings;
        settings.setPath( "MASA Group", tools::translate( "Application", "SWORD" ) );
        return settings.readEntry( "/Common/Language", QTextCodec::locale() ).ascii();
    }
}

// -----------------------------------------------------------------------------
// Name: Filter constructor
// Created: ABR 2011-06-17
// -----------------------------------------------------------------------------
Filter::Filter()
    : description_( ReadLang() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Filter constructor
// Created: ABR 2011-09-29
// -----------------------------------------------------------------------------
Filter::Filter( xml::xistream& xis )
    : description_( xis, ReadLang() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Filter destructor
// Created: ABR 2011-06-17
// -----------------------------------------------------------------------------
Filter::~Filter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Filter::GetName
// Created: ABR 2011-06-17
// -----------------------------------------------------------------------------
const std::string Filter::GetName() const
{
    return description_.GetName();
}

// -----------------------------------------------------------------------------
// Name: Filter::GetDescription
// Created: ABR 2011-06-17
// -----------------------------------------------------------------------------
const std::string Filter::GetDescription() const
{
    return description_.GetDescription();
}

// -----------------------------------------------------------------------------
// Name: Filter::NeedToReloadExercise
// Created: ABR 2011-06-24
// -----------------------------------------------------------------------------
bool Filter::NeedToReloadExercise() const
{
    return false;
}
