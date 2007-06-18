// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamAutomatList.h"
#include "ParamAutomat.h"
#include "ParamVisitor_ABC.h"
#include "gaming/ActionParameterAutomatList.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/OrderParameter.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ParamAutomatList constructor
// Created: AGE 2006-11-29
// -----------------------------------------------------------------------------
ParamAutomatList::ParamAutomatList( QObject* parent, const OrderParameter& parameter, ActionController& controller )
    : EntityListParameter< Automat_ABC >( parent, parameter, controller )
    , parameter_( parameter )
    , count_( 0 )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: ParamAutomatList destructor
// Created: AGE 2006-11-29
// -----------------------------------------------------------------------------
ParamAutomatList::~ParamAutomatList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamAutomatList::AddToMenu
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
void ParamAutomatList::AddToMenu( kernel::ContextMenu& menu )
{
    MakeMenu( tr( "Add automat" ), menu );
}

// -----------------------------------------------------------------------------
// Name: ParamAutomatList::CreateElement
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
EntityParameter< kernel::Automat_ABC >* ParamAutomatList::CreateElement( const kernel::Automat_ABC& potential )
{
    return new ParamAutomat( this, OrderParameter( tr( "Automat %1" ).arg( ++count_ ), "automat", false ), potential );
}

namespace
{
    struct AsnSerializer : public ParamVisitor_ABC
    {
        explicit AsnSerializer( ASN1T_AutomatList& list ) : list_( list ), index_( 0 ) {}
        virtual void Visit( const Param_ABC& param )
        {
            if( index_ < list_.n )
                static_cast< const ParamAutomat& >( param ).CommitTo( list_.elem[index_++] );
        }

        ASN1T_AutomatList& list_;
        unsigned int index_;
    };
}

// -----------------------------------------------------------------------------
// Name: ParamAutomatList::CommitTo
// Created: SBO 2007-03-14
// -----------------------------------------------------------------------------
void ParamAutomatList::CommitTo( ASN1T_MissionParameter& asn ) const
{
    ASN1T_AutomatList*& list = asn.value.u.automatList = new ASN1T_AutomatList();
    asn.value.t = T_MissionParameter_value_automatList;
    list->n = Count();
    asn.null_value = list->n ? 0 : 1;
    if( asn.null_value )
        return;
    list->elem = new ASN1T_Automat[ list->n ];
    AsnSerializer serializer( *list );
    Accept( serializer );
}

// -----------------------------------------------------------------------------
// Name: ParamAutomatList::Clean
// Created: SBO 2007-03-14
// -----------------------------------------------------------------------------
void ParamAutomatList::Clean( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.automatList )
        delete[] asn.value.u.automatList->elem;
    delete asn.value.u.automatList;
}

namespace
{
    struct ActionSerializer : public ParamVisitor_ABC
    {
        explicit ActionSerializer( ActionParameter_ABC& parent ) : parent_( parent ) {}
        virtual void Visit( const Param_ABC& param )
        {
            static_cast< const ParamAutomat& >( param ).CommitTo( parent_ );
        }

        ActionParameter_ABC& parent_;
    };
}

// -----------------------------------------------------------------------------
// Name: ParamAutomatList::CommitTo
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
void ParamAutomatList::CommitTo( Action_ABC& action ) const
{
    std::auto_ptr< ActionParameter_ABC > param( new ActionParameterAutomatList( parameter_ ) );
    ActionSerializer serializer( *param );
    Accept( serializer );
    action.AddParameter( *param.release() );
}
