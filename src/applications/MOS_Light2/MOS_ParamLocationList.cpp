// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-05-07 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ParamLocationList.cpp $
// $Author: Age $
// $Modtime: 30/03/05 18:49 $
// $Revision: 9 $
// $Workfile: MOS_ParamLocationList.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MOS_ParamLocationList.h"
#include "moc_MOS_ParamLocationList.cpp"

#include "MOS_App.h"
#include "MOS_World.h"
#include "MOS_ActionContext.h"
#include "MOS_Tools.h"
#include "MOS_ShapeEditorMapEventFilter.h"

// -----------------------------------------------------------------------------
// Name: MOS_ParamLocationList constructor
// Created: APE 2004-05-07
// -----------------------------------------------------------------------------
MOS_ParamLocationList::MOS_ParamLocationList( ASN1T_ListLocalisation& asnListLoc, const std::string strLabel, const std::string strMenuText, QWidget* pParent, bool bOptional )
    : MOS_ParamListView     ( strLabel, true, pParent )
    , MOS_Param_ABC         ( bOptional )
    , strMenuText_          ( strMenuText )
    , asnListLoc_           ( asnListLoc )
    , pAsnLocalisationList_ ( 0 )
    , pLineEditor_          ( new MOS_ShapeEditorMapEventFilter( this ) )
{
    pPopupMenu_ = new QPopupMenu( this );
    int n;
    n = pPopupMenu_->insertItem( MOS_Tools::ToString( EnumTypeLocalisation::point ), this, SLOT( StartTracing( int ) ) );
    pPopupMenu_->setItemParameter( n, (int)EnumTypeLocalisation::point );
    n = pPopupMenu_->insertItem( MOS_Tools::ToString( EnumTypeLocalisation::line ), this, SLOT( StartTracing( int ) ) );
    pPopupMenu_->setItemParameter( n, (int)EnumTypeLocalisation::line );
    n = pPopupMenu_->insertItem( MOS_Tools::ToString( EnumTypeLocalisation::polygon ), this, SLOT( StartTracing( int ) ) );
    pPopupMenu_->setItemParameter( n, (int)EnumTypeLocalisation::polygon );

    connect( pLineEditor_, SIGNAL( Done() ), this, SLOT( TracingDone() ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamLocationList constructor
// Created: APE 2004-05-07
// -----------------------------------------------------------------------------
MOS_ParamLocationList::MOS_ParamLocationList( ASN1T_ListPolygon& asnListPoly, const std::string strLabel, const std::string strMenuText, QWidget* pParent, bool bOptional )
    : MOS_ParamListView ( strLabel, true, pParent )
    , MOS_Param_ABC     ( bOptional )
    , strMenuText_      ( strMenuText )
    , asnListLoc_       ( (ASN1T_ListLocalisation&)asnListPoly )
    , pAsnLocalisationList_ ( 0 )
    , pLineEditor_      ( new MOS_ShapeEditorMapEventFilter( this ) )
{
    pPopupMenu_ = new QPopupMenu( this );
    int n;
    n = pPopupMenu_->insertItem( MOS_Tools::ToString( EnumTypeLocalisation::polygon ), this, SLOT( StartTracing( int ) ) );
    pPopupMenu_->setItemParameter( n, (int)EnumTypeLocalisation::polygon );

    connect( pLineEditor_, SIGNAL( Done() ), this, SLOT( TracingDone() ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamLocationList constructor
// Created: APE 2004-09-07
// -----------------------------------------------------------------------------
MOS_ParamLocationList::MOS_ParamLocationList( ASN1T_ListPoint& asnListPoint, const std::string strLabel, const std::string strMenuText, QWidget* pParent, bool bOptional )
    : MOS_ParamListView ( strLabel, true, pParent )
    , MOS_Param_ABC     ( bOptional )
    , strMenuText_      ( strMenuText )
    , asnListLoc_       ( (ASN1T_ListLocalisation&)asnListPoint )
    , pAsnLocalisationList_ ( 0 )
    , pLineEditor_      ( new MOS_ShapeEditorMapEventFilter( this ) )
{
    pPopupMenu_ = new QPopupMenu( this );
    int n;
    n = pPopupMenu_->insertItem( MOS_Tools::ToString( EnumTypeLocalisation::point ), this, SLOT( StartTracing( int ) ) );
    pPopupMenu_->setItemParameter( n, (int)EnumTypeLocalisation::point );

    connect( pLineEditor_, SIGNAL( Done() ), this, SLOT( TracingDone() ) );
}



// -----------------------------------------------------------------------------
// Name: MOS_ParamLocationList destructor
// Created: APE 2004-05-07
// -----------------------------------------------------------------------------
MOS_ParamLocationList::~MOS_ParamLocationList()
{
    delete[] pAsnLocalisationList_;
    while( ! asnUMTCoordPtrList_.empty() )
    {
        delete[] asnUMTCoordPtrList_.back();
        asnUMTCoordPtrList_.pop_back();
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamLocationList::FillRemotePopupMenu
// Created: APE 2004-05-07
// -----------------------------------------------------------------------------
void MOS_ParamLocationList::FillRemotePopupMenu( QPopupMenu& popupMenu, const MOS_ActionContext& context )
{
    if( context.pPoint_ == 0 )
        return;

    popupPoint_ = *(context.pPoint_);
    popupMenu.insertItem( strMenuText_.c_str(), pPopupMenu_ );
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamLocationList::CheckValidity
// Created: APE 2004-05-07
// -----------------------------------------------------------------------------
bool MOS_ParamLocationList::CheckValidity()
{
    if( this->childCount() > 0 )
        return true;

    this->TurnHeaderRed( 3000 );
    return false;
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamLocationList::WriteMsg
// Created: APE 2004-05-07
// -----------------------------------------------------------------------------
void MOS_ParamLocationList::WriteMsg( std::stringstream& strMsg )
{
    strMsg << this->header()->label(0).latin1() << ": " << this->childCount() << " localisations.";

    uint nNbrChilds = this->childCount();
    asnListLoc_.n = nNbrChilds;

    assert( !( nNbrChilds == 0 && ! IsOptional() ) );
    if( nNbrChilds == 0 && IsOptional() )
        return;

    delete[] pAsnLocalisationList_;
    pAsnLocalisationList_ = new ASN1T_Localisation[ nNbrChilds ];
    asnListLoc_.elem = pAsnLocalisationList_;

    while( ! asnUMTCoordPtrList_.empty() )
    {
        delete[] asnUMTCoordPtrList_.back();
        asnUMTCoordPtrList_.pop_back();
    }

    QListViewItem* pItem = this->firstChild();
    uint i = 0;
    while( pItem )
    {
        T_LocationItem* pCastItem = (T_LocationItem*)pItem;

        const T_PointVector& locPoints = pCastItem->GetValue().second;
        assert( !locPoints.empty() );

        pAsnLocalisationList_[i].type               = pCastItem->GetValue().first;
        pAsnLocalisationList_[i].vecteur_point.n    = locPoints.size();
        pAsnLocalisationList_[i].vecteur_point.elem = new ASN1T_CoordUTM[ locPoints.size() ];
        asnUMTCoordPtrList_.push_back( pAsnLocalisationList_[i].vecteur_point.elem );
        for( uint j = 0; j < locPoints.size(); ++j )
        {
            std::string strMGRS;
            MOS_App::GetApp().GetWorld().SimToMosMgrsCoord( locPoints[j], strMGRS );
            pAsnLocalisationList_[i].vecteur_point.elem[j] = strMGRS.c_str();
        }

        pItem = pItem->nextSibling();
        ++i;
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamLocationList::StartTracing
// Created: APE 2004-05-07
// -----------------------------------------------------------------------------
void MOS_ParamLocationList::StartTracing( int nLocationType )
{
    T_Location location( (ASN1T_EnumTypeLocalisation)nLocationType, T_PointVector() );

    pItemUnderConstruction_ = new T_LocationItem( location, this, tr( "En cours" ) );

    // Special case for point parameter.
    if( nLocationType == EnumTypeLocalisation::point )
    {
        T_PointVector pointList;
        pointList.push_back( popupPoint_ );
        pItemUnderConstruction_->GetValue().second = pointList;
        pItemUnderConstruction_->setText( 0, MOS_Tools::ToString( (ASN1T_EnumTypeLocalisation)nLocationType ) );
        return;
    }

    // Push map event handler.
    pLineEditor_->PrepareNewLine( (MOS_ShapeEditorMapEventFilter::E_LineType)nLocationType );
    pLineEditor_->Push();
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamLocationList::TracingDone
// Created: APE 2004-05-07
// -----------------------------------------------------------------------------
void MOS_ParamLocationList::TracingDone()
{
    assert( pItemUnderConstruction_ != 0 );
    
    T_PointVector& pointList = pLineEditor_->GetPointList();
    ASN1T_EnumTypeLocalisation nType = pItemUnderConstruction_->GetValue().first;

    if(    (nType == EnumTypeLocalisation::point   && (int)pointList.size() == 1 )
        || (nType == EnumTypeLocalisation::line    && (int)pointList.size() >= 2 )
        || (nType == EnumTypeLocalisation::polygon && (int)pointList.size() > 2 ) )
    {
        pItemUnderConstruction_->GetValue().second = pointList;
        pItemUnderConstruction_->setText( 0, MOS_Tools::ToString( nType ) );
    }
    else
    {
        delete pItemUnderConstruction_;
        pItemUnderConstruction_ = 0;
    }
}
