// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "AutomatComposition.h"
#include "Resolver_ABC.h"
#include "xeumeuleu/xml.h"
#include <qstringlist.h>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AutomatComposition constructor
// Created: AGE 2006-09-05
// -----------------------------------------------------------------------------
AutomatComposition::AutomatComposition( xml::xistream& input, const Resolver_ABC< AgentType, QString >& agentResolver )
    : type_( agentResolver.Get( ReadType( input ) ) ) 
    , min_( 0 )
    , max_( 0 )
{
    std::string quantity;
    input >> quantity;
    QString q = quantity.c_str();

    if( q == "+" || q == "*" )
    {
        min_ = ( q == "+" ) ? 1 : 0;
        max_ = std::numeric_limits< unsigned int >::max();
    }
    else
    {
        QStringList minMax = QStringList::split( "..", q );
        if( ! minMax.isEmpty() )
            max_ = min_ = minMax[0].toUInt();
        if( minMax.size() >= 2 )
            max_ = minMax[1].toUInt();
    }
}

// -----------------------------------------------------------------------------
// Name: AutomatComposition destructor
// Created: AGE 2006-09-05
// -----------------------------------------------------------------------------
AutomatComposition::~AutomatComposition()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatComposition::ReadType
// Created: AGE 2006-09-05
// -----------------------------------------------------------------------------
QString AutomatComposition::ReadType( xml::xistream& input )
{
    std::string name;
    input >> xml::attribute( "nom", name );
    return name.c_str();
}

// -----------------------------------------------------------------------------
// Name: AutomatComposition::GetType
// Created: AGE 2006-09-05
// -----------------------------------------------------------------------------
const AgentType& AutomatComposition::GetType() const
{
    return type_;
}
    
// -----------------------------------------------------------------------------
// Name: AutomatComposition::GetMin
// Created: AGE 2006-09-05
// -----------------------------------------------------------------------------
unsigned int AutomatComposition::GetMin() const
{
    return min_;
}
    
// -----------------------------------------------------------------------------
// Name: AutomatComposition::GetMax
// Created: AGE 2006-09-05
// -----------------------------------------------------------------------------
unsigned int AutomatComposition::GetMax() const
{
    return max_;
}

// -----------------------------------------------------------------------------
// Name: AutomatComposition::GetSensibleNumber
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
unsigned int AutomatComposition::GetSensibleNumber() const
{
    if( max_ == min_ )
        return min_;
    if( max_ != std::numeric_limits< unsigned int >::max() )
        return std::max( ( max_ - min_ ) / 2u, 1u );
    return std::max( min_, 1u );
}
