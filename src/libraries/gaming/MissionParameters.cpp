// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "MissionParameters.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/GlTools_ABC.h"

// -----------------------------------------------------------------------------
// Name: MissionParameters constructor
// Created: SBO 2006-11-13
// -----------------------------------------------------------------------------
MissionParameters::MissionParameters( const kernel::CoordinateConverter_ABC& converter )
    : converter_( converter )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: MissionParameters destructor
// Created: SBO 2006-11-13
// -----------------------------------------------------------------------------
MissionParameters::~MissionParameters()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameters::Clear
// Created: SBO 2006-11-20
// -----------------------------------------------------------------------------
void MissionParameters::Clear()
{
    rightLimit_.clear();
    leftLimit_.clear();
    limas_.clear();
}

// -----------------------------------------------------------------------------
// Name: MissionParameters::DecodePointList
// Created: SBO 2006-11-13
// -----------------------------------------------------------------------------
void MissionParameters::DecodePointList( const ASN1T_Line& src, T_PointVector& dest )
{
    dest.reserve( src.vecteur_point.n );
    for( unsigned int i = 0; i < src.vecteur_point.n; ++i )
        dest.push_back( converter_.ConvertToXY( src.vecteur_point.elem[i] ) );
}

// -----------------------------------------------------------------------------
// Name: MissionParameters::AddLima
// Created: SBO 2006-11-20
// -----------------------------------------------------------------------------
void MissionParameters::AddLima( const ASN1T_LimaOrder& lima )
{
    T_PointVector points;
    DecodePointList( lima.lima, points );
    T_LimaFunctions functions;
    functions.reserve( lima.fonctions.n );
    for( unsigned int i = 0; i < lima.fonctions.n; ++i )
        functions.push_back( (E_FuncLimaType)lima.fonctions.elem[i] );
    limas_.push_back( std::make_pair( points, functions ) );
}

// -----------------------------------------------------------------------------
// Name: MissionParameters::DoUpdateOrderContext
// Created: SBO 2006-11-20
// -----------------------------------------------------------------------------
void MissionParameters::DoUpdateOrderContext( const ASN1T_OrderContext& context )
{
    if( context.m.limite_gauchePresent )
        DecodePointList( context.limite_gauche, leftLimit_ );
    if( context.m.limite_droitePresent )
        DecodePointList( context.limite_gauche, rightLimit_ );
    limas_.reserve( context.limas.n );
    for( unsigned int i = 0; i < context.limas.n; ++i )
        AddLima( context.limas.elem[i] );
}

// -----------------------------------------------------------------------------
// Name: MissionParameters::DoUpdate
// Created: SBO 2006-11-13
// -----------------------------------------------------------------------------
void MissionParameters::DoUpdate( const ASN1T_MsgPionOrder& message )
{
    Clear();
    DoUpdateOrderContext( message.order_context );
}

// -----------------------------------------------------------------------------
// Name: MissionParameters::DoUpdate
// Created: SBO 2006-11-20
// -----------------------------------------------------------------------------
void MissionParameters::DoUpdate( const ASN1T_MsgAutomateOrder& message )
{
    Clear();
    DoUpdateOrderContext( message.order_context );
}

// -----------------------------------------------------------------------------
// Name: MissionParameters::DoUpdateOrderManagement
// Created: SBO 2006-11-20
// -----------------------------------------------------------------------------
template< typename T >
void MissionParameters::DoUpdateOrderManagement( const T& message )
{
    if( message.etat == EnumOrderState::stopped || message.etat == EnumOrderState::cancelled )
        Clear();
}

// -----------------------------------------------------------------------------
// Name: MissionParameters::DoUpdate
// Created: SBO 2006-11-20
// -----------------------------------------------------------------------------
void MissionParameters::DoUpdate( const ASN1T_MsgPionOrderManagement& message )
{
    DoUpdateOrderManagement( message );
}

// -----------------------------------------------------------------------------
// Name: MissionParameters::DoUpdate
// Created: SBO 2006-11-20
// -----------------------------------------------------------------------------
void MissionParameters::DoUpdate( const ASN1T_MsgAutomateOrderManagement& message )
{
    DoUpdateOrderManagement( message );
}

// -----------------------------------------------------------------------------
// Name: MissionParameters::Draw
// Created: SBO 2006-11-13
// -----------------------------------------------------------------------------
void MissionParameters::Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const kernel::GlTools_ABC& tools ) const
{
    // $$$$ AGE 2006-04-21: viewport
//    if( ! tools.ShouldDisplay( "TacticalLines", false )  // $$$$ SBO 2006-11-13: not really TacticalLines... Mission Parameters
//       && tools.ShouldDisplay( "TacticalLines" ) )
    {
        bool state = tools.Select( true );
        if( !rightLimit_.empty() )
            tools.DrawLines( rightLimit_ ); // $$$$ SBO 2006-11-13: do something better
        if( !leftLimit_.empty() )
            tools.DrawLines( leftLimit_ );
        for( unsigned int i = 0; i < limas_.size(); ++i )
        {
            tools.DrawLines( limas_[i].first );
            // $$$$ SBO 2006-11-20: display lima functions
        }
        tools.Select( state );
    }
}
