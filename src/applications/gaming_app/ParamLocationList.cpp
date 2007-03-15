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
#include "clients_gui/LocationCreator.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/ActionController.h"
#include "clients_gui/Tools.h"
#include "clients_kernel/Location_ABC.h"
#include "LocationSerializer.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ParamLocationList constructor
// Created: AGE 2006-04-03
// -----------------------------------------------------------------------------
ParamLocationList::ParamLocationList( QObject* parent, const QString& name, ParametersLayer& layer, const CoordinateConverter_ABC& converter )
    : ParamListView( parent, name )
    , converter_( converter )
    , creator_( new LocationCreator( this, name, layer, *this ) )
    , controller_( 0 )
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

// -----------------------------------------------------------------------------
// Name: ParamLocationList::SetShapeFilter
// Created: SBO 2007-03-15
// -----------------------------------------------------------------------------
void ParamLocationList::SetShapeFilter( bool point, bool line, bool polygon, bool circle )
{
    creator_->Allow( point, line, polygon, circle );
}

// -----------------------------------------------------------------------------
// Name: ParamLocationList::RemoveFromController
// Created: AGE 2006-04-03
// -----------------------------------------------------------------------------
void ParamLocationList::RemoveFromController()
{
    if( controller_ )
        controller_->Remove( *creator_ );
    Param_ABC::RemoveFromController();
}

// -----------------------------------------------------------------------------
// Name: ParamLocationList::RegisterIn
// Created: AGE 2006-04-03
// -----------------------------------------------------------------------------
void ParamLocationList::RegisterIn( ActionController& controller )
{
    controller_ = & controller;
    controller_->Register( *creator_ );
    Param_ABC::RegisterIn( controller );
}

// -----------------------------------------------------------------------------
// Name: ParamLocationList::CheckValidity
// Created: AGE 2006-04-03
// -----------------------------------------------------------------------------
bool ParamLocationList::CheckValidity()
{
    if( locations_.empty() && ! IsOptional() )
        return Invalid();
    return true;
}

// -----------------------------------------------------------------------------
// Name: ParamLocationList::CommitTo
// Created: SBO 2007-03-15
// -----------------------------------------------------------------------------
void ParamLocationList::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.value.t = T_MissionParameter_value_listLocalisation;
    CommitTo( asn.value.u.listLocalisation );
    asn.null_value = asn.value.u.listLocalisation->n ? 0 : 1;
}

// -----------------------------------------------------------------------------
// Name: ParamLocationList::Clean
// Created: SBO 2007-03-15
// -----------------------------------------------------------------------------
void ParamLocationList::Clean( ASN1T_MissionParameter& asn ) const
{
    Clean( asn.value.u.listLocalisation );
}

// -----------------------------------------------------------------------------
// Name: ParamLocationList::CommitTo
// Created: SBO 2007-03-15
// -----------------------------------------------------------------------------
void ParamLocationList::CommitTo( ASN1T_ListLocalisation*& asn ) const
{
    if( !ListView() )
        InterfaceNotInitialized();

    asn = new ASN1T_ListLocalisation();
    asn->n = locations_.size();
    if( asn->n == 0 && IsOptional() )
        return;

    asn->elem = new ASN1T_Localisation[ asn->n ];
    LocationSerializer serializer( converter_ );
    for( unsigned i = 0; i < locations_.size(); ++i )
        serializer.Serialize( *locations_.at( i ), asn->elem[i] );
}

// -----------------------------------------------------------------------------
// Name: ParamLocationList::Clean
// Created: SBO 2007-03-15
// -----------------------------------------------------------------------------
void ParamLocationList::Clean( ASN1T_ListLocalisation*& asn ) const
{
    // $$$$ SBO 2007-03-15: clean points
    delete[] asn->elem;
    delete asn;
}

// -----------------------------------------------------------------------------
// Name: ParamLocationList::Handle
// Created: AGE 2006-04-03
// -----------------------------------------------------------------------------
void ParamLocationList::Handle( Location_ABC& location )
{
    locations_.push_back( &location );
    new QListViewItem( ListView(), location.GetName() );
}

// -----------------------------------------------------------------------------
// Name: ParamLocationList::OnDeleteSelectedItem
// Created: AGE 2006-04-03
// -----------------------------------------------------------------------------
void ParamLocationList::OnDeleteSelectedItem()
{
    QListViewItem* selected = ListView()->selectedItem();
    unsigned index = 0;
    QListViewItem* item = ListView()->firstChild();
    while( item && item != selected )
    {
        ++index;
        item = item->nextSibling();
    }
    if( item == selected && index < locations_.size() )
    {
        delete *(locations_.begin() + index);
        locations_.erase( locations_.begin() + index );
    }
    ParamListView::OnDeleteSelectedItem();
}

// -----------------------------------------------------------------------------
// Name: ParamLocationList::OnClearList
// Created: AGE 2006-04-03
// -----------------------------------------------------------------------------
void ParamLocationList::OnClearList()
{
    ParamListView::OnClearList();
}
