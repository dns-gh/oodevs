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
#include "xeumeuleu/xml.h"

using namespace xml;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: GradientPreferences constructor
// Created: SBO 2007-07-03
// -----------------------------------------------------------------------------
GradientPreferences::GradientPreferences()
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
    for( std::vector< Gradient* >::const_iterator it = presets.begin(); it != presets.end(); ++it )
        Register( (*it)->GetName(), *new Gradient( **it ) );

    xml::xofstream xos( "gradients.xml" ); // $$$$ SBO 2007-07-03: 
    Save( xos );
}

// -----------------------------------------------------------------------------
// Name: GradientPreferences::Reset
// Created: SBO 2007-07-03
// -----------------------------------------------------------------------------
void GradientPreferences::Reset()
{
    xml::xifstream xis( "gradients.xml" ); // $$$$ SBO 2007-07-03: 
    Load( xis );
}

// -----------------------------------------------------------------------------
// Name: GradientPreferences::Load
// Created: SBO 2007-07-03
// -----------------------------------------------------------------------------
void GradientPreferences::Load( xml::xistream& xis )
{
    DeleteAll();
    xis >> start( "gradients" )
            >> list( "gradient", *this, &GradientPreferences::ReadGradient )
        >> end();
}

// -----------------------------------------------------------------------------
// Name: GradientPreferences::Save
// Created: SBO 2007-07-03
// -----------------------------------------------------------------------------
void GradientPreferences::Save( xml::xostream& xos ) const
{
    xos << start( "gradients" );
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        it->second->Save( xos );
    xos << end();            
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
