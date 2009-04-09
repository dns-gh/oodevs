// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "IndicatorFunction.h"
#include "IndicatorPrimitive.h"
#include "IndicatorPrimitiveParameter.h"
#include "IndicatorType.h"
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <xeumeuleu/xml.h>

// -----------------------------------------------------------------------------
// Name: IndicatorFunction constructor
// Created: SBO 2009-03-17
// -----------------------------------------------------------------------------
IndicatorFunction::IndicatorFunction( unsigned long id, const IndicatorPrimitive& primitive )
    : IndicatorElement_ABC( id )
    , primitive_( primitive )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IndicatorFunction destructor
// Created: SBO 2009-03-17
// -----------------------------------------------------------------------------
IndicatorFunction::~IndicatorFunction()
{
    // NOTHING
}

namespace
{
    class ParameterTypeChecker : public IndicatorPrimitiveParameterVisitor_ABC
    {
    public:
        ParameterTypeChecker( const IndicatorElement_ABC& element, unsigned int count )
            : element_( element )
            , count_( count )
            , current_( 0 )
            , parameter_( 0 )
        {}
        virtual ~ParameterTypeChecker() {}

        virtual void Visit( const IndicatorPrimitiveParameter& parameter )
        {
            if( current_++ != count_ )
                return;
            if( element_.GetType() != parameter.GetType() )
                throw std::exception( "parameter type mismatch" );
            parameter_ = &parameter;
        }

    private:
        ParameterTypeChecker& operator=( const ParameterTypeChecker& );

    public:
        const IndicatorElement_ABC& element_;
        const unsigned int count_;
        unsigned int current_;
        const IndicatorPrimitiveParameter* parameter_;
    };
}

// -----------------------------------------------------------------------------
// Name: IndicatorFunction::AddParameter
// Created: SBO 2009-03-17
// -----------------------------------------------------------------------------
void IndicatorFunction::AddParameter( boost::shared_ptr< IndicatorElement_ABC > element )
{
    ParameterTypeChecker checker( *element, parameters_.size() );
    primitive_.Accept( checker );
    parameters_[ checker.parameter_ ] = element;
}

// -----------------------------------------------------------------------------
// Name: IndicatorFunction::GetType
// Created: SBO 2009-04-09
// -----------------------------------------------------------------------------
const IndicatorType& IndicatorFunction::GetType() const
{
    return primitive_.GetType();
}

// -----------------------------------------------------------------------------
// Name: IndicatorFunction::Serialize
// Created: SBO 2009-03-17
// -----------------------------------------------------------------------------
void IndicatorFunction::Serialize( xml::xostream& xos ) const
{
    BOOST_FOREACH( const T_Parameters::value_type& parameter, parameters_ )
        parameter.second->SerializeDeclaration( xos );
    xos << xml::start( primitive_.GetCategory() )
            << xml::attribute( "id", GetId() )
            << xml::attribute( "function", primitive_.GetName() )
            << xml::attribute( "type", primitive_.GetType().ToString() );
    std::map< std::string, std::vector< std::string > > values;
    BOOST_FOREACH( const T_Parameters::value_type& parameter, parameters_ )
        values[ parameter.first->GetAttribute() ].push_back( boost::lexical_cast< std::string, unsigned long >( parameter.second->GetId() ) );
    for( std::map< std::string, std::vector< std::string > >::const_iterator it = values.begin(); it != values.end(); ++it )
    {
        const std::string value = boost::algorithm::join( it->second, "," );
        if( !value.empty() )
            xos << xml::attribute( it->first, value );
    }
    xos << xml::end();
}

// -----------------------------------------------------------------------------
// Name: IndicatorFunction::SerializeDeclaration
// Created: SBO 2009-04-03
// -----------------------------------------------------------------------------
void IndicatorFunction::SerializeDeclaration( xml::xostream& xos ) const
{
    // $$$$ SBO 2009-04-03: NOTHING
}
