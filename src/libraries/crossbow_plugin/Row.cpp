// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "Row.h"
#include <boost/lexical_cast.hpp>
#include <boost/variant.hpp>

using namespace plugins;

// -----------------------------------------------------------------------------
// Name: Row constructor
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
crossbow::Row::Row()
    : id_( -1 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Row destructor
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
crossbow::Row::~Row()
{
    // NOTHING
}

namespace
{
    struct ValueVisitor : public boost::static_visitor< CComVariant >
    {
        CComVariant operator()( int value ) const
        {
            return CComVariant( value );
        }

        CComVariant operator()( long value ) const
        {
            return CComVariant( value );
        }

        CComVariant operator()( const std::string& value ) const
        {
            return CComVariant( value.c_str() );
        }

        CComVariant operator()( bool value ) const
        {
            return CComVariant( value );
        }
    };
}

namespace
{
    crossbow::FieldVariant ConvertVariant( const CComVariant& value )
    {
        switch( value.vt )
        {
        case VT_NULL:
            return crossbow::FieldVariant( std::string( "null" ) );
        case VT_I2:
            return crossbow::FieldVariant( (int)value.iVal );
        case VT_I4:
            return crossbow::FieldVariant( value.lVal );
        case VT_BSTR:
            return crossbow::FieldVariant( std::string( _bstr_t( value.bstrVal ) ) ); // $$$$ SBO 2007-05-31: !!
        case VT_BOOL:
            return crossbow::FieldVariant( value.bVal != 0 );
        }
        throw;
    }
}

// -----------------------------------------------------------------------------
// Name: Row::SetField
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void crossbow::Row::SetField( const std::string& name, const crossbow::FieldVariant& value )
{
    const long index = GetFieldIndex( name );
    if( index < 0 )
        return; // $$$$ SBO 2007-08-31: throw
    row_->put_Value( index, boost::apply_visitor( ValueVisitor(), value ) );
}

// -----------------------------------------------------------------------------
// Name: Row::SetShape
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void crossbow::Row::SetShape( const Shape_ABC& /*value*/ )
{
    throw std::runtime_error( "Trying to set shape for table." );
}

// -----------------------------------------------------------------------------
// Name: Row::GetField
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
crossbow::FieldVariant crossbow::Row::GetField( const std::string& name ) const
{
    const long index = GetFieldIndex( name );
    if( index < 0 )
        throw; // $$$$ SBO 2007-08-31: something
    CComVariant value;
    row_->get_Value( index, &value );
    return ConvertVariant( value );
}

// -----------------------------------------------------------------------------
// Name: Row::GetID
// Created: JCR 2008-07-25
// -----------------------------------------------------------------------------
long crossbow::Row::GetID() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: Row::GetShape
// Created: SBO 2007-09-26
// -----------------------------------------------------------------------------
crossbow::Shape_ABC& crossbow::Row::GetShape() const
{
    throw std::runtime_error( "Trying to get shape from a table" );
}

namespace
{
    void ThrowError()
    {
        IErrorInfoPtr ipError;
        BSTR strError;
        ::GetErrorInfo( 0, &ipError );
        ipError->GetDescription( &strError );
        const std::string error = boost::lexical_cast< std::string >( _bstr_t( strError ) );
        MT_LOG_ERROR_MSG( error ); // $$$$ SBO 2008-05-15: should throw
    }
}

// -----------------------------------------------------------------------------
// Name: Row::Commit
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void crossbow::Row::Commit( ICursorPtr cursor /*=NULL*/ )
{
    if( cursor != NULL ) 
    {
        CComVariant value;
        HRESULT res = cursor->InsertRow( row_, &value );
        if( FAILED( res ) )
            ThrowError();
		else
		{
			id_ = boost::get< int >( ConvertVariant( value ) );
		}
    }
}


// -----------------------------------------------------------------------------
// Name: Row::BindRow
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void crossbow::Row::BindRow( IRowBufferPtr row, long id )
{
    row_ = row;
    id_ = id;
}

// -----------------------------------------------------------------------------
// Name: Row::GetFieldIndex
// Created: SBO 2007-08-31
// -----------------------------------------------------------------------------
long crossbow::Row::GetFieldIndex( const std::string& name ) const
{
    IFieldsPtr fields; 
    row_->get_Fields( &fields );
    long index;
    HRESULT res;
    if( FAILED( res = fields->FindField( CComBSTR( name.c_str() ), &index ) ) )
        return -1;
    return index;
}
