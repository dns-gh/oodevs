// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "SuccessFactorCondition.h"
#include "Score_ABC.h"
#include "ScoresModel.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: SuccessFactorCondition constructor
// Created: SBO 2009-06-16
// -----------------------------------------------------------------------------
SuccessFactorCondition::SuccessFactorCondition( xml::xistream& xis, const ScoresModel& model )
    : score_( model.Get( xml::attribute< std::string >( xis, "property" ).c_str() ) )
    , operator_( xml::attribute< std::string >( xis, "operator", "or" ) )
    , value_( xml::attribute< float >( xis, "value" ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorCondition constructor
// Created: SBO 2009-06-17
// -----------------------------------------------------------------------------
SuccessFactorCondition::SuccessFactorCondition( const Score_ABC& score, const std::string& op, float value )
    : score_( score )
    , operator_( op )
    , value_( value )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorCondition destructor
// Created: SBO 2009-06-16
// -----------------------------------------------------------------------------
SuccessFactorCondition::~SuccessFactorCondition()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorCondition::Serialize
// Created: SBO 2009-06-16
// -----------------------------------------------------------------------------
void SuccessFactorCondition::Serialize( xml::xostream& xos )
{
    xos << xml::start( "condition" )
            << xml::attribute( "property", score_.GetName() )
            << xml::attribute( "operator", operator_ )
            << xml::attribute( "value", value_ )
        << xml::end();
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorCondition::GetScore
// Created: SBO 2009-06-16
// -----------------------------------------------------------------------------
const Score_ABC& SuccessFactorCondition::GetScore() const
{
    return score_;
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorCondition::GetOperator
// Created: SBO 2009-06-16
// -----------------------------------------------------------------------------
std::string SuccessFactorCondition::GetOperator() const
{
    return operator_;
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorCondition::GetValue
// Created: SBO 2009-06-16
// -----------------------------------------------------------------------------
float SuccessFactorCondition::GetValue() const
{
    return value_;
}
