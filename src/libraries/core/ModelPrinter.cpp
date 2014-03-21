// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "ModelPrinter.h"
#include "Model.h"
#include "UserData_ABC.h"
#include <boost/lexical_cast.hpp>
#include <ostream>
#include <iomanip>

using namespace core;

// -----------------------------------------------------------------------------
// Name: ModelPrinter constructor
// Created: MCO 2012-04-12
// -----------------------------------------------------------------------------
ModelPrinter::ModelPrinter( const Model& model, std::ostream& os, bool quotes, std::size_t indent )
    : os_     ( os )
    , quotes_ ( quotes )
    , indent_ ( indent )
    , depth_  ( 0 )
    , child_  ( false )
    , element_( false )
    , key_    ( false )
{
    model.Accept( *this );
}

// -----------------------------------------------------------------------------
// Name: ModelPrinter constructor
// Created: MCO 2012-06-14
// -----------------------------------------------------------------------------
ModelPrinter::ModelPrinter( const ModelPrinter& parent, const Model& model, bool key )
    : os_     ( parent.os_ )
    , quotes_ ( parent.quotes_ )
    , indent_ ( parent.indent_ )
    , depth_  ( parent.depth_ + indent_ )
    , child_  ( false )
    , element_( false )
    , key_    ( key )
{
    model.Accept( *this );
}

// -----------------------------------------------------------------------------
// Name: ModelPrinter destructor
// Created: MCO 2012-04-12
// -----------------------------------------------------------------------------
ModelPrinter::~ModelPrinter()
{
    Close( '}', child_ );
    Close( ']', element_ );
}

// -----------------------------------------------------------------------------
// Name: ModelPrinter::Open
// Created: MCO 2012-06-14
// -----------------------------------------------------------------------------
void ModelPrinter::Open( char prefix, bool open, const std::string& key )
{
    if( open )
        os_ << ",";
    else
    {
        if( key_ )
            Indent();
        os_ << prefix;
    }
    Indent( indent_ );
    if( ! indent_ )
        os_ << " ";
    os_ << key;
}

// -----------------------------------------------------------------------------
// Name: ModelPrinter::Close
// Created: MCO 2012-06-14
// -----------------------------------------------------------------------------
void ModelPrinter::Close( char postfix, bool open )
{
    if( open )
    {
        Indent();
        if( indent_ )
            os_ << postfix;
        else
            os_ << " " << postfix;
    }
}

// -----------------------------------------------------------------------------
// Name: ModelPrinter::Indent
// Created: MCO 2012-06-14
// -----------------------------------------------------------------------------
void ModelPrinter::Indent( std::size_t indent )
{
    if( indent_ )
        os_ << std::endl << std::setw( depth_ + indent ) << "";
}

// -----------------------------------------------------------------------------
// Name: ModelPrinter::QuoteString
// Created: MCO 2012-04-16
// -----------------------------------------------------------------------------
std::string ModelPrinter::QuoteString( const std::string& s ) const
{
    if( quotes_ )
        return '"' + s + '"';
    return s;
}

// -----------------------------------------------------------------------------
// Name: ModelPrinter::Visit
// Created: MCO 2012-04-12
// -----------------------------------------------------------------------------
void ModelPrinter::Visit( int64_t value )
{
    os_ << value;
}

// -----------------------------------------------------------------------------
// Name: ModelPrinter::Visit
// Created: MCO 2012-04-12
// -----------------------------------------------------------------------------
void ModelPrinter::Visit( uint64_t value )
{
    os_ << value;
}

// -----------------------------------------------------------------------------
// Name: ModelPrinter::Visit
// Created: MCO 2012-04-12
// -----------------------------------------------------------------------------
void ModelPrinter::Visit( double value )
{
    os_ << value;
}

// -----------------------------------------------------------------------------
// Name: ModelPrinter::Visit
// Created: MCO 2012-04-12
// -----------------------------------------------------------------------------
void ModelPrinter::Visit( const std::string& value )
{
    os_ << QuoteString( value );
}

// -----------------------------------------------------------------------------
// Name: ModelPrinter::Visit
// Created: MCO 2012-04-12
// -----------------------------------------------------------------------------
void ModelPrinter::Visit( const std::string& key, const Model& child )
{
    Open( '{', child_, QuoteString( key ) + ": " );
    child_ = true;
    ModelPrinter( *this, child, true );
}

// -----------------------------------------------------------------------------
// Name: ModelPrinter::Visit
// Created: MCO 2012-09-25
// -----------------------------------------------------------------------------
void ModelPrinter::Visit( std::size_t key, const Model& child )
{
    Open( '{', child_, boost::lexical_cast< std::string >( key ) + ": " );
    child_ = true;
    ModelPrinter( *this, child, true );
}

// -----------------------------------------------------------------------------
// Name: ModelPrinter::Visit
// Created: MCO 2012-04-12
// -----------------------------------------------------------------------------
void ModelPrinter::Visit( const Model& element )
{
    Open( '[', element_, "" );
    element_ = true;
    ModelPrinter( *this, element, false );
}

// -----------------------------------------------------------------------------
// Name: ModelPrinter::Visit
// Created: MCO 2012-04-12
// -----------------------------------------------------------------------------
void ModelPrinter::Visit( const boost::shared_ptr< UserData_ABC >& data )
{
    os_ << boost::lexical_cast< std::string >( data->Get() );
}
