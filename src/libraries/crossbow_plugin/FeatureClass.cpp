// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "FeatureClass.h"
#include "FeatureRow.h"

namespace
{
    ITablePtr GetTable( IFeatureClassPtr featureClass )
    {
        ITablePtr table;
        featureClass.QueryInterface( IID_ITable, &table );
        return table;
    }
}

// -----------------------------------------------------------------------------
// Name: FeatureClass constructor
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
crossbow::FeatureClass::FeatureClass( IFeatureClassPtr featureClass, const std::string& name )
    : Table( GetTable( featureClass ), name )
    , name_( name )
    , featureClass_( featureClass )
    , spatialReference_()
    , feature_( 0 )
{
    InitializeSpatialReference();
    feature_.reset( new FeatureRow( spatialReference_ ) );
}

// -----------------------------------------------------------------------------
// Name: FeatureClass destructor
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
crossbow::FeatureClass::~FeatureClass()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FeatureClass::InitializeSpatialReference
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void crossbow::FeatureClass::InitializeSpatialReference()
{
    ISpatialReferenceFactoryPtr factory( CLSID_SpatialReferenceEnvironment );
    IGeographicCoordinateSystemPtr geoCoordSystem;
    // $$$$ SBO 2007-08-30: _NTF / _WGS1984
    if( factory == NULL || FAILED( factory->CreateGeographicCoordinateSystem( (int)esriSRGeoCS_WGS1984, &geoCoordSystem ) ) )
        throw std::runtime_error( "Unable to initialize coordinate system" );
    spatialReference_ = geoCoordSystem;

    IGeoDatasetSchemaEditPtr geoDatasetSchemaEdit;
    if( FAILED( featureClass_->QueryInterface( IID_IGeoDatasetSchemaEdit, (LPVOID*)&geoDatasetSchemaEdit ) ) )
        throw; // $$$$ SBO 2007-08-30: 
    VARIANT_BOOL canAlter;
    if( FAILED( geoDatasetSchemaEdit->get_CanAlterSpatialReference( &canAlter ) ) )
        throw; // $$$$ SBO 2007-08-30: 
    if( canAlter == VARIANT_TRUE )
        geoDatasetSchemaEdit->AlterSpatialReference( spatialReference_ );
}

// -----------------------------------------------------------------------------
// Name: FeatureClass::CreateRow
// Created: SBO 2007-08-31
// -----------------------------------------------------------------------------
crossbow::Row_ABC& crossbow::FeatureClass::CreateRow()
{
    IFeaturePtr feature;
    featureClass_->CreateFeature( &feature ); // $$$$ SBO 2007-08-30: check
    feature_->BindFeature( feature ); // $$$$ SBO 2007-08-30: only allows one row to be edited...
    return *feature_;
}

// -----------------------------------------------------------------------------
// Name: FeatureClass::UpdateRow
// Created: SBO 2007-09-26
// -----------------------------------------------------------------------------
void crossbow::FeatureClass::UpdateRow( const Row_ABC& row )
{
    if( &row != feature_.get() )
        return;
    feature_->Commit( cursor_ != NULL ? cursor_ : NULL );
    // feature_->Commit( InTransaction() ? cursor_ : NULL );
}

namespace
{
    void ThrowError()
    {
        IErrorInfoPtr ipError;
        BSTR strError;
        ::GetErrorInfo( 0, &ipError );
        ipError->GetDescription( &strError );

        TCHAR szFinal[ 255 ];
        _stprintf_s( szFinal, _T( "%s" ), ( LPCTSTR )_bstr_t( strError ) );
        MT_LOG_ERROR_MSG( szFinal ); // $$$$ SBO 2007-08-24: should throw
    }
}


// -----------------------------------------------------------------------------
// Name: FeatureClass::Find
// Created: SBO 2007-08-31
// -----------------------------------------------------------------------------
crossbow::Row_ABC* crossbow::FeatureClass::Find( const std::string& query )
{
    IQueryFilterPtr filter;

    if ( query != "" )
    {
        filter.CreateInstance( CLSID_QueryFilter );
        filter->put_WhereClause( CComBSTR( query.c_str() ) );
    }    
    HRESULT res = InTransaction() ? featureClass_->Update( filter, true, &cursor_ ) : featureClass_->Search( filter, true, &cursor_ );
    if( FAILED( res ) )
    {
        ThrowError();
        throw std::runtime_error( "Error executing query: " + query );
    }
    return GetNextRow();
}

// -----------------------------------------------------------------------------
// Name: FeatureClass::GetNextRow
// Created: SBO 2007-08-31
// -----------------------------------------------------------------------------
crossbow::Row_ABC* crossbow::FeatureClass::GetNextRow()
{
    IFeaturePtr feature;
    if( FAILED( cursor_->NextFeature( &feature ) ) || feature == NULL )
        return 0;
    feature_->BindFeature( feature );
    return feature_.get();
}
