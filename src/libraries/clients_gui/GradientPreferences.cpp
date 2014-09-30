// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "GradientPreferences.h"
#include "Gradient.h"
#include "clients_kernel/OptionsController.h"
#include "clients_kernel/OptionVariant.h"
#include <xeumeuleu/xml.hpp>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: GradientPreferences constructor
// Created: SBO 2007-07-03
// -----------------------------------------------------------------------------
GradientPreferences::GradientPreferences( kernel::OptionsController& options )
    : options_( options )
{
    Reset();
}

// -----------------------------------------------------------------------------
// Name: GradientPreferences destructor
// Created: SBO 2007-07-03
// -----------------------------------------------------------------------------
GradientPreferences::~GradientPreferences()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: GradientPreferences::Commit
// Created: SBO 2007-07-03
// -----------------------------------------------------------------------------
void GradientPreferences::Commit( const std::vector< Gradient* >& presets )
{
    DeleteAll();
    for( auto it = presets.begin(); it != presets.end(); ++it )
        Register( (*it)->GetName(), *new Gradient( **it ) );
    Save();
}

// -----------------------------------------------------------------------------
// Name: GradientPreferences::Reset
// Created: SBO 2007-07-03
// -----------------------------------------------------------------------------
void GradientPreferences::Reset()
{
    tools::Xifstream xis( "gradients.xml" );
    Load( xis );
}

// -----------------------------------------------------------------------------
// Name: GradientPreferences::Load
// Created: SBO 2007-07-03
// -----------------------------------------------------------------------------
void GradientPreferences::Load( xml::xistream& xis )
{
    DeleteAll();
    xis >> xml::start( "gradients" )
            >> xml::list( "gradient", *this, &GradientPreferences::ReadGradient )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: GradientPreferences::Save
// Created: SBO 2007-07-03
// -----------------------------------------------------------------------------
void GradientPreferences::Save() const
{
    for( auto it = elements_.begin(); it != elements_.end(); ++it )
        it->second->Save( options_, "Elevation/Gradients/" );
}

// -----------------------------------------------------------------------------
// Name: GradientPreferences::ReadGradient
// Created: SBO 2007-07-03
// -----------------------------------------------------------------------------
void GradientPreferences::ReadGradient( xml::xistream& xis )
{
    Gradient* gradient = new Gradient( xis );
    Register( gradient->GetName(), *gradient );
}

// -----------------------------------------------------------------------------
// Name: GradientPreferences::SetGradient
// Created: SBO 2008-08-18
// -----------------------------------------------------------------------------
void GradientPreferences::SetGradient( const QString& name, const QString& values )
{
    Gradient* gradient = Find( name );
    if( !gradient )
    {
        gradient = new Gradient( name, values );
        Register( gradient->GetName(), *gradient );
    }
    else
        gradient->LoadValues( values );
}
