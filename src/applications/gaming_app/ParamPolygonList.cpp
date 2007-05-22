// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamPolygonList.h"
#include "ParamLocation.h"
#include "ParamVisitor_ABC.h"
#include "gaming/Action_ABC.h"
#include "gaming/ActionParameterPolygonList.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ParamPolygonList constructor
// Created: SBO 2007-03-15
// -----------------------------------------------------------------------------
ParamPolygonList::ParamPolygonList( QObject* parent, const OrderParameter& parameter, gui::ParametersLayer& layer, const CoordinateConverter_ABC& converter, ActionController& controller )
    : ParamLocationList( parent, parameter, layer, converter, controller )
    , parameter_( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamPolygonList destructor
// Created: SBO 2007-03-15
// -----------------------------------------------------------------------------
ParamPolygonList::~ParamPolygonList()
{
    // NOTHING
}

namespace
{
    class AsnSerializer : public ParamVisitor_ABC
    {
    public:
        AsnSerializer( ASN1T_ListPolygon& list )
            : list_( list )
            , index_( 0 )
        {}

        virtual void Visit( const Param_ABC& param )
        {
            if( index_ < list_.n )
                static_cast< const ParamLocation& >( param ).CommitTo( list_.elem[index_++] );
        }

    private:
        ASN1T_ListPolygon& list_;
        unsigned int index_;
    };
}

// -----------------------------------------------------------------------------
// Name: ParamPolygonList::CommitTo
// Created: SBO 2007-03-15
// -----------------------------------------------------------------------------
void ParamPolygonList::CommitTo( ASN1T_MissionParameter& asn ) const
{
    ASN1T_ListPolygon*& list = asn.value.u.listPolygon = new ASN1T_ListPolygon();
    asn.value.t = T_MissionParameter_value_listPolygon;
    list->n = Count();
    asn.null_value = list->n ? 0 : 1;
    if( asn.null_value )
        return;
    list->elem = new ASN1T_Polygon[ list->n ];
    AsnSerializer serializer( *list );
    Accept( serializer );
}

// -----------------------------------------------------------------------------
// Name: ParamPolygonList::Clean
// Created: SBO 2007-03-15
// -----------------------------------------------------------------------------
void ParamPolygonList::Clean( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.listPolygon )
    {
        for( unsigned int i = 0; i < asn.value.u.listPolygon->n; ++i )
            delete[] asn.value.u.listPolygon->elem[i].vecteur_point.elem;
        delete[] asn.value.u.listPolygon->elem;
    }
    delete asn.value.u.listPolygon;
}

namespace
{
    class ActionSerializer : public ParamVisitor_ABC
    {
    public:
        ActionSerializer( ActionParameter_ABC& parent )
            : parent_( parent )
        {}

        virtual void Visit( const Param_ABC& param )
        {
            static_cast< const ParamLocation& >( param ).CommitTo( parent_ );
        }

    private:
        ActionParameter_ABC& parent_;
    };
}

// -----------------------------------------------------------------------------
// Name: ParamPolygonList::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
void ParamPolygonList::CommitTo( Action_ABC& action ) const
{
    std::auto_ptr< ActionParameter_ABC > param( new ActionParameterPolygonList( parameter_ ) );
    ActionSerializer serializer( *param );
    Accept( serializer );
    action.AddParameter( *param.release() );
}

// -----------------------------------------------------------------------------
// Name: ParamPolygonList::CreateElement
// Created: SBO 2007-04-27
// -----------------------------------------------------------------------------
Param_ABC* ParamPolygonList::CreateElement()
{
    Param_ABC* param = ParamLocationList::CreateElement();
    static_cast< ParamLocation* >( param )->SetShapeFilter( false, false, true, false );
    return param;
}
