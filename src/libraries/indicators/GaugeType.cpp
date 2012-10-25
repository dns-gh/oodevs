// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "indicators_pch.h"
#include "GaugeType.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/DisplayExtractor_ABC.h"
#include <xeumeuleu/xml.hpp>

using namespace indicators;

// -----------------------------------------------------------------------------
// Name: GaugeType constructor
// Created: SBO 2009-05-05
// -----------------------------------------------------------------------------
GaugeType::GaugeType( xml::xistream& xis )
    : name_( xis.attribute< std::string >( "name" ).c_str() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GaugeType destructor
// Created: SBO 2009-05-05
// -----------------------------------------------------------------------------
GaugeType::~GaugeType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GaugeType::GetName
// Created: SBO 2009-05-05
// -----------------------------------------------------------------------------
const QString& GaugeType::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: GaugeType::Display
// Created: SBO 2009-05-05
// -----------------------------------------------------------------------------
void GaugeType::Display( kernel::Displayer_ABC& displayer, double value ) const
{
    displayer.Display( value );
}

// -----------------------------------------------------------------------------
// Name: GaugeType::Display
// Created: JSR 2012-10-25
// -----------------------------------------------------------------------------
void GaugeType::Display( QTreeWidgetItem* item, kernel::DisplayExtractor_ABC& extractor, int col, double value ) const
{
    item->setText( col, extractor.GetDisplayName( value ) );
}
