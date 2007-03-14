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
ParamLocationList::ParamLocationList( QObject* parent, ASN1T_ListLocalisation*& asn, const QString& name, const QString& menu, ParametersLayer& layer, const CoordinateConverter_ABC& converter )
    : ParamListView( parent, name )
    , converter_( converter )
    , asn_( new ASN1T_ListLocalisation() )
    , pAsnLocalisationList_( 0 )
    , controller_( 0 )
{
    asn = asn_;
    creator_ = new LocationCreator( this, menu, layer, *this );
}

// -----------------------------------------------------------------------------
// Name: ParamLocationList constructor
// Created: AGE 2006-04-03
// -----------------------------------------------------------------------------
ParamLocationList::ParamLocationList( QObject* parent, ASN1T_ListPolygon*& asn, const QString& name, const QString& menu, ParametersLayer& layer, const CoordinateConverter_ABC& converter )
    : ParamListView( parent, name )
    , converter_( converter )
    , asn_( new ASN1T_ListLocalisation() )
    , pAsnLocalisationList_( 0 )
    , controller_( 0 )
{
    asn = (ASN1T_ListPolygon*&)asn_;
    creator_ = new LocationCreator( this, menu, layer, *this );
    creator_->Allow( false, false, true, false );
}

// -----------------------------------------------------------------------------
// Name: ParamLocationList constructor
// Created: AGE 2006-04-03
// -----------------------------------------------------------------------------
ParamLocationList::ParamLocationList( QObject* parent, ASN1T_ListPoint*& asn, const QString& name, const QString& menu, ParametersLayer& layer, const CoordinateConverter_ABC& converter )
    : ParamListView( parent, name )
    , converter_( converter )
    , asn_( new ASN1T_ListLocalisation() )
    , pAsnLocalisationList_( 0 )
    , controller_( 0 )
{
    asn = (ASN1T_ListPoint*&)asn_;
    creator_ = new LocationCreator( this, menu, layer, *this );
    creator_->Allow( true, false, false, false );
}

// -----------------------------------------------------------------------------
// Name: ParamLocationList destructor
// Created: AGE 2006-04-03
// -----------------------------------------------------------------------------
ParamLocationList::~ParamLocationList()
{
    delete asn_;
    delete[] pAsnLocalisationList_;
}

// -----------------------------------------------------------------------------
// Name: ParamLocationList::ClearSerializers
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
void ParamLocationList::ClearSerializers()
{
    while( ! serializers_.empty() )
    {
        delete serializers_.back();
        serializers_.pop_back();
    }
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
// Name: ParamLocationList::Commit
// Created: AGE 2006-04-03
// -----------------------------------------------------------------------------
void ParamLocationList::Commit()
{
    if( !ListView() )
        InterfaceNotInitialized();
    const unsigned nNbrChilds = locations_.size();
    asn_->n = nNbrChilds;

    if( nNbrChilds == 0 && IsOptional() )
        return;

    delete[] pAsnLocalisationList_;
    pAsnLocalisationList_ = new ASN1T_Localisation[ nNbrChilds ];
    asn_->elem = pAsnLocalisationList_;

    ClearSerializers();
    for( unsigned i = 0; i < locations_.size(); ++i )
    {
        serializers_.push_back( new LocationSerializer( converter_, asn_->elem[i] ) );
        serializers_.back()->Serialize( *locations_.at( i ) );
    }
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
    delete[] pAsnLocalisationList_;
    ClearSerializers();
    ParamListView::OnClearList();
}
