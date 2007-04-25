// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterLimaList.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "Tools.h"

// -----------------------------------------------------------------------------
// Name: ActionParameterLimaList constructor
// Created: SBO 2007-04-16
// -----------------------------------------------------------------------------
ActionParameterLimaList::ActionParameterLimaList( const kernel::OrderParameter& parameter )
    : ActionParameter< QString >( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLimaList constructor
// Created: SBO 2007-04-16
// -----------------------------------------------------------------------------
ActionParameterLimaList::ActionParameterLimaList( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const ASN1T_LimasOrder& limas )
    : ActionParameter< QString >( parameter )
{
    limas_.reserve( limas.n );
    for( unsigned int i = 0; i < limas.n; ++i )
    {
        const ASN1T_LimaOrder& order = limas.elem[i];
        T_PointVector points;
        points.reserve( order.lima.vecteur_point.n );
        for( unsigned int j = 0; j < order.lima.vecteur_point.n; ++j )
            points.push_back( converter.ConvertToXY( order.lima.vecteur_point.elem[j] ) );
        QStringList functions;
        for( unsigned int j = 0; j < order.fonctions.n; ++j )
            functions.append( tools::ToString( (E_FuncLimaType)order.fonctions.elem[j] ) );
        AddLima( points, functions.join( "," ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLimaList destructor
// Created: SBO 2007-04-16
// -----------------------------------------------------------------------------
ActionParameterLimaList::~ActionParameterLimaList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLimaList::Draw
// Created: SBO 2007-04-16
// -----------------------------------------------------------------------------
void ActionParameterLimaList::Draw( const geometry::Point2f&, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    if( viewport.IsVisible( boundingBox_ ) && !limas_.empty() )
    {
        glPushAttrib( GL_LINE_BIT );
        glLineWidth( 3.f );
        for( T_Limas::const_iterator it = limas_.begin(); it != limas_.end(); ++it )
            if( !it->first.empty() )
            {
                tools.DrawLines( it->first );
                tools.Print( it->second.ascii(), it->first.back() );
            }
        glPopAttrib();
    }
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLimaList::AddLima
// Created: SBO 2007-04-16
// -----------------------------------------------------------------------------
void ActionParameterLimaList::AddLima( const T_PointVector& points, const QString& functions )
{
    limas_.push_back( std::make_pair( points, functions ) );
    for( CIT_PointVector it = points.begin(); it != points.end(); ++it )
        boundingBox_.Incorporate( *it );
}
