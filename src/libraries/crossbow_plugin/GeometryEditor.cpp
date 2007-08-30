// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "GeometryEditor.h"

using namespace crossbow;

// -----------------------------------------------------------------------------
// Name: GeometryEditor::GeometryEditor
// Created: JCR 2007-05-15
// -----------------------------------------------------------------------------
GeometryEditor::GeometryEditor( ISpatialReferencePtr spSpatialReference )
    : spSpatialReference_ ( spSpatialReference )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GeometryEditor::~GeometryEditor
// Created: JCR 2007-05-15
// -----------------------------------------------------------------------------
GeometryEditor::~GeometryEditor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GeometryEditor::Create
// Created: JCR 2007-05-15
// -----------------------------------------------------------------------------
void GeometryEditor::Create( IPointPtr& spPoint, bool bUpdateZAxis /*= false*/ )
{    
    spPoint.CreateInstance( CLSID_Point );
    spPoint->putref_SpatialReference( spSpatialReference_ );
    if ( bUpdateZAxis )
        UpdateZAware( spPoint );
}

// -----------------------------------------------------------------------------
// Name: GeometryEditor::UpdateZAware
// Created: JCR 2007-05-15
// -----------------------------------------------------------------------------
void GeometryEditor::UpdateZAware( IPointPtr spPoint ) const
{
    VARIANT_BOOL    bZAware;
    IZAwarePtr      spZAware;

    spPoint.QueryInterface( IID_IZAware, &spZAware );            
    spZAware->get_ZAware( &bZAware );
    if ( bZAware == VARIANT_FALSE )
        spZAware->put_ZAware( VARIANT_TRUE );   
}

// -----------------------------------------------------------------------------
// Name: GeometryEditor::Validate
// Created: JCR 2007-05-15
// -----------------------------------------------------------------------------
void GeometryEditor::Validate( IFeaturePtr spFeature, IPointPtr& spPoint )
{    
    IGeometryPtr    spGeometry;
        
    if ( FAILED( spFeature->get_Shape( &spGeometry ) ) )
        Create( spPoint );
    else
    {
        esriGeometryType shapeType;
        spGeometry->get_GeometryType( &shapeType );
        assert( shapeType == esriGeometryPoint );
        spGeometry->QueryInterface( IID_IPoint, (LPVOID*)&spPoint );
    }
    UpdateZAware( spPoint );
}

// -----------------------------------------------------------------------------
// Name: GeometryEditor::UpdateCoord
// Created: JCR 2007-05-15
// -----------------------------------------------------------------------------
bool GeometryEditor::UpdateCoord( IPointPtr spPoint, const ASN1T_CoordUTM& asnUTM )
{
    const std::string   strMgrs( (const char*)asnUTM.data, 15 );
    IPointPtr           spPointWGS;
    double              x, y;

    ConvertCoordMosToSim( strMgrs, spPointWGS );
    if ( spPointWGS == NULL )
        return false;
    
    spPointWGS->QueryCoords( &x, &y );
    spPoint->PutCoords( x, y );
    spPoint->put_Z( 0. );
    return true;
}

// -----------------------------------------------------------------------------
// Name: GeometryEditor::Create
// Created: JCR 2007-05-15
// -----------------------------------------------------------------------------
void GeometryEditor::Create( ILinePtr& spLine )
{
    spLine.CreateInstance( CLSID_Line );
    spLine->putref_SpatialReference( spSpatialReference_ );
}

// -----------------------------------------------------------------------------
// Name: GeometryEditor::Create
// Created: JCR 2007-05-15
// -----------------------------------------------------------------------------
void GeometryEditor::Create( IPolylinePtr& spPolyline )
{
    VARIANT_BOOL    bZAware;
    IZAwarePtr      spZAware;

    spPolyline.CreateInstance( CLSID_Polyline );
    spPolyline->putref_SpatialReference( spSpatialReference_ );    
    spPolyline.QueryInterface( IID_IZAware, &spZAware );            
    spZAware->get_ZAware( &bZAware );
    if ( bZAware == VARIANT_FALSE )
        spZAware->put_ZAware( VARIANT_TRUE ); 
}


namespace 
{
    void check_error()
    {        
        IErrorInfoPtr ipError;
        BSTR          strError;

        ::GetErrorInfo( 0, &ipError );
        ipError->GetDescription( &strError );
        MT_LOG_ERROR_MSG( strError );
    }
}

// -----------------------------------------------------------------------------
// Name: GeometryEditor::ConvertCoordMosToSim
// Created: JCR 2007-05-03
// -----------------------------------------------------------------------------
void GeometryEditor::ConvertCoordMosToSim( const std::string& strMgrs, IPointPtr& spPoint ) const
{    
    IPointPtr   spPointWGS;
    BSTR        stDMS;
    BSTR        stUTM;
    BSTR        stMGRS;
        
    ICoordinateToolPtr spConverter( CLSID_CoordinateTool ); //WGS 1984 (WGS84)
    if ( FAILED( spConverter->ConvertLocation( CComVariant( strMgrs.c_str() ), 4, CComVariant( L"WGS 1984 (WGS84)" ), CComVariant( L"WGS 1984 (WGS84)" ), &spPointWGS, &spPoint, &stDMS, &stUTM, &stMGRS ) ) )
        check_error();
}

