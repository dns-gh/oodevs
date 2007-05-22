// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterPath.h"
#include "ActionParameterPathPoint.h"
#include "ActionParameterVisitor_ABC.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Point.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: ActionParameterPath constructor
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
ActionParameterPath::ActionParameterPath( const QString& name, const CoordinateConverter_ABC& converter, const Location_ABC& location )
    : ActionParameter< QString >( name )
    , converter_( converter )
{
    location.Accept( *this );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPath constructor
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
ActionParameterPath::ActionParameterPath( const QString& name, const CoordinateConverter_ABC& converter, const ASN1T_Localisation& asn )
    : ActionParameter< QString >( name )
    , converter_( converter )
{
    AddPoints( asn );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPath constructor
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
ActionParameterPath::ActionParameterPath( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const Location_ABC& location )
    : ActionParameter< QString >( parameter )
    , converter_( converter )
{
    location.Accept( *this );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPath constructor
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
ActionParameterPath::ActionParameterPath( const OrderParameter& parameter, const CoordinateConverter_ABC& converter, const ASN1T_Localisation& asn )
    : ActionParameter< QString >( parameter )
    , converter_( converter )
{
    AddPoints( asn );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPath constructor
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
ActionParameterPath::ActionParameterPath( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, xml::xistream& xis )
    : ActionParameter< QString >( parameter )
    , converter_( converter )
{
    xis >> list( "parameter", *this, &ActionParameterPath::ReadPoint );
}

namespace
{
    QString ReadName( xml::xistream& xis )
    {
        std::string name;
        xis >> attribute( "name", name );
        return name.c_str();
    }
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPath constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
ActionParameterPath::ActionParameterPath( const kernel::CoordinateConverter_ABC& converter, xml::xistream& xis )
    : ActionParameter< QString >( ReadName( xis ) )
    , converter_( converter )
{
    xis >> list( "parameter", *this, &ActionParameterPath::ReadPoint );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPath destructor
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
ActionParameterPath::~ActionParameterPath()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPath::AddPoints
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
void ActionParameterPath::AddPoints( const ASN1T_Localisation& asn )
{
    for( unsigned int i = 0; i < asn.vecteur_point.n; ++i )
    {
        Point pt;
        pt.AddPoint( converter_.ConvertToXY( asn.vecteur_point.elem[i] ) );
        QString label;
        if( i + 1 == asn.vecteur_point.n )
            label = tools::translate( "ActionParameter", "Destination" );
        else
            label = tools::translate( "ActionParameter", "Way point %1" ).arg( i + 1 );
        AddParameter( *new ActionParameterPathPoint( label, converter_, pt ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPath::VisitLines
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
void ActionParameterPath::VisitLines( const T_PointVector& points )
{
    unsigned int i = 0;
    for( CIT_PointVector it = points.begin(); it != points.end(); ++it )
    {
        Point pt;
        pt.AddPoint( *it );
        QString label;
        if( ++i == points.size() )
            label = tools::translate( "ActionParameter", "Destination" );
        else
            label = tools::translate( "ActionParameter", "Way point %1" ).arg( i );
        AddParameter( *new ActionParameterPathPoint( label, converter_, pt ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPath::ReadPoint
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
void ActionParameterPath::ReadPoint( xml::xistream& xis )
{
    AddParameter( *new ActionParameterPathPoint( xis, converter_ ) );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPath::CommitTo
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void ActionParameterPath::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.value.t = T_MissionParameter_value_itineraire;
    ASN1T_Itineraire*& path = asn.value.u.itineraire = new ASN1T_Itineraire();
    CommitTo( *path );
    asn.null_value = path->vecteur_point.n ? 0 : 1;
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPath::Clean
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
void ActionParameterPath::Clean( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.itineraire )
        Clean( *asn.value.u.itineraire );
    delete asn.value.u.itineraire;
}

namespace
{
    struct AsnSerializer : public ActionParameterVisitor_ABC
    {
        explicit AsnSerializer( ASN1T_Itineraire& asn ) : asn_( &asn ), current_( 0 ) {}
        virtual void Visit( const ActionParameterPathPoint& param )
        {
            param.CommitTo( asn_->vecteur_point.elem[current_++] );
        }

        ASN1T_Itineraire* asn_;
        unsigned int current_;
    };
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPath::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void ActionParameterPath::CommitTo( ASN1T_Itineraire& asn ) const
{
    asn.type = EnumTypeLocalisation::line;
    asn.vecteur_point.n = Count();
    asn.vecteur_point.elem = new ASN1T_CoordUTM[asn.vecteur_point.n];
    AsnSerializer serializer( asn );
    Accept( serializer );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPath::Clean
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void ActionParameterPath::Clean( ASN1T_Itineraire& asn ) const
{
    if( asn.vecteur_point.n )
        delete[] asn.vecteur_point.elem;
}

// -----------------------------------------------------------------------------
// Name: ActionParameterPath::Accept
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void ActionParameterPath::Accept( ActionParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
    ActionParameter< QString >::Accept( visitor );
}
