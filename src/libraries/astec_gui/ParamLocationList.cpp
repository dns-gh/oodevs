// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_gui_pch.h"
#include "ParamLocationList.h"
#include "LocationCreator.h"
#include "astec_kernel/CoordinateConverter_ABC.h"
#include "astec_kernel/ActionController.h"
#include "Tools.h"

// -----------------------------------------------------------------------------
// Name: ParamLocationList constructor
// Created: AGE 2006-04-03
// -----------------------------------------------------------------------------
ParamLocationList::ParamLocationList( QWidget* pParent, ASN1T_ListLocalisation& asn, const std::string label, const std::string menu, ParametersLayer& layer, const CoordinateConverter_ABC& converter )
    : ParamListView( pParent, label.c_str() )
    , converter_( converter )
    , asn_( asn )
    , pAsnLocalisationList_( 0 )
    , controller_( 0 )
{
    creator_ = new LocationCreator( this, menu, layer, *this );
    creator_->AddLocationType( tr( "point" ), EnumTypeLocalisation::point );
    creator_->AddLocationType( tr( "ligne" ), EnumTypeLocalisation::line );
    creator_->AddLocationType( tr( "polygone" ), EnumTypeLocalisation::polygon );
    creator_->AddLocationType( tr( "cercle" ), EnumTypeLocalisation::circle );
}

// -----------------------------------------------------------------------------
// Name: ParamLocationList constructor
// Created: AGE 2006-04-03
// -----------------------------------------------------------------------------
ParamLocationList::ParamLocationList( QWidget* pParent, ASN1T_ListPolygon& asn, const std::string label, const std::string menu, ParametersLayer& layer, const CoordinateConverter_ABC& converter )
    : ParamListView( pParent, label.c_str() )
    , converter_( converter )
    , asn_( (ASN1T_ListLocalisation&)asn )
    , pAsnLocalisationList_( 0 )
{
    creator_ = new LocationCreator( this, menu, layer, *this );
    creator_->AddLocationType( tr( "polygone" ), EnumTypeLocalisation::polygon );
}

// -----------------------------------------------------------------------------
// Name: ParamLocationList constructor
// Created: AGE 2006-04-03
// -----------------------------------------------------------------------------
ParamLocationList::ParamLocationList( QWidget* pParent, ASN1T_ListPoint& asn, const std::string label, const std::string menu, ParametersLayer& layer, const CoordinateConverter_ABC& converter )
    : ParamListView( pParent, label.c_str() )
    , converter_( converter )
    , asn_( (ASN1T_ListLocalisation&)asn )
    , pAsnLocalisationList_( 0 )
{
    creator_ = new LocationCreator( this, menu, layer, *this );
    creator_->AddLocationType( tr( "point" ), EnumTypeLocalisation::point );
}

// -----------------------------------------------------------------------------
// Name: ParamLocationList destructor
// Created: AGE 2006-04-03
// -----------------------------------------------------------------------------
ParamLocationList::~ParamLocationList()
{
    delete[] pAsnLocalisationList_;
    while( ! asnUMTCoordPtrList_.empty() )
    {
        delete[] asnUMTCoordPtrList_.back();
        asnUMTCoordPtrList_.pop_back();
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
    if( points_.empty() && ! IsOptional() )
        return Invalid();
    return true;
}

// -----------------------------------------------------------------------------
// Name: ParamLocationList::Commit
// Created: AGE 2006-04-03
// -----------------------------------------------------------------------------
void ParamLocationList::Commit()
{
    const unsigned nNbrChilds = points_.size();
    asn_.n = nNbrChilds;

    if( nNbrChilds == 0 && IsOptional() )
        return;

    delete[] pAsnLocalisationList_;
    pAsnLocalisationList_ = new ASN1T_Localisation[ nNbrChilds ];
    asn_.elem = pAsnLocalisationList_;

    while( ! asnUMTCoordPtrList_.empty() )
    {
        delete[] asnUMTCoordPtrList_.back();
        asnUMTCoordPtrList_.pop_back();
    }

    for( unsigned i = 0; i < points_.size(); ++i )
    {
        const T_PointVector& points = points_.at( i );
        asn_.elem[i].type               = types_.at( i );
        asn_.elem[i].vecteur_point.n    = points.size();
        asn_.elem[i].vecteur_point.elem = new ASN1T_CoordUTM[ points.size() ];
        for( unsigned j = 0; j < points.size(); ++j )
        {
            const std::string coord = converter_.ConvertToMgrs( points.at( j ) );
            asn_.elem[i].vecteur_point.elem[j] = coord.c_str();
        }
        asnUMTCoordPtrList_.push_back( pAsnLocalisationList_[i].vecteur_point.elem );
    }
}

// -----------------------------------------------------------------------------
// Name: ParamLocationList::Handle
// Created: AGE 2006-04-03
// -----------------------------------------------------------------------------
void ParamLocationList::Handle( const T_PointVector& points )
{
    points_.push_back( points );
    types_.push_back( creator_->GetCurrentType() );
    new QListViewItem( this, Tools::ToString(  creator_->GetCurrentType() ) );
}

// -----------------------------------------------------------------------------
// Name: ParamLocationList::OnDeleteSelectedItem
// Created: AGE 2006-04-03
// -----------------------------------------------------------------------------
void ParamLocationList::OnDeleteSelectedItem()
{
    QListViewItem* selected = selectedItem();
    unsigned index = 0;
    QListViewItem* item = firstChild();
    while( item && item != selected )
    {
        ++index;
        item = item->nextSibling();
    }
    if( item == selected && index < points_.size() )
    {
        points_.erase( points_.begin() + index );
        types_.erase( types_.begin() + index );
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
    while( ! asnUMTCoordPtrList_.empty() )
    {
        delete[] asnUMTCoordPtrList_.back();
        asnUMTCoordPtrList_.pop_back();
    }
    ParamListView::OnClearList();
}
