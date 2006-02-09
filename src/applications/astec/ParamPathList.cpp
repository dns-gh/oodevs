// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-05-11 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ParamPathList.cpp $
// $Author: Ape $
// $Modtime: 23/09/04 17:35 $
// $Revision: 7 $
// $Workfile: ParamPathList.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "ParamPathList.h"
#include "moc_ParamPathList.cpp"

#include "App.h"
#include "World.h"
#include "ActionContext.h"

// -----------------------------------------------------------------------------
// Name: ParamPathList constructor
// Created: APE 2004-05-11
// -----------------------------------------------------------------------------
ParamPathList::ParamPathList( ASN1T_ListItineraire& asnPathList, const std::string strLabel, const std::string strMenuText, int nMinItems, int nMaxItems, QWidget* pParent, bool bOptional )
    : ParamListView( strLabel, true, pParent )
    , Param_ABC     ( bOptional )
    , strMenuText_      ( strMenuText )
    , asnPathList_      ( asnPathList )
    , pLocalisations_   ( 0 )
    , nMinItems_        ( nMinItems )
    , nMaxItems_        ( nMaxItems )
    , pLineEditor_      ( new ShapeEditorMapEventFilter( this ) )
{
    connect( pLineEditor_, SIGNAL( Done() ), this, SLOT( PathDone() ) );
}


// -----------------------------------------------------------------------------
// Name: ParamPathList destructor
// Created: APE 2004-05-11
// -----------------------------------------------------------------------------
ParamPathList::~ParamPathList()
{
    delete[] pLocalisations_;
    while( ! asnUMTCoordPtrList_.empty() )
    {
        delete[] asnUMTCoordPtrList_.back();
        asnUMTCoordPtrList_.pop_back();
    }
}


// -----------------------------------------------------------------------------
// Name: ParamPathList::FillRemotePopupMenu
// Created: APE 2004-05-11
// -----------------------------------------------------------------------------
void ParamPathList::FillRemotePopupMenu( QPopupMenu& popupMenu, const ActionContext& context )
{
    if( context.pPoint_ != 0 && ! context.selectedElement_.IsAMapElementSelected() )
        popupMenu.insertItem( strMenuText_.c_str(), this, SLOT( StartPath() ) );
}


// -----------------------------------------------------------------------------
// Name: ParamPathList::CheckValidity
// Created: APE 2004-05-11
// -----------------------------------------------------------------------------
bool ParamPathList::CheckValidity()
{
    if( this->childCount() >= nMinItems_ && this->childCount() <= nMaxItems_ )
        return true;

    this->TurnHeaderRed( 3000 );
    return false;
}


// -----------------------------------------------------------------------------
// Name: ParamPathList::WriteMsg
// Created: APE 2004-05-11
// -----------------------------------------------------------------------------
void ParamPathList::WriteMsg( std::stringstream& strMsg )
{
    strMsg << this->header()->label(0).latin1() << ": " << this->childCount() << " chemins.";
    
    uint nNbrChildren = this->childCount();
    asnPathList_.n    = nNbrChildren;
    if( nNbrChildren == 0 )
        return;

    delete[] pLocalisations_;
    pLocalisations_ = new ASN1T_Itineraire[ nNbrChildren ];
    asnPathList_.elem = pLocalisations_;

    while( ! asnUMTCoordPtrList_.empty() )
    {
        delete[] asnUMTCoordPtrList_.back();
        asnUMTCoordPtrList_.pop_back();
    }

    QListViewItem* pItem = this->firstChild();
    uint i = 0;
    while( pItem != 0 )
    {
        MT_ValuedListViewItem< T_PointVector >* pCastItem = (MT_ValuedListViewItem< T_PointVector >*)pItem;

        const T_PointVector& locPoints = pCastItem->GetValue();
        assert( !locPoints.empty() );

        pLocalisations_[i].type = EnumTypeLocalisation::line;
        pLocalisations_[i].vecteur_point.n    = locPoints.size();
        pLocalisations_[i].vecteur_point.elem = new ASN1T_CoordUTM[ locPoints.size() ];
        asnUMTCoordPtrList_.push_back( pLocalisations_[i].vecteur_point.elem );
        for( uint j = 0; j < locPoints.size(); ++j )
        {
            std::string strMGRS;
            App::GetApp().GetWorld().SimToMosMgrsCoord( locPoints[j], strMGRS );
            pLocalisations_[i].vecteur_point.elem[j] = strMGRS.c_str();
        }

        pItem = pItem->nextSibling();
        ++i;
    }
}


// -----------------------------------------------------------------------------
// Name: ParamPathList::StartPath
// Created: APE 2004-05-11
// -----------------------------------------------------------------------------
void ParamPathList::StartPath()
{
    // Push map event handler.
    pLineEditor_->PrepareNewLine( ShapeEditorMapEventFilter::ePath );
    pLineEditor_->Push();
}


// -----------------------------------------------------------------------------
// Name: ParamPathList::PathDone
// Created: APE 2004-05-11
// -----------------------------------------------------------------------------
void ParamPathList::PathDone()
{
    T_PointVector& pointList = pLineEditor_->GetPointList();
    if( pointList.empty() )
        return;

    MT_ValuedListViewItem< T_PointVector >* pItem = new MT_ValuedListViewItem< T_PointVector >( pointList, this, tr( "Itinéraire" ) );

    for( IT_PointVector it = pointList.begin(); it != pointList.end(); ++it )
    {
        std::string strPos;
        App::GetApp().GetWorld().SimToMosMgrsCoord( *it, strPos );
        new QListViewItem( pItem, strPos.c_str() );
    }
}
