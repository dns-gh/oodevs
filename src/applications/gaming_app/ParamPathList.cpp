// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamPathList.h"
#include "ParamPath.h"
#include "ParamVisitor_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/OrderParameter.h"
#include "gaming/Action_ABC.h"
#include "gaming/ActionParameterPathList.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ParamPathList constructor
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
ParamPathList::ParamPathList( QObject* parent, const OrderParameter& parameter, ParametersLayer& layer, const CoordinateConverter_ABC& converter, const Entity_ABC& entity, ActionController& controller )
    : ListParameter( parent, parameter.GetName(), controller, parameter.IsOptional() )
    , converter_( converter )
    , parameter_( parameter )
    , layer_( layer )
    , entity_( entity )
    , count_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamPathList destructor
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
ParamPathList::~ParamPathList()
{
    // NOTHING
}

namespace
{
    class AsnSerializer : public ParamVisitor_ABC
    {
    public:
        AsnSerializer( ASN1T_ListItineraire& list )
            : list_( list )
            , index_( 0 )
        {}

        virtual void Visit( const Param_ABC& param )
        {
            if( index_ < list_.n )
                static_cast< const ParamPath& >( param ).CommitTo( list_.elem[index_++] );
        }

    private:
        ASN1T_ListItineraire& list_;
        unsigned int index_;
    };
}

// -----------------------------------------------------------------------------
// Name: ParamPathList::CommitTo
// Created: SBO 2007-03-15
// -----------------------------------------------------------------------------
void ParamPathList::CommitTo( ASN1T_MissionParameter& asn ) const
{
    ASN1T_ListItineraire*& list = asn.value.u.listItineraire = new ASN1T_ListItineraire();
    asn.value.t = T_MissionParameter_value_listItineraire;
    list->n = Count();
    asn.null_value = list->n ? 0 : 1;
    if( asn.null_value )
        return;
    list->elem = new ASN1T_Itineraire[ list->n ];
    AsnSerializer serializer( *list );
    Accept( serializer );
}

// -----------------------------------------------------------------------------
// Name: ParamPathList::Clean
// Created: SBO 2007-03-15
// -----------------------------------------------------------------------------
void ParamPathList::Clean( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.listItineraire )
    {
        for( unsigned int i = 0; i < asn.value.u.listItineraire->n; ++i )
            delete[] asn.value.u.listItineraire->elem[i].vecteur_point.elem;
        delete[] asn.value.u.listItineraire->elem;
    }
    delete asn.value.u.listItineraire;
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
            static_cast< const ParamPath& >( param ).CommitTo( parent_ );
        }

    private:
        ActionParameter_ABC& parent_;
    };
}

// -----------------------------------------------------------------------------
// Name: ParamPathList::CommitTo
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
void ParamPathList::CommitTo( Action_ABC& action ) const
{
    std::auto_ptr< ActionParameter_ABC > param( new ActionParameterPathList( parameter_ ) );
    ActionSerializer serializer( *param );
    Accept( serializer );
    action.AddParameter( *param.release() );
}

// -----------------------------------------------------------------------------
// Name: ParamPathList::CreateElement
// Created: SBO 2007-04-27
// -----------------------------------------------------------------------------
Param_ABC* ParamPathList::CreateElement()
{
    return new ParamPath( this, tr( "Route %1" ).arg( ++count_ ), layer_, converter_, entity_, false );
}
