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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ParamPath.cpp $
// $Author: Ape $
// $Modtime: 23/09/04 17:35 $
// $Revision: 8 $
// $Workfile: MOS_ParamPath.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MOS_ParamPath.h"
#include "moc_MOS_ParamPath.cpp"

#include "MOS_App.h"
#include "MOS_World.h"
#include "MOS_ActionContext.h"
#include "MOS_ShapeEditorMapEventFilter.h"
#include "MOS_Agent_ABC.h"

// -----------------------------------------------------------------------------
// Name: MOS_ParamPath constructor
/** @param  asnListPoint 
    @param  strLabel 
    @param  strMenuText 
    @param  pParent 
*/
// Created: APE 2004-04-13
// -----------------------------------------------------------------------------
MOS_ParamPath::MOS_ParamPath( ASN1T_Itineraire& asnListPoint, MOS_Agent_ABC& agent, const std::string strLabel, const std::string strMenuText, QWidget* pParent)
    : MOS_ParamListView ( strLabel, false, pParent )
    , MOS_Param_ABC     ()
    , strMenuText_      ( strMenuText )
    , asnListPoint_     ( asnListPoint )
    , pUMTCoords_       ( 0 )
    , agent_            ( agent )
    , pLineEditor_      ( new MOS_ShapeEditorMapEventFilter( this ) )
{
    connect( pLineEditor_, SIGNAL( Done() ), this, SLOT( PathDone() ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamPath destructor
// Created: APE 2004-04-13
// -----------------------------------------------------------------------------
MOS_ParamPath::~MOS_ParamPath()
{
    delete pUMTCoords_;
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamPath::FillRemotePopupMenu
/** @param  popupMenu 
    @param  pAgent 
    @param  pPoint 
*/
// Created: APE 2004-04-13
// -----------------------------------------------------------------------------
void MOS_ParamPath::FillRemotePopupMenu( QPopupMenu& popupMenu, const MOS_ActionContext& context )
{
    if( context.pPoint_ != 0 && ! context.selectedElement_.IsAMapElementSelected() )
        popupMenu.insertItem( strMenuText_.c_str(), this, SLOT( StartPath() ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamPath::CheckValidity
/** @return 
*/
// Created: APE 2004-04-13
// -----------------------------------------------------------------------------
bool MOS_ParamPath::CheckValidity()
{
    if( ! pointList_.empty() )
        return true;

    // If the pointlist is empty, flash the header red.
    this->TurnHeaderRed( 3000 );
    return false;
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamPath::WriteMsg
/** @param  sParam 
*/
// Created: APE 2004-04-13
// -----------------------------------------------------------------------------
void MOS_ParamPath::WriteMsg( std::stringstream& strMsg )
{
    strMsg << this->header()->label(0).latin1() << ": ";

    assert( pointList_.size() >= 1 );
    uint nNbrPoints = pointList_.size();
    strMsg << nNbrPoints << " points. [";

    asnListPoint_.type               = EnumTypeLocalisation::line;
    asnListPoint_.vecteur_point.n    = nNbrPoints;

    delete[] pUMTCoords_;
    pUMTCoords_ = new ASN1T_CoordUTM[ nNbrPoints ];
    asnListPoint_.vecteur_point.elem = pUMTCoords_;

    for( uint i = 0; i < nNbrPoints; ++i )
    {
        std::string strValue;
        MOS_App::GetApp().GetWorld().SimToMosMgrsCoord( pointList_[i], strValue );
        assert( strValue.size() == 15 );
        asnListPoint_.vecteur_point.elem[i] = strValue.c_str();
        strMsg << strValue << (i < nNbrPoints-1 ? ", " : "");
    }
    strMsg << "]";
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamPath::StartPath
// Created: APE 2004-04-13
// -----------------------------------------------------------------------------
void MOS_ParamPath::StartPath()
{
    this->clear();
    pointList_.clear();
    
    // Push map event handler.
    pLineEditor_->PrepareNewLine( MOS_ShapeEditorMapEventFilter::ePath );
    pLineEditor_->Push();
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamPath::PathDone
// Created: APE 2004-04-22
// -----------------------------------------------------------------------------
void MOS_ParamPath::PathDone()
{
    T_PointVector& pointList = pLineEditor_->GetPointList();
    if( pointList.empty() )
        return;

    pointList_ = pointList;
    for( IT_PointVector it = pointList_.begin(); it != pointList_.end(); ++it )
    {
        std::string strPos;
        MOS_App::GetApp().GetWorld().SimToMosMgrsCoord( *it, strPos );
        QListViewItem* pItem = new QListViewItem( this, strPos.c_str() );
        pItem->moveItem( this->lastItem() );
        this->ensureItemVisible( this->lastItem() );
    }
}
