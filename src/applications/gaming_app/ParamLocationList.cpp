// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamLocationList.h"
#include "ParamLocation.h"
#include "ParamVisitor_ABC.h"
#include "clients_kernel/OrderParameter.h"
#include "gaming/Action_ABC.h"
#include "gaming/ActionParameterLocation.h"
#include "gaming/ActionParameterLocationList.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ParamLocationList constructor
// Created: AGE 2006-04-03
// -----------------------------------------------------------------------------
ParamLocationList::ParamLocationList( QObject* parent, const OrderParameter& parameter, ParametersLayer& layer, const CoordinateConverter_ABC& converter, ActionController& controller )
    : ListParameter( parent, parameter.GetName(), controller, parameter.IsOptional() )
    , converter_( converter )
    , parameter_( parameter )
    , layer_( layer )
    , count_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamLocationList destructor
// Created: AGE 2006-04-03
// -----------------------------------------------------------------------------
ParamLocationList::~ParamLocationList()
{
    // NOTHING
}

namespace
{
    class AsnSerializer : public ParamVisitor_ABC
    {
    public:
        AsnSerializer( ASN1T_ListLocalisation& list )
            : list_( list )
            , index_( 0 )
        {}

        virtual void Visit( const Param_ABC& param )
        {
            if( index_ < list_.n )
                static_cast< const ParamLocation& >( param ).CommitTo( list_.elem[index_++] );
        }

    private:
        ASN1T_ListLocalisation& list_;
        unsigned int index_;
    };
}

// -----------------------------------------------------------------------------
// Name: ParamLocationList::CommitTo
// Created: SBO 2007-03-15
// -----------------------------------------------------------------------------
void ParamLocationList::CommitTo( ASN1T_MissionParameter& asn ) const
{
    ASN1T_ListLocalisation*& list = asn.value.u.listLocalisation = new ASN1T_ListLocalisation();
    asn.value.t = T_MissionParameter_value_listLocalisation;
    list->n = Count();
    asn.null_value = list->n ? 0 : 1;
    if( asn.null_value )
        return;
    list->elem = new ASN1T_Localisation[ list->n ];
    AsnSerializer serializer( *list );
    Accept( serializer );
}

// -----------------------------------------------------------------------------
// Name: ParamLocationList::Clean
// Created: SBO 2007-03-15
// -----------------------------------------------------------------------------
void ParamLocationList::Clean( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.listLocalisation )
    {
        for( unsigned int i = 0; i < asn.value.u.listLocalisation->n; ++i )
            delete[] asn.value.u.listLocalisation->elem[i].vecteur_point.elem;
        delete[] asn.value.u.listLocalisation->elem;
    }
    delete asn.value.u.listLocalisation;
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
// Name: ParamLocationList::CommitTo
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
void ParamLocationList::CommitTo( Action_ABC& action ) const
{
    std::auto_ptr< ActionParameter_ABC > param( new ActionParameterLocationList( parameter_ ) );
    ActionSerializer serializer( *param );
    Accept( serializer );
    action.AddParameter( *param.release() );
}

// -----------------------------------------------------------------------------
// Name: ParamLocationList::CreateElement
// Created: SBO 2007-04-27
// -----------------------------------------------------------------------------
Param_ABC* ParamLocationList::CreateElement()
{
    return new ParamLocation( tr( "Location %1" ).arg( ++count_ ), layer_, converter_, false );
}
