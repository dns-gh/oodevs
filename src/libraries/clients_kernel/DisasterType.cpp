// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "DisasterType.h"
#include <boost/algorithm/string/replace.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: DisasterType constructor
// Created: LGY 2012-11-19
// -----------------------------------------------------------------------------
DisasterType::DisasterType( xml::xistream& input )
{
    input >> xml::attribute( "name", name_ )
            >> xml::start( "contamination" )
                >> xml::list( "threshold", *this, &DisasterType::ReadContaminationThresholds )
            >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: DisasterType destructor
// Created: LGY 2012-11-19
// -----------------------------------------------------------------------------
DisasterType::~DisasterType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DisasterType::ReadContaminationThresholds
// Created: LGY 2012-11-19
// -----------------------------------------------------------------------------
void DisasterType::ReadContaminationThresholds( xml::xistream& xis )
{
    std::string color = xis.attribute< std::string >( "color" );
    boost::replace_all( color, "0x", "#" );
    color_[ xis.attribute< double > ( "value" ) ] = QColor( color.c_str() );
}

// -----------------------------------------------------------------------------
// Name: DisasterType::GetName
// Created: LGY 2012-11-19
// -----------------------------------------------------------------------------
const std::string& DisasterType::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: DisasterType::GetColor
// Created: LGY 2012-11-21
// -----------------------------------------------------------------------------
QColor DisasterType::GetColor( double value ) const
{
    QColor color( Qt::blue );
    if( color_.empty() )
        return Qt::blue;
    for( auto it = color_.begin(); it != color_.end(); ++it )
        if( value >= it->first )
            color = it->second;
    return color;
}
