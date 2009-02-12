// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "esri_plugin_pch.h"
#include "Row.h"

using namespace plugins;

// -----------------------------------------------------------------------------
// Name: Row constructor
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
esri::Row::Row()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Row destructor
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
esri::Row::~Row()
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

// -----------------------------------------------------------------------------
// Name: Row::SetField
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void esri::Row::SetField( const std::string& name, const esri::FieldVariant& value )
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
void esri::Row::SetShape( const Shape_ABC& /*value*/ )
{
    throw std::runtime_error( "Trying to set shape for table." );
}

namespace
{
    esri::FieldVariant ConvertVariant( const CComVariant& value )
    {
        switch( value.vt )
        {
        case VT_I2:
            return esri::FieldVariant( (int)value.iVal );
        case VT_I4:
            return esri::FieldVariant( value.lVal );
        case VT_BSTR:
            return esri::FieldVariant( std::string( _bstr_t( value.bstrVal ) ) ); // $$$$ SBO 2007-05-31: !!
        case VT_BOOL:
            return esri::FieldVariant( value.bVal != 0 );
        }
        throw;
    }
}

// -----------------------------------------------------------------------------
// Name: Row::GetField
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
esri::FieldVariant esri::Row::GetField( const std::string& name ) const
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
long esri::Row::GetID() const
{
    long id;
    
    row_->get_OID( &id );
    return id;
}

// -----------------------------------------------------------------------------
// Name: Row::GetShape
// Created: SBO 2007-09-26
// -----------------------------------------------------------------------------
esri::Shape_ABC& esri::Row::GetShape() const
{
    throw std::runtime_error( "Trying to get shape from a table" );
}

// -----------------------------------------------------------------------------
// Name: Row::Commit
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void esri::Row::Commit( ICursorPtr cursor /*=NULL*/)
{
    if( cursor != NULL )
        cursor->UpdateRow( row_ );
    else
        row_->Store();
}

// -----------------------------------------------------------------------------
// Name: Row::BindRow
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void esri::Row::BindRow( IRowPtr row )
{
    row_ = row;
}

// -----------------------------------------------------------------------------
// Name: Row::GetFieldIndex
// Created: SBO 2007-08-31
// -----------------------------------------------------------------------------
long esri::Row::GetFieldIndex( const std::string& name ) const
{
    IFieldsPtr fields; 
    row_->get_Fields( &fields );
    long index;
    HRESULT res;
    if( FAILED( res = fields->FindField( CComBSTR( name.c_str() ), &index ) ) )
        return -1;
    return index;
}
