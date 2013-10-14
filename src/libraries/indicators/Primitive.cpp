// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "indicators_pch.h"
#include "Primitive.h"
#include "DataTypeFactory.h"
#include "ElementTypeResolver.h"
#include "Function.h"
#include "PrimitiveParameter.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/Tools.h"
#include "tools/Language.h"
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <xeumeuleu/xml.hpp>

using namespace indicators;

// -----------------------------------------------------------------------------
// Name: Primitive constructor
// Created: SBO 2009-04-06
// -----------------------------------------------------------------------------
Primitive::Primitive( xml::xistream& xis, const DataTypeFactory& types )
    : name_( xis.attribute< std::string >( "name" ).c_str() )
    , category_( xis.attribute< std::string >( "category" ) )
    , comment_( "" )
    , type_( xis.attribute< std::string >( "type" ) )
    , types_( types )
{
    xis >> xml::start( "comments" )
            >> xml::list( "comment", *this, &Primitive::ReadComment )
        >> xml::end;
    xis >> xml::optional >> xml::start( "parameters" )
            >> xml::list( "parameter", *this, &Primitive::ReadParameter )
        >> xml::end;
    prototype_ = QString( "%1(%2)" ).arg( name_ ).arg( BuildParameterList() );
}

// -----------------------------------------------------------------------------
// Name: Primitive destructor
// Created: SBO 2009-04-06
// -----------------------------------------------------------------------------
Primitive::~Primitive()
{
    for( T_Parameters::iterator it = parameters_.begin(); it != parameters_.end(); ++it )
        delete *it;
}

// -----------------------------------------------------------------------------
// Name: Primitive::ReadParameter
// Created: SBO 2009-04-06
// -----------------------------------------------------------------------------
void Primitive::ReadParameter( xml::xistream& xis )
{
    parameters_.push_back( new PrimitiveParameter( xis, types_ ) );
}

// -----------------------------------------------------------------------------
// Name: Primitive::GetName
// Created: SBO 2009-04-09
// -----------------------------------------------------------------------------
QString Primitive::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: Primitive::GetCategory
// Created: SBO 2009-04-09
// -----------------------------------------------------------------------------
std::string Primitive::GetCategory() const
{
    return category_;
}

// -----------------------------------------------------------------------------
// Name: Primitive::GetPrototype
// Created: SBO 2009-05-11
// -----------------------------------------------------------------------------
QString Primitive::GetPrototype() const
{
    return prototype_;
}

// -----------------------------------------------------------------------------
// Name: Primitive::BuildParameterList
// Created: SBO 2009-04-20
// -----------------------------------------------------------------------------
QString Primitive::BuildParameterList() const
{
    std::vector< std::string > list;
    BOOST_FOREACH( const T_Parameters::value_type& parameter, parameters_ )
        list.push_back( parameter->GetName().toStdString() ); // $$$$ ABR 2012-08-14: Do NOT use .toStdString() here or it will crash
    return list.empty() ? "" : ( " " + boost::join( list, ", " ) + " " ).c_str();
}

// -----------------------------------------------------------------------------
// Name: Primitive::ReadComment
// Created: NPT 2012-11-28
// -----------------------------------------------------------------------------
void Primitive::ReadComment( xml::xistream& xis )
{
    if( !xis.has_content() )
        return;
    std::string lang = xis.attribute< std::string >( "lang", "" );
    if( lang.empty() || 
            ( lang == tools::Language::Current() || 
            ( comment_.empty() && lang == "en" ) ) )
        comment_ = xis.value< std::string >();
}


// -----------------------------------------------------------------------------
// Name: Primitive::DisplayInTooltip
// Created: SBO 2009-05-11
// -----------------------------------------------------------------------------
void Primitive::DisplayInTooltip( kernel::Displayer_ABC& displayer ) const
{
    displayer.Display( QString( "<b>%1</b><br><i>%2</i><br>%3" ).arg( name_ ).arg( prototype_ ).arg( comment_.c_str() ) );
}

// -----------------------------------------------------------------------------
// Name: Primitive::Instanciate
// Created: SBO 2009-05-11
// -----------------------------------------------------------------------------
boost::shared_ptr< Element_ABC > Primitive::Instanciate( const std::string& input ) const
{
    boost::shared_ptr< ElementTypeResolver > resolver( new ElementTypeResolver() );
    Function* element = new Function( input, name_, category_, types_.Instanciate( type_, resolver ) );
    BOOST_FOREACH( const T_Parameters::value_type& parameter, parameters_ )
        parameter->Declare( *element, resolver );
    return boost::shared_ptr< Element_ABC >( element );
}


