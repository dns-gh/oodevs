// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "OGR_FeatureClass.h"
#include "OGR_FeatureRow.h"
#include "DatabaseEditor.h"

#include <gdal/gdal.h>
#include <gdal/ogr_feature.h>
#include <gdal/ogrsf_frmts.h>

using namespace plugins;
using namespace crossbow;

// -----------------------------------------------------------------------------
// Name: OGR_FeatureClass constructor
// Created: JCR 2010-02-25
// -----------------------------------------------------------------------------
OGR_FeatureClass::OGR_FeatureClass( OGRLayer& layer )
    : layer_ ( layer )
    , editor_ ( new DatabaseEditor( layer_ ) )
    , feature_ ( new OGR_FeatureRow( layer_.GetSpatialRef() ) )
{
    layer_.ResetReading();
    BeginTransaction();
}

// -----------------------------------------------------------------------------
// Name: OGR_FeatureClass destructor
// Created: JCR 2010-02-25
// -----------------------------------------------------------------------------
OGR_FeatureClass::~OGR_FeatureClass()
{
    EndTransaction();
}

// -----------------------------------------------------------------------------
// Name: FeatureClass::CreateRow
// Created: SBO 2007-08-31
// -----------------------------------------------------------------------------
Row_ABC& OGR_FeatureClass::CreateRow()
{
    boost::shared_ptr< OGRFeature > poFeature( OGRFeature::CreateFeature( layer_.GetLayerDefn() ), &OGRFeature::DestroyFeature );
    feature_->BindFeature( poFeature, poFeature->GetFID() );
    return *feature_;
}

// -----------------------------------------------------------------------------
// Name: OGR_FeatureClass::GetNextRow
// Created: JCR 2010-02-24
// -----------------------------------------------------------------------------
Row_ABC* OGR_FeatureClass::GetNextRow()
{
    boost::shared_ptr< OGRFeature > poFeature( layer_.GetNextFeature(), &OGRFeature::DestroyFeature );
    if( poFeature.get() == NULL )
        return NULL;
    feature_->BindFeature( poFeature, poFeature->GetFID() );
    return feature_.get();
}

// -----------------------------------------------------------------------------
// Name: OGR_FeatureClass::Find
// Created: JCR 2010-02-25
// -----------------------------------------------------------------------------
Row_ABC* OGR_FeatureClass::Find( const std::string& query, bool /* forceUpdate = false*/ )
{
    layer_.ResetReading();
    if( query.length() > 0 )
        layer_.SetAttributeFilter( query.c_str() );
    return GetNextRow();
}

// -----------------------------------------------------------------------------
// Name: OGR_FeatureClass::DeleteRows
// Created: JCR 2010-02-25
// -----------------------------------------------------------------------------
void OGR_FeatureClass::DeleteRows( const std::string& query )
{
    Row_ABC* row = Find( query );
    while ( row != NULL )
    {
        layer_.DeleteFeature( row->GetID() );
        row = GetNextRow();
    }
}

// -----------------------------------------------------------------------------
// Name: OGR_FeatureClass::UpdateRow
// Created: JCR 2010-02-25
// -----------------------------------------------------------------------------
void OGR_FeatureClass::UpdateRow( const Row_ABC& row )
{
    if( &row != feature_.get() )
        return;
    feature_->Update( layer_ );
}

// -----------------------------------------------------------------------------
// Name: OGR_FeatureClass::InsertRow
// Created: JCR 2010-02-25
// -----------------------------------------------------------------------------
void OGR_FeatureClass::InsertRow( const Row_ABC& row )
{
    if( &row != feature_.get() )
        return;
    feature_->Insert( layer_ );
}

// -----------------------------------------------------------------------------
// Name: OGR_FeatureClass::Clear
// Created: JCR 2010-02-25
// -----------------------------------------------------------------------------
void OGR_FeatureClass::Clear()
{
    // $$$$ TODO ?
}

// -----------------------------------------------------------------------------
// Name: OGR_FeatureClass::Flush
// Created: JCR 2011-01-24
// -----------------------------------------------------------------------------
void OGR_FeatureClass::Flush()
{
    if( OGRERR_NONE != layer_.SyncToDisk() )
        throw std::runtime_error( "Crossbow plugin : OGR_Database::SyncToDisk failed" );
}

// -----------------------------------------------------------------------------
// Name: OGR_FeatureClass::BeginTransaction
// Created: JCR 2010-02-27
// -----------------------------------------------------------------------------
void OGR_FeatureClass::BeginTransaction()
{
    editor_->BeginTransaction();
}

// -----------------------------------------------------------------------------
// Name: OGR_FeatureClass::EndTransaction
// Created: JCR 2010-02-27
// -----------------------------------------------------------------------------
void OGR_FeatureClass::EndTransaction()
{
   editor_->EndTransaction();
}
