// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "ObjectListener.h"
#include "Connector.h"
#include "dispatcher/Network_Def.h"

using namespace crossbow;

// -----------------------------------------------------------------------------
// Name: ObjectListener constructor
// Created: SBO 2007-09-23
// -----------------------------------------------------------------------------
ObjectListener::ObjectListener( Connector& connector, dispatcher::SimulationPublisher_ABC& publisher )
    : publisher_            ( publisher )
    , featureClass_         ( connector.GetFeatureClass( "TacticalObjectPoint" ) )
    , pendingCreationFilter_( CLSID_QueryFilter )
    , cursor_               ()
{
    featureClass_.QueryInterface( IID_ITable, &table_ );
    pendingCreationFilter_->put_WhereClause( CComBSTR( "`Info`<>\"\"" ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectListener destructor
// Created: SBO 2007-09-23
// -----------------------------------------------------------------------------
ObjectListener::~ObjectListener()
{
    pendingCreationFilter_.Release();
}

// -----------------------------------------------------------------------------
// Name: ObjectListener::Listen
// Created: SBO 2007-09-23
// -----------------------------------------------------------------------------
void ObjectListener::Listen()
{
    IFeaturePtr feature;
    HRESULT res = featureClass_->Search( pendingCreationFilter_, false, &cursor_ );
    if( FAILED( res ) )
        throw std::runtime_error( "Search failed" ); // $$$$ SBO 2007-05-30:

    cursor_->NextFeature( &feature );
    while( feature != 0 )
    {
        SendCreation( feature );
        cursor_->NextFeature( &feature );
    }
    CleanPending();
}

namespace
{
    // $$$$ SBO 2007-09-23: factorize this stuff !
    CComVariant GetFieldValue( const IRowPtr& row, const std::string& name )
    {
        IFieldsPtr fields;
        row->get_Fields( &fields );
        long index;
        fields->FindField( CComBSTR( name.c_str() ), &index );
        CComVariant value;
        row->get_Value( index, &value );
        return value;
    }

    std::string GetString( const CComVariant& value )
    {
        switch( value.vt )
        {
        case VT_BSTR:
            return std::string( _bstr_t( value.bstrVal ) ); // $$$$ SBO 2007-05-31: !!
        default:
            return "";
        }
    }

    // $$$$ SBO 2007-09-23: hard coded object list
    unsigned int GetType( const std::string& type )
    {
        if( type == "fire" )
            return EnumObjectType::fire;
        if( type == "emergency shelter" )
            return EnumObjectType::emergency_shelter;
        throw std::runtime_error( "unsupported object type" );
    }
}

// -----------------------------------------------------------------------------
// Name: ObjectListener::SendCreation
// Created: SBO 2007-09-23
// -----------------------------------------------------------------------------
void ObjectListener::SendCreation( const IFeaturePtr feature )
{
    dispatcher::AsnMsgClientToSimObjectMagicAction asn;
    asn().action.t = T_MsgObjectMagicAction_action_create_object;
    ASN1T_MagicActionCreateObject*& creation = asn().action.u.create_object = new ASN1T_MagicActionCreateObject();
    creation->m.namePresent = 0;
    creation->m.obstacle_typePresent = 0;
    creation->m.reserved_obstacle_activatedPresent = 0;
    creation->m.specific_attributesPresent = 0;
    creation->team = 1; // $$$$ SBO 2007-09-23: 
    creation->type = (ASN1T_EnumObjectType)GetType( GetString( GetFieldValue( feature, "Info" ) ) );
    IGeometryPtr shape;
    feature->get_Shape( &shape );
    SerializeLocation( creation->location, shape );
    asn.Send( publisher_ );
}

// -----------------------------------------------------------------------------
// Name: ObjectListener::CleanPending
// Created: SBO 2007-09-23
// -----------------------------------------------------------------------------
void ObjectListener::CleanPending()
{
    HRESULT res = table_->DeleteSearchedRows( pendingCreationFilter_ );
    if( FAILED( res ) )
        throw std::runtime_error( "Cleaning failed" ); // $$$$ SBO 2007-05-30:
}

namespace
{
    std::string ToMgrs( const IPointPtr point )
    {
        IPointPtr   wgs, datum;
        BSTR        dms, utm, mgrs;
        ICoordinateToolPtr converter( CLSID_CoordinateTool ); //WGS 1984 (WGS84)
        if ( FAILED( converter->ConvertLocation( CComVariant( (IPoint*)point ), 1, CComVariant( L"WGS 1984 (WGS84)" ), CComVariant( L"WGS 1984 (WGS84)" ), &wgs, &datum, &dms, &utm, &mgrs ) ) )
            throw std::runtime_error( "Conversion failed" );
        return _com_util::ConvertBSTRToString( mgrs );
    }
}

// -----------------------------------------------------------------------------
// Name: ObjectListener::SerializeLocation
// Created: SBO 2007-05-31
// -----------------------------------------------------------------------------
void ObjectListener::SerializeLocation( ASN1T_Location& asn, IGeometryPtr geometry ) const
{
    IPointPtr point;
    if( !FAILED( geometry.QueryInterface( IID_IPoint, &point ) ) )
    {
        asn.type = EnumLocationType::point;
        asn.coordinates.n = 1;
        asn.coordinates.elem = new ASN1T_CoordUTM[1];
        asn.coordinates.elem[0] = ToMgrs( point ).c_str();
    }
}
