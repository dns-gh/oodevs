// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-03-25 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ParamPath.cpp $
// $Author: Ape $
// $Modtime: 23/09/04 17:35 $
// $Revision: 8 $
// $Workfile: ParamPath.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "ParamPath.h"
#include "moc_ParamPath.cpp"

#include "App.h"
#include "World.h"
#include "ActionContext.h"
#include "ShapeEditorMapEventFilter.h"
#include "Agent_ABC.h"

// -----------------------------------------------------------------------------
// Name: ParamPath constructor
// Created: APE 2004-04-13
// -----------------------------------------------------------------------------
ParamPath::ParamPath( ASN1T_Itineraire& asnListPoint, Agent_ABC& agent, const std::string strLabel, const std::string strMenuText, QWidget* pParent, OptionalParamFunctor_ABC* pOptional )
    : ParamListView ( strLabel, false, pParent )
    , Param_ABC     ( pOptional )
    , strMenuText_      ( strMenuText )
    , asnListPoint_     ( asnListPoint )
    , pUMTCoords_       ( 0 )
    , agent_            ( agent )
    , pLineEditor_      ( new ShapeEditorMapEventFilter( this ) )
{
    connect( pLineEditor_, SIGNAL( Done() ), this, SLOT( PathDone() ) );
}


// -----------------------------------------------------------------------------
// Name: ParamPath destructor
// Created: APE 2004-04-13
// -----------------------------------------------------------------------------
ParamPath::~ParamPath()
{
    delete pUMTCoords_;
}


// -----------------------------------------------------------------------------
// Name: ParamPath::FillRemotePopupMenu
// Created: APE 2004-04-13
// -----------------------------------------------------------------------------
void ParamPath::FillRemotePopupMenu( QPopupMenu& popupMenu, const ActionContext& context )
{
    if( context.pPoint_ != 0 && ! context.selectedElement_.IsAMapElementSelected() )
        popupMenu.insertItem( strMenuText_.c_str(), this, SLOT( StartPath() ) );
}


// -----------------------------------------------------------------------------
// Name: ParamPath::CheckValidity
// Created: APE 2004-04-13
// -----------------------------------------------------------------------------
bool ParamPath::CheckValidity()
{
    if( ! pointList_.empty() || IsOptional() )
        return true;

    // If the pointlist is empty, flash the header red.
    this->TurnHeaderRed( 3000 );
    return false;
}


// -----------------------------------------------------------------------------
// Name: ParamPath::WriteMsg
// Created: APE 2004-04-13
// -----------------------------------------------------------------------------
void ParamPath::WriteMsg( std::stringstream& strMsg )
{
    strMsg << this->header()->label(0).latin1() << ": ";

    assert( pointList_.size() >= 1 );
    uint nNbrPoints = pointList_.size();
    strMsg << nNbrPoints << " points. [";

    if( pOptional_ )
        pOptional_->SetOptionalPresent();

    asnListPoint_.type               = EnumTypeLocalisation::line;
    asnListPoint_.vecteur_point.n    = nNbrPoints;

    delete[] pUMTCoords_;
    pUMTCoords_ = new ASN1T_CoordUTM[ nNbrPoints ];
    asnListPoint_.vecteur_point.elem = pUMTCoords_;

    for( uint i = 0; i < nNbrPoints; ++i )
    {
        std::string strValue;
        App::GetApp().GetWorld().SimToMosMgrsCoord( pointList_[i], strValue );
        assert( strValue.size() == 15 );
        asnListPoint_.vecteur_point.elem[i] = strValue.c_str();
        strMsg << strValue << (i < nNbrPoints-1 ? ", " : "");
    }
    strMsg << "]";
}


// -----------------------------------------------------------------------------
// Name: ParamPath::StartPath
// Created: APE 2004-04-13
// -----------------------------------------------------------------------------
void ParamPath::StartPath()
{
    this->clear();
    pointList_.clear();
    
    // Push map event handler.
    pLineEditor_->PrepareNewLine( ShapeEditorMapEventFilter::ePath );
    pLineEditor_->Push();
}


// -----------------------------------------------------------------------------
// Name: ParamPath::PathDone
// Created: APE 2004-04-22
// -----------------------------------------------------------------------------
void ParamPath::PathDone()
{
    T_PointVector& pointList = pLineEditor_->GetPointList();
    if( pointList.empty() )
        return;

    pointList_ = pointList;
    for( IT_PointVector it = pointList_.begin(); it != pointList_.end(); ++it )
    {
        std::string strPos;
        App::GetApp().GetWorld().SimToMosMgrsCoord( *it, strPos );
        QListViewItem* pItem = new QListViewItem( this, strPos.c_str() );
        pItem->moveItem( this->lastItem() );
        this->ensureItemVisible( this->lastItem() );
    }
}
