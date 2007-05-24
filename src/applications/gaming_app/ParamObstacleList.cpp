//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamObstacleList.h"
#include "ParamObstacle.h"
#include "ParamVisitor_ABC.h"
#include "clients_gui/ValuedListItem.h"
#include "clients_kernel/OrderParameter.h"
#include "gaming/Action_ABC.h"
#include "gaming/ActionParameterObstacleList.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ParamObstacleList constructor
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
ParamObstacleList::ParamObstacleList( QObject* parent, const OrderParameter& parameter, const ObjectTypes& objectTypes, ParametersLayer& layer, const CoordinateConverter_ABC& converter, ActionController& controller )
    : ListParameter( parent, parameter.GetName(), controller, parameter.IsOptional() )
    , parameter_( parameter )
    , converter_( converter )
    , objectTypes_( objectTypes )
    , layer_( layer )
    , count_( 0 )
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: ParamObstacleList destructor
// Created: SBO 2006-06-28
// -----------------------------------------------------------------------------
ParamObstacleList::~ParamObstacleList()
{
    // NOTHING
}

namespace
{
    class AsnSerializer : public ParamVisitor_ABC
    {
    public:
        AsnSerializer( ASN1T_ListMissionGenObject& list )
            : list_( list )
            , index_( 0 )
        {}

        virtual void Visit( const Param_ABC& param )
        {
            if( index_ < list_.n )
                static_cast< const ParamObstacle& >( param ).CommitTo( list_.elem[index_++] );
        }

    private:
        ASN1T_ListMissionGenObject& list_;
        unsigned int index_;
    };
}

// -----------------------------------------------------------------------------
// Name: ParamObstacleList::CommitTo
// Created: SBO 2007-03-15
// -----------------------------------------------------------------------------
void ParamObstacleList::CommitTo( ASN1T_MissionParameter& asn ) const
{
    ASN1T_ListMissionGenObject*& list = asn.value.u.listMissionGenObject = new ASN1T_ListMissionGenObject();
    asn.value.t = T_MissionParameter_value_listMissionGenObject;
    list->n = Count();
    asn.null_value = list->n ? 0 : 1;
    if( asn.null_value )
        return;
    list->elem = new ASN1T_MissionGenObject[ list->n ];
    AsnSerializer serializer( *list );
    Accept( serializer );
}

// -----------------------------------------------------------------------------
// Name: ParamObstacleList::Clean
// Created: SBO 2007-03-15
// -----------------------------------------------------------------------------
void ParamObstacleList::Clean( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.listMissionGenObject )
        delete[] asn.value.u.listMissionGenObject->elem;
    delete asn.value.u.listMissionGenObject;
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
            static_cast< const ParamObstacle& >( param ).CommitTo( parent_ );
        }

    private:
        ActionParameter_ABC& parent_;
    };
}

// -----------------------------------------------------------------------------
// Name: ParamObstacleList::CommitTo
// Created: SBO 2007-04-16
// -----------------------------------------------------------------------------
void ParamObstacleList::CommitTo( Action_ABC& action ) const
{
    std::auto_ptr< ActionParameter_ABC > param( new ActionParameterObstacleList( parameter_ ) );
    ActionSerializer serializer( *param );
    Accept( serializer );
    action.AddParameter( *param.release() );
}

// -----------------------------------------------------------------------------
// Name: ParamObstacleList::CreateElement
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
Param_ABC* ParamObstacleList::CreateElement()
{
    return new ParamObstacle( this, tr( "Obstacle %1" ).arg( ++count_ ), objectTypes_, layer_, converter_, false );
}
