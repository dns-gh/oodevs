// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamPointList.h"
#include "ParamLocation.h"
#include "ParamLocationList.h"
#include "ParamVisitor_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ParamPointList constructor
// Created: SBO 2007-03-15
// -----------------------------------------------------------------------------
ParamPointList::ParamPointList( QObject* parent, const OrderParameter& parameter, gui::ParametersLayer& layer, const CoordinateConverter_ABC& converter, ActionController& controller )
    : ParamLocationList( parent, parameter, layer, converter, controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamPointList destructor
// Created: SBO 2007-03-15
// -----------------------------------------------------------------------------
ParamPointList::~ParamPointList()
{
    // NOTHING
}

namespace
{
    class AsnSerializer : public ParamVisitor_ABC
    {
    public:
        AsnSerializer( ASN1T_ListPoint& list )
            : list_( list )
            , index_( 0 )
        {}

        virtual void Visit( const Param_ABC& param )
        {
            if( index_ < list_.n )
                static_cast< const ParamLocation& >( param ).CommitTo( list_.elem[index_++] );
        }

    private:
        ASN1T_ListPoint& list_;
        unsigned int index_;
    };
}

// -----------------------------------------------------------------------------
// Name: ParamPointList::CommitTo
// Created: SBO 2007-03-15
// -----------------------------------------------------------------------------
void ParamPointList::CommitTo( ASN1T_MissionParameter& asn ) const
{
    ASN1T_ListPoint*& list = asn.value.u.listPoint = new ASN1T_ListPoint();
    asn.value.t = T_MissionParameter_value_listPoint;
    list->n = Count();
    asn.null_value = list->n ? 0 : 1;
    if( asn.null_value )
        return;
    list->elem = new ASN1T_Point[ list->n ];
    AsnSerializer serializer( *list );
    Accept( serializer );
}

// -----------------------------------------------------------------------------
// Name: ParamPointList::Clean
// Created: SBO 2007-03-15
// -----------------------------------------------------------------------------
void ParamPointList::Clean( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.listPoint )
    {
        for( unsigned int i = 0; i < asn.value.u.listPoint->n; ++i )
            delete[] asn.value.u.listPoint->elem[i].vecteur_point.elem;
        delete[] asn.value.u.listPoint->elem;
    }
    delete asn.value.u.listPoint;
}

// -----------------------------------------------------------------------------
// Name: ParamPointList::CreateElement
// Created: SBO 2007-04-27
// -----------------------------------------------------------------------------
Param_ABC* ParamPointList::CreateElement()
{
    Param_ABC* param = ParamLocationList::CreateElement();
    static_cast< ParamLocation* >( param )->SetShapeFilter( true, false, false, false );
    return param;
}
