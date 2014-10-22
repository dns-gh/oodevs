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
#include "clients_kernel/Options.h"
#include "clients_kernel/OptionVariant.h"
#include <xeumeuleu/xml.hpp>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: GradientPreferences constructor
// Created: SBO 2007-07-03
// -----------------------------------------------------------------------------
GradientPreferences::GradientPreferences()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GradientPreferences constructor
// Created: ABR 2014-07-28
// -----------------------------------------------------------------------------
GradientPreferences::GradientPreferences( const GradientPreferences& other )
{
    for( auto it = other.gradients_.begin(); it != other.gradients_.end(); ++it )
        Add( std::make_shared< Gradient >( **it ) );
    current_ = GetByName( other.current_->GetName() );
}

// -----------------------------------------------------------------------------
// Name: GradientPreferences destructor
// Created: SBO 2007-07-03
// -----------------------------------------------------------------------------
GradientPreferences::~GradientPreferences()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GradientPreferences::Load
// Created: ABR 2014-08-05
// -----------------------------------------------------------------------------
void GradientPreferences::Load( const kernel::Options& options )
{
    gradients_.clear();
    options.Apply( [&]( const std::string& option, const kernel::OptionVariant& value, bool ) {
        QString name = option.c_str();
        if( name.indexOf( "Elevation/Gradients/" ) != 0 )
            return;
        name.remove( "Elevation/Gradients/" );
        Add( std::make_shared< Gradient >( name, value.To< QString >() ) );
    } );
    current_ = GetByName( options.Get( "Elevation/Gradient" ).To< QString >() );
}

// -----------------------------------------------------------------------------
// Name: GradientPreferences::Add
// Created: ABR 2014-10-09
// -----------------------------------------------------------------------------
void GradientPreferences::Add( const T_Gradient& gradient )
{
    if( !gradient )
        return;
    auto it = std::find( gradients_.begin(), gradients_.end(), gradient );
    if( it != gradients_.end() )
        throw MASA_EXCEPTION( "gradient already registered" );
    gradients_.push_back( gradient );
    if( !current_ )
        current_ = gradient;
}

// -----------------------------------------------------------------------------
// Name: GradientPreferences::Remove
// Created: ABR 2014-10-09
// -----------------------------------------------------------------------------
void GradientPreferences::Remove( const T_Gradient& gradient )
{
    auto it = std::find( gradients_.begin(), gradients_.end(), gradient );
    if( it != gradients_.end() )
        gradients_.erase( it );
}

// -----------------------------------------------------------------------------
// Name: GradientPreferences::Count
// Created: ABR 2014-10-09
// -----------------------------------------------------------------------------
size_t GradientPreferences::Count() const
{
    return gradients_.size();
}

// -----------------------------------------------------------------------------
// Name: GradientPreferences::Apply
// Created: ABR 2014-10-09
// -----------------------------------------------------------------------------
void GradientPreferences::Apply( const std::function< void( const T_Gradient ) >& functor ) const
{
    std::for_each( gradients_.begin(), gradients_.end(), functor );
}

// -----------------------------------------------------------------------------
// Name: GradientPreferences::GetByName
// Created: ABR 2014-10-09
// -----------------------------------------------------------------------------
GradientPreferences::T_Gradient GradientPreferences::GetByName( const QString& name ) const
{
    auto it = std::find_if( gradients_.begin(), gradients_.end(), [&name]( const T_Gradient& gradient ){
        return gradient->GetName() == name;
    } );
    if( it == gradients_.end() )
        return gradients_.empty() ? T_Gradient() : gradients_.at( 0 );
    return *it;
}

// -----------------------------------------------------------------------------
// Name: GradientPreferences::GetByDisplayName
// Created: ABR 2014-10-09
// -----------------------------------------------------------------------------
GradientPreferences::T_Gradient GradientPreferences::GetByDisplayName( const QString& name ) const
{
    auto it = std::find_if( gradients_.begin(), gradients_.end(), [&name]( const T_Gradient& gradient ){
        return gradient->GetDisplayName() == name;
    } );
    if( it == gradients_.end() )
        return gradients_.empty() ? T_Gradient() : gradients_.at( 0 );
    return *it;
}

// -----------------------------------------------------------------------------
// Name: GradientPreferences::GetCurrent
// Created: ABR 2014-10-09
// -----------------------------------------------------------------------------
GradientPreferences::T_Gradient GradientPreferences::GetCurrent() const
{
    return current_;
}

// -----------------------------------------------------------------------------
// Name: GradientPreferences::SetCurrent
// Created: ABR 2014-10-09
// -----------------------------------------------------------------------------
void GradientPreferences::SetCurrent( const T_Gradient& current )
{
    current_ = current;
}
