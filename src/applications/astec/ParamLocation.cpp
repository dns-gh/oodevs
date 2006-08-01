// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-05-06 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ParamLocation.cpp $
// $Author: Age $
// $Modtime: 30/03/05 18:49 $
// $Revision: 9 $
// $Workfile: ParamLocation.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "ParamLocation.h"
#include "moc_ParamLocation.cpp"

#include "App.h"
#include "World.h"
#include "ActionContext.h"
#include "Tools.h"
#include "ShapeEditorMapEventFilter.h"

// -----------------------------------------------------------------------------
// Name: ParamLocation constructor
// Created: APE 2004-05-06
// -----------------------------------------------------------------------------
ParamLocation::ParamLocation( ASN1T_Localisation& asnListPoint, const std::string strLabel, const std::string strMenuText, QWidget* pParent, OptionalParamFunctor_ABC* pOptional, bool bOutsideData )
    : QHBox             ( pParent )
    , Param_ABC     ( pOptional )
    , strMenuText_      ( strMenuText )
    , asnListPoint_     ( asnListPoint )
    , pUMTCoords_       ( 0 )
    , bOutsideData_     ( bOutsideData )
    , nType_            ( EnumTypeLocalisation::point )
    , pLineEditor_      ( new ShapeEditorMapEventFilter( this ) )
{
    this->setSpacing( 5 );
    pLabel_ = new MT_ParameterLabel( strLabel.c_str(), false, this, "" );

    pShapeLabel_ = new QLabel( "---", this );
    pShapeLabel_->setMinimumWidth( 100 );
    pShapeLabel_->setAlignment( Qt::AlignCenter );
    pShapeLabel_->setFrameStyle( QFrame::Box | QFrame::Sunken );

    pPopupMenu_ = new QPopupMenu( this );
    int n;
    n = pPopupMenu_->insertItem( Tools::ToString( EnumTypeLocalisation::point ), this, SLOT( StartTracing( int ) ) );
    pPopupMenu_->setItemParameter( n, (int)EnumTypeLocalisation::point );
    n = pPopupMenu_->insertItem( Tools::ToString( EnumTypeLocalisation::line ), this, SLOT( StartTracing( int ) ) );
    pPopupMenu_->setItemParameter( n, (int)EnumTypeLocalisation::line );
    n = pPopupMenu_->insertItem( Tools::ToString( EnumTypeLocalisation::polygon ), this, SLOT( StartTracing( int ) ) );
    pPopupMenu_->setItemParameter( n, (int)EnumTypeLocalisation::polygon );
    n = pPopupMenu_->insertItem( Tools::ToString( EnumTypeLocalisation::circle ), this, SLOT( StartTracing( int ) ) );
    pPopupMenu_->setItemParameter( n, (int)EnumTypeLocalisation::circle );

    connect( pLineEditor_, SIGNAL( Done() ), this, SLOT( TracingDone() ) );

    if( bOutsideData_ )
    {
        nType_ = asnListPoint_.type;
        pShapeLabel_->setText( Tools::ToString( nType_ ) );

        for( uint n = 0; n < asnListPoint_.vecteur_point.n; ++n )
        {
            MT_Vector2D vPos;
            std::string strPos( (char*)((asnListPoint_.vecteur_point.elem + n)->data), sizeof(asnListPoint_.vecteur_point.elem->data) );
            App::GetApp().GetWorld().MosToSimMgrsCoord( strPos, vPos );
            pointList_.push_back( vPos );
        }
    }
    else
    {
        asnListPoint_.vecteur_point.elem = 0;
        asnListPoint_.vecteur_point.n = 0;
    }
}


// -----------------------------------------------------------------------------
// Name: ParamLocation destructor
// Created: APE 2004-05-06
// -----------------------------------------------------------------------------
ParamLocation::~ParamLocation()
{
    if( ! bOutsideData_ )
        delete[] pUMTCoords_;
}


// -----------------------------------------------------------------------------
// Name: ParamLocation::FillRemotePopupMenu
/** @param  popupMenu 
    @param  context 
*/
// Created: APE 2004-05-06
// -----------------------------------------------------------------------------
void ParamLocation::FillRemotePopupMenu( QPopupMenu& popupMenu, const ActionContext& context )
{
    if( context.pPoint_ == 0 )
        return;

    popupPoint_ = *(context.pPoint_);
    popupMenu.insertItem( strMenuText_.c_str(), pPopupMenu_ );
}


// -----------------------------------------------------------------------------
// Name: ParamLocation::CheckValidity
/** @return 
*/
// Created: APE 2004-05-06
// -----------------------------------------------------------------------------
bool ParamLocation::CheckValidity()
{
    if( ! pointList_.empty() || IsOptional() )
        return true;

    pLabel_->TurnRed( 3000 );
    return false;
}


// -----------------------------------------------------------------------------
// Name: ParamLocation::WriteMsg
// Created: APE 2004-05-06
// -----------------------------------------------------------------------------
void ParamLocation::WriteMsg( std::stringstream& strMsg )
{
    asnListPoint_.type = nType_;
    uint nNbrPoints = pointList_.size();

    strMsg << pLabel_->text().latin1() << ": " << pShapeLabel_->text().latin1() <<". [";

    asnListPoint_.vecteur_point.n = nNbrPoints;
    if( nNbrPoints == 0 )
        return;

    if( pOptional_ )
        pOptional_->SetOptionalPresent();

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
// Name: ParamLocation::StartTracing
/** @param  nLocationType 
*/
// Created: APE 2004-05-06
// -----------------------------------------------------------------------------
void ParamLocation::StartTracing( int nLocationType )
{
    pointList_.clear();
    nType_ = (ASN1T_EnumTypeLocalisation)nLocationType;

    // Special case for point parameter.
    if( nType_ == EnumTypeLocalisation::point )
    {
        pointList_.push_back( popupPoint_ );
        pShapeLabel_->setText( Tools::ToString( nType_ ) );
        return;
    }
    
    pShapeLabel_->setText( tr( "En cours..." ) );
    // Push map event handler.
    pLineEditor_->PrepareNewLine( (ShapeEditorMapEventFilter::E_LineType)nType_ );
    pLineEditor_->Push();
}


// -----------------------------------------------------------------------------
// Name: ParamLocation::TracingDone
// Created: APE 2004-05-06
// -----------------------------------------------------------------------------
void ParamLocation::TracingDone()
{
    T_PointVector& pointList = pLineEditor_->GetPointList();
    if(    (nType_ == EnumTypeLocalisation::point   && (int)pointList.size() == 1 )
        || (nType_ == EnumTypeLocalisation::line    && (int)pointList.size() >= 2 )
        || (nType_ == EnumTypeLocalisation::circle  && (int)pointList.size() == 2 )
        || (nType_ == EnumTypeLocalisation::polygon && (int)pointList.size() > 2 ) )
    {
        pointList_ = pointList;
        pShapeLabel_->setText( Tools::ToString( nType_ ) );
    }
    else
        pShapeLabel_->setText( tr( "---" ) );
}
