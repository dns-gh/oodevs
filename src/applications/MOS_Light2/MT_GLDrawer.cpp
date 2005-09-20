// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-06-23 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MT_GLDrawer.cpp $
// $Author: Age $
// $Modtime: 19/05/05 15:08 $
// $Revision: 4 $
// $Workfile: MT_GLDrawer.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MT_GLDrawer.h"
#include "moc_MT_GLDrawer.cpp"

#include "MT_GLDLayer.h"
#include "MT_GLDLayerDialog.h"
#include "MT_GLDShapeDialog.h"
#include "MT_GLFont.h"

#include <memory.h>

#include "res/visible.xpm"
#include "res/select.xpm"
#include "res/point.xpm"
#include "res/line.xpm"
#include "res/circle.xpm"
#include "res/rectangle.xpm"
#include "res/polygon.xpm"
#include "res/text.xpm"
#include "res/image.xpm"
#include "res/empty.xpm"
#include "res/cross.xpm"
#include "res/open.xpm"
#include "res/save.xpm"
#include "res/saveall.xpm"

// -----------------------------------------------------------------------------
// Name: MT_GLDrawer constructor
/** @param  glWidget 
    @param  pMainWindow 
*/
// Created: APE 2004-06-23
// -----------------------------------------------------------------------------
MT_GLDrawer::MT_GLDrawer( QGLWidget& glWidget, T_ScreenToGLFunc pFunc, QMainWindow& mainWindow, bool bDrawToolbar /* = true */ )
    : QObject           ( &glWidget )
    , glWidget_         ( glWidget )
    , bOn_              ( false )
    , pScreenToGLFunc_  ( pFunc )
    , pCurrentLayer_    ( 0 )
    , nLayerCounter_    ( 0 )
    , pUnderCreation_   ( 0 )
    , pToolBar_         ( 0 )
    , pCombo_           ( 0 )
    , rMinFontSize_     ( 0.5f )
    , rMaxFontSize_     ( 10.0f )
    , pPopupMenu_       ( new QPopupMenu() )
    , pLayerDialog_     ( new MT_GLDLayerDialog( &glWidget ) )
    , pShapeDialog_     ( 0 )
    , pGLFont_          ( new MT_GLFont( "Arial" ) )
    , pSelectedPoint_   ( 0 )
{
    pShapeDialog_ = new MT_GLDShapeDialog( &glWidget, *this );
    if( bDrawToolbar )
    {
        pToolBar_ = new QToolBar( tr( "Dessin" ), &mainWindow );
        pToolBar_->hide();

        pCombo_ = new MT_ValuedComboBox<MT_GLDLayer*>( pToolBar_ );
        pCombo_->setEditable( false );
        pCombo_->AddItem( tr( "Inactif" ), 0 );

        pVisibleButton_ = new QToolButton( QPixmap( xpm_visible ), tr( "Affichage des calques" ), tr( "Affichage des calques" ), 0, 0, pToolBar_ );
        pVisibleButton_->setToggleButton( true );
        pVisibleButton_->setOn( true );

        pLayerVisibleButton_ = new QToolButton( QPixmap( xpm_visible ), tr( "Affichage du calque" ), tr( "Affichage du calque" ), 0, 0, pToolBar_ );
        pLayerVisibleButton_->setToggleButton( true );
        pLayerVisibleButton_->setOn( true );
    
        pToolBar_->addSeparator();

        pLoadButton_ = new QToolButton( QPixmap( xpm_open ), tr( "Ouvrir" ), tr( "Ouvrir" ), this, SLOT( Load() ), pToolBar_ );
        pSaveButton_ = new QToolButton( QPixmap( xpm_save ), tr( "Enregistrer" ), tr( "Enregistrer" ), this, SLOT( Save() ), pToolBar_ );
        pSaveAllButton_ = new QToolButton( QPixmap( xpm_saveall ), tr( "Enregistrer tous" ), tr( "Enregistrer tous" ), this, SLOT( SaveAll() ), pToolBar_ );

        pToolBar_->addSeparator();

        pToolButtonGroup_ = new QButtonGroup( pToolBar_ );
        pToolButtonGroup_->setExclusive( true );
        pToolButtonGroup_->hide();

        pDefaultButton_ = new QToolButton( QPixmap( xpm_select ), tr( "Sélection des objets" ), tr( "Sélection des objets" ), 0, 0, pToolBar_ );
        pDefaultButton_->setToggleButton( true );
        pDefaultButton_->setOn( true );
        pToolButtonGroup_->insert( pDefaultButton_ );

        QSignalMapper* pMapper = new QSignalMapper( this );
        connect( pMapper, SIGNAL( mapped( int ) ), this, SLOT( OnShapeAction( int ) ) );
        shapeButtons_[MT_GLDShape::ePoint]      =  new QToolButton( QPixmap( xpm_point ),     tr( "Point" ),     tr( "Point" ),     pMapper, SLOT( map() ), pToolBar_ );
        shapeButtons_[MT_GLDShape::eLine]       =  new QToolButton( QPixmap( xpm_line ),      tr( "Ligne" ),     tr( "Ligne" ),     pMapper, SLOT( map() ), pToolBar_ );
        shapeButtons_[MT_GLDShape::eRectangle]  =  new QToolButton( QPixmap( xpm_rectangle ), tr( "Rectangle" ), tr( "Rectangle" ), pMapper, SLOT( map() ), pToolBar_ );
        shapeButtons_[MT_GLDShape::ePolygon]    =  new QToolButton( QPixmap( xpm_polygon ),   tr( "Polygone" ),  tr( "Polygone" ),  pMapper, SLOT( map() ), pToolBar_ );
        shapeButtons_[MT_GLDShape::eCircle]     =  new QToolButton( QPixmap( xpm_circle ),    tr( "Cercle" ),    tr( "Cercle" ),    pMapper, SLOT( map() ), pToolBar_ );
        shapeButtons_[MT_GLDShape::eText]       =  new QToolButton( QPixmap( xpm_text ),      tr( "Texte" ),     tr( "Texte" ),     pMapper, SLOT( map() ), pToolBar_ );
        shapeButtons_[MT_GLDShape::eImage]      =  new QToolButton( QPixmap( xpm_image ),     tr( "Image" ),     tr( "Image" ),     pMapper, SLOT( map() ), pToolBar_ );

        for( int n = 0; n < MT_GLDShape::eNbrShapes; ++n )
        {
            shapeButtons_[n]->setToggleButton( true );
            pToolButtonGroup_->insert( shapeButtons_[n] );
            pMapper->setMapping( shapeButtons_[n], n );
        }

        NewLayer();
        SetActive( false );

        connect( pLayerVisibleButton_, SIGNAL( toggled( bool ) ), this, SLOT( SetCurrentLayerVisible( bool ) ) );
        connect( pCombo_, SIGNAL( activated( int ) ), this, SLOT( OnComboBoxChange() ) );
        connect( pLayerDialog_, SIGNAL( LayerEdited( MT_GLDLayer& ) ), this, SLOT( UpdateLayerInCombo( MT_GLDLayer& ) ) );
    }
}


// -----------------------------------------------------------------------------
// Name: MT_GLDrawer destructor
// Created: APE 2004-06-23
// -----------------------------------------------------------------------------
MT_GLDrawer::~MT_GLDrawer()
{
    MT_DELETEOWNED( layers_ );
    delete pPopupMenu_;
    delete pGLFont_;
}


// -----------------------------------------------------------------------------
// Name: MT_GLDrawer::Draw
// Created: APE 2004-06-24
// -----------------------------------------------------------------------------
void MT_GLDrawer::Draw()
{
    if( !pToolBar_ || !pVisibleButton_->isOn() )
        return;

    for( IT_LayerVector it = layers_.begin(); it != layers_.end(); ++it )
        (*it)->Draw();

    if( pUnderCreation_ != 0 )
        pUnderCreation_->Draw();
}

// -----------------------------------------------------------------------------
// Name: MT_GLDrawer::OnShow
// Created: SBO 2005-09-05
// -----------------------------------------------------------------------------
void MT_GLDrawer::OnShow()
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: MT_GLDrawer::OnHide
// Created: SBO 2005-09-05
// -----------------------------------------------------------------------------
void MT_GLDrawer::OnHide()
{
    if( pToolBar_ )
        SetActive( false );
}

// -----------------------------------------------------------------------------
// Name: MT_GLDrawer::SetActive
/** @param  bOn 
*/
// Created: APE 2004-06-23
// -----------------------------------------------------------------------------
void MT_GLDrawer::SetActive( bool bOn )
{
    if( bOn == bOn_ )
        return;
    bOn_ = bOn;

    if( bOn )
    {
        glWidget_.installEventFilter( this );
        pDefaultButton_->setEnabled( true );
        pDefaultButton_->setOn( true );
        for( int n = 0; n < MT_GLDShape::eNbrShapes; ++n )
            shapeButtons_[n]->setEnabled( true );
        pVisibleButton_->setOn( true );
        pVisibleButton_->setEnabled( false );
        pLoadButton_->setEnabled( true );
        pSaveButton_->setEnabled( true );
        pSaveAllButton_->setEnabled( true );

        if( pCombo_->GetValue() == 0 )  
            SetCurrentLayer( layers_.front() );
    }
    else
    {
        glWidget_.removeEventFilter( this );
        pDefaultButton_->setEnabled( false );
        pDefaultButton_->setOn( true );
        for( int n = 0; n < MT_GLDShape::eNbrShapes; ++n )
            shapeButtons_[n]->setEnabled( false );
        pToolButtonGroup_->setEnabled( false );
        pVisibleButton_->setEnabled( true );
        pLoadButton_->setEnabled( false );
        pSaveButton_->setEnabled( false );
        pSaveAllButton_->setEnabled( false );

        if( pCombo_->GetValue() != 0 )  
            SetCurrentLayer( 0 );
    }
}
    
// -----------------------------------------------------------------------------
// Name: MT_GLDrawer::NewLayer
// Created: APE 2004-06-23
// -----------------------------------------------------------------------------
void MT_GLDrawer::NewLayer()
{
    ++nLayerCounter_;
    MT_GLDLayer* pLayer = new MT_GLDLayer( tr( "Calque " ) + QString::number( nLayerCounter_ ), *this );
    layers_.push_back( pLayer );
    pCombo_->AddItem( pLayer->GetName(), pLayer );
    UpdateLayerInCombo( *pLayer );    // To correctly set the visibility icon in the combo entry.

    SetCurrentLayer( pLayer );
}


// -----------------------------------------------------------------------------
// Name: MT_GLDrawer::DeleteCurrentLayer
// Created: APE 2004-06-23
// -----------------------------------------------------------------------------
void MT_GLDrawer::DeleteCurrentLayer()
{
    assert( pCurrentLayer_ != 0 );
    ClearSelection();

    IT_LayerVector it = std::find( layers_.begin(), layers_.end(), pCurrentLayer_ );
    assert( it != layers_.end() );
    it = layers_.erase( it );
    
    pCombo_->RemoveItem( pCurrentLayer_ );

    delete pCurrentLayer_;

    if( layers_.empty() )
    {
        nLayerCounter_ = 0;
        NewLayer();
    }
    else
        SetCurrentLayer( layers_.front() );
}


// -----------------------------------------------------------------------------
// Name: MT_GLDrawer::SetCurrentLayer
/** @param  pLayer 
*/
// Created: APE 2004-06-23
// -----------------------------------------------------------------------------
void MT_GLDrawer::SetCurrentLayer( MT_GLDLayer* pLayer )
{
    delete pUnderCreation_;
    pUnderCreation_ = 0;
    pDefaultButton_->setOn( true );

    pCurrentLayer_ = pLayer;
    pLayerDialog_->SetLayer( pLayer );
    pCombo_->SetCurrentItem( pLayer );
    if( pCurrentLayer_ == 0 )
    {
        pLayerVisibleButton_->setEnabled( false );
        pLayerVisibleButton_->setOn( false );
    }
    else
    {
        pLayerVisibleButton_->setEnabled( true );
        pLayerVisibleButton_->setOn( pCurrentLayer_->IsVisible() );
    }

    SetActive( pCurrentLayer_ != 0 );
}


// -----------------------------------------------------------------------------
// Name: MT_GLDrawer::OnComboBoxChange
// Created: APE 2004-06-23
// -----------------------------------------------------------------------------
void MT_GLDrawer::OnComboBoxChange()
{
    SetCurrentLayer( pCombo_->GetValue() );
}


// -----------------------------------------------------------------------------
// Name: MT_GLDrawer::UpdateLayerInCombo
/** @param  layer 
*/
// Created: APE 2004-06-24
// -----------------------------------------------------------------------------
void MT_GLDrawer::UpdateLayerInCombo( MT_GLDLayer& layer )
{
    if( layer.IsVisible() )
        pCombo_->ChangeItem( QPixmap( xpm_empty ), layer.GetName(), &layer );
    else
        pCombo_->ChangeItem( QPixmap( xpm_cross ), layer.GetName(), &layer );
}


// -----------------------------------------------------------------------------
// Name: MT_GLDrawer::OnShapeAction
// Created: APE 2004-06-24
// -----------------------------------------------------------------------------
void MT_GLDrawer::OnShapeAction( int nAction )
{
    delete pUnderCreation_;
    ClearSelection();

    SetCurrentLayerVisible( true );
    
    MT_GLDShape* pShape = new MT_GLDShape( *this, (MT_GLDShape::E_ShapeType)nAction );
    connect( pShape, SIGNAL( CreationDone() ), this, SLOT( ValidateCreation() ) );
    connect( pShape, SIGNAL( CreationCanceled() ), this, SLOT( CancelCreation() ) );
    pShape->StartCreation();
    pUnderCreation_ = pShape;
}


// -----------------------------------------------------------------------------
// Name: MT_GLDrawer::CancelCreation
// Created: APE 2004-06-24
// -----------------------------------------------------------------------------
void MT_GLDrawer::CancelCreation()
{
    delete pUnderCreation_;
    pUnderCreation_ = 0;
    pDefaultButton_->setOn( true );
}


// -----------------------------------------------------------------------------
// Name: MT_GLDrawer::ValidateCreation
// Created: APE 2004-06-24
// -----------------------------------------------------------------------------
void MT_GLDrawer::ValidateCreation()
{
    assert( pUnderCreation_ != 0 );
    disconnect( pUnderCreation_, SIGNAL( CreationDone() ), this, SLOT( ValidateCreation() ) );
    disconnect( pUnderCreation_, SIGNAL( CreationCanceled() ), this, SLOT( CancelCreation() ) );
    pCurrentLayer_->AddShape( *pUnderCreation_ );
    SetSelection( *pUnderCreation_ );
    pUnderCreation_ = 0;
    pDefaultButton_->setOn( true );
}


// -----------------------------------------------------------------------------
// Name: MT_GLDrawer::MoveSelectionToLayer
/** @param  nLayer 
*/
// Created: APE 2004-06-30
// -----------------------------------------------------------------------------
void MT_GLDrawer::MoveSelectionToLayer( int nLayer )
{
    assert( ! selection_.empty() );
    pCurrentLayer_->RemoveShapes( selection_ );
    layers_[nLayer]->AddShapes( selection_ );
    ClearSelection();
}


// -----------------------------------------------------------------------------
// Name: MT_GLDrawer::SetCurrentLayerVisible
/** @param  bVisible 
*/
// Created: APE 2004-07-02
// -----------------------------------------------------------------------------
void MT_GLDrawer::SetCurrentLayerVisible( bool bVisible )
{
    if( pCurrentLayer_ == 0 )
        return;

    pLayerVisibleButton_->setOn( bVisible );
    pCurrentLayer_->SetVisible( bVisible );
    UpdateLayerInCombo( *pCurrentLayer_ );
}


// -----------------------------------------------------------------------------
// Name: MT_GLDrawer::eventFilter
/** @param  pObject 
    @param  pEvent 
    @return 
*/
// Created: APE 2004-06-23
// -----------------------------------------------------------------------------
bool MT_GLDrawer::eventFilter( QObject* /*pObject*/, QEvent* pEvent )
{
    if( pEvent->type() == QEvent::MouseButtonPress )
    {
        QMouseEvent* pMouseEvent = (QMouseEvent*)pEvent;
        if( pMouseEvent->button() & Qt::RightButton )
            return MouseRightClickEvent( pMouseEvent );
        
        if( pMouseEvent->button() & Qt::LeftButton )
            return MouseLeftClickEvent( pMouseEvent );
    }

    if( pEvent->type() == QEvent::MouseMove )
    {
        QMouseEvent* pMouseEvent = (QMouseEvent*)pEvent;
        if( pMouseEvent->state() & Qt::LeftButton )
            return MouseMoveEvent( pMouseEvent );
    }

    if( pEvent->type() == QEvent::MouseButtonRelease )
        pSelectedPoint_ = 0;

    if( pEvent->type() == QEvent::MouseButtonDblClick )
    {
        if( ! selection_.empty() )
            pShapeDialog_->show();
        return true;
    }

    if( pEvent->type() == QEvent::KeyPress )
    {
        QKeyEvent* pKeyEvent = (QKeyEvent*)pEvent;
        if( pKeyEvent->key() == Qt::Key_Delete || pKeyEvent->key() == Qt::Key_Backspace )
        {
            MT_GLDShape_ABC::T_ShapeVector oldSelection = selection_;
            ClearSelection();
            pCurrentLayer_->DeleteShapes( oldSelection );
            return true;
        }
    }

    return false;
}


// -----------------------------------------------------------------------------
// Name: MT_GLDrawer::MouseRightClickEvent
/** @param  pEvent 
*/
// Created: APE 2004-06-23
// -----------------------------------------------------------------------------
bool MT_GLDrawer::MouseRightClickEvent( QMouseEvent* /*pEvent*/ )
{
    pPopupMenu_->clear();
    if( ! selection_.empty() )
    {
        if( selection_.size() > 1 )
            pPopupMenu_->insertItem( tr( "Grouper" ), this, SLOT( GroupSelection() ) );
        if( selection_.size() == 1 && selection_.front()->GetShapes().size() > 0 )
            pPopupMenu_->insertItem( tr( "Dissocier" ), this, SLOT( DisgroupSelection() ) );

        pPopupMenu_->insertItem( tr( "Propriétés" ), pShapeDialog_, SLOT( show() ) );
        
        if( layers_.size() > 1 )
        {
            QPopupMenu* pSubMenu1 = new QPopupMenu( pPopupMenu_ );
            for( IT_LayerVector it = layers_.begin(); it != layers_.end(); ++it )
            {
                if( *it == pCurrentLayer_ )
                    continue;
                int n = pSubMenu1->insertItem( (*it)->GetName(), this, SLOT( MoveSelectionToLayer( int ) ) );
                pSubMenu1->setItemParameter( n, it - layers_.begin() );
            }
            pPopupMenu_->insertItem( tr( "Transferer vers" ), pSubMenu1 );
        }

        pPopupMenu_->insertSeparator();
    }

    pPopupMenu_->insertItem( tr( "Propriétés du calque" ), pLayerDialog_, SLOT( show() ) );
    pPopupMenu_->insertItem( tr( "Nouveau calque" ), this, SLOT( NewLayer() ) );
    pPopupMenu_->insertItem( tr( "Supprimer calque" ), this, SLOT( DeleteCurrentLayer() ) );

    pPopupMenu_->popup( QCursor::pos() );

    return true; // intercept the event
}


// -----------------------------------------------------------------------------
// Name: MT_GLDrawer::MouseLeftClickEvent
/** @param  pEvent 
*/
// Created: APE 2004-06-29
// -----------------------------------------------------------------------------
bool MT_GLDrawer::MouseLeftClickEvent( QMouseEvent* pEvent )
{
    assert( pCurrentLayer_ != 0 );
    pSelectedPoint_ = 0;
    MT_Vector2D vPos =  ScreenToGL( MT_Vector2D( pEvent->x(), pEvent->y() ) );
    MT_GLDShape_ABC* pShape = pCurrentLayer_->GetShapeAtPos( vPos );

    // If Shift is pressed...
    if( (pEvent->state() & Qt::ShiftButton) )
    {
        // And if we clicked on a shape, we either add it to the selection
        // or remove it if it was already in it.
        if( pShape != 0 )
        {
            MT_GLDShape_ABC::IT_ShapeVector it = std::find( selection_.begin(), selection_.end(), pShape );
            if( it != selection_.end() )
                RemoveFromSelection( **it );
            else
                AddToSelection( *pShape );
        }
    }
    else // If Shift is not pressed...
    {
        // And if we clicked on a shape...
        if( pShape != 0 )
        {
            MT_GLDShape_ABC::IT_ShapeVector it = std::find( selection_.begin(), selection_.end(), pShape );
            // If it's not in the selection, make it the selection.
            if( it == selection_.end() )
                SetSelection( *pShape );
            else // Otherwise check if we're selecting one of its points.
                pSelectedPoint_ = pShape->GetPointAt( vPos );
        }
        else
            ClearSelection();
    }

    vOldPos_ = vPos;

    return true;
}


// -----------------------------------------------------------------------------
// Name: MT_GLDrawer::MouseMoveEvent
/** @param  pEvent 
*/
// Created: APE 2004-07-01
// -----------------------------------------------------------------------------
bool MT_GLDrawer::MouseMoveEvent( QMouseEvent* pEvent )
{
    MT_Vector2D vGLPos = ScreenToGL( MT_Vector2D( pEvent->x(), pEvent->y() ) );
    
    if( pSelectedPoint_ != 0 )
        *pSelectedPoint_ = vGLPos;
    else
    {
        for( MT_GLDShape_ABC::IT_ShapeVector it = selection_.begin(); it != selection_.end(); ++it )
            (*it)->Translate( vGLPos - vOldPos_ );
    }

    vOldPos_ = vGLPos;

    return true;
}


// -----------------------------------------------------------------------------
// Name: MT_GLDrawer::GroupSelection
// Created: APE 2004-07-08
// -----------------------------------------------------------------------------
void MT_GLDrawer::GroupSelection()
{
    assert( pCurrentLayer_ != 0 && selection_.size() > 1 );
    MT_GLDShape_ABC& newShape = pCurrentLayer_->Group( selection_ );
    SetSelection( newShape );
}


// -----------------------------------------------------------------------------
// Name: MT_GLDrawer::DisgroupSelection
// Created: APE 2004-07-08
// -----------------------------------------------------------------------------
void MT_GLDrawer::DisgroupSelection()
{
    assert( pCurrentLayer_ != 0 && selection_.size() == 1 );
    MT_GLDShape_ABC::T_ShapeVector shapes = pCurrentLayer_->Disgroup( *(selection_.front()) );
    selection_.clear(); // The selected item (which was a group) was destroyed during the disgroup.
    SetSelection( shapes );
}


// -----------------------------------------------------------------------------
// Name: MT_GLDrawer::Load
// Created: APE 2004-07-06
// -----------------------------------------------------------------------------
void MT_GLDrawer::Load()
{
    ClearSelection();
    pSelectedPoint_ = 0;
    QString strFileName = QFileDialog::getOpenFileName(
                    "",
                    "Calque (*.txt *.xml *.ini)",
                    & glWidget_,
                    "",
                    tr( "Ouvrir" ) );

    if( strFileName.isNull() )
        return;

    MT_XXmlInputArchive archive;
    archive.EnableExceptions( true );
    try
    {
        archive.Open( strFileName.ascii() );
        archive.BeginList( "Calques" );
        while( archive.NextListElement() )
        {
            archive.Section( "Calque" );
            std::auto_ptr<MT_GLDLayer> spLayer( new MT_GLDLayer( "", *this ) );
            spLayer->Read( archive );
            MT_GLDLayer* pLayer = spLayer.release();
            layers_.push_back( pLayer );
            pCombo_->AddItem( pLayer->GetName(), pLayer );
            UpdateLayerInCombo( *pLayer );    // To correctly set the visibility icon in the combo entry.
            archive.EndSection();
        }
        archive.EndList();
    }
    catch(...)
    {
        QMessageBox::warning( &glWidget_, "MOSLight 2", tr("Erreur lors de la lecture des calques.\nOpération annulée.") );
        return;
    }
    SetCurrentLayer( layers_.back() );
}


// -----------------------------------------------------------------------------
// Name: MT_GLDrawer::Save
// Created: APE 2004-07-06
// -----------------------------------------------------------------------------
void MT_GLDrawer::Save()
{
    ClearSelection();
    pSelectedPoint_ = 0;
    assert( pCurrentLayer_ != 0 );
    QString strFileName = QFileDialog::getSaveFileName(
                    "",
                    "Calque (*.txt *.xml *.ini)",
                    & glWidget_,
                    "",
                    tr( "Enregister le calque sous" ) );

    if( strFileName.isNull() )
        return;

    MT_XXmlOutputArchive archive;
    archive.EnableExceptions( true );
    try
    {
        archive.BeginList( "Calques", 1 );

        archive.Section( "Calque" );
        pCurrentLayer_->Write( archive );
        archive.EndSection();

        archive.EndList();

        archive.WriteToFile( strFileName.ascii() );
    }
    catch(...)
    {
        QMessageBox::warning( &glWidget_, "MOSLight 2", tr("Erreur lors de l'enregistrement du calque.\nOpération annulée.") );
        return;
    }
}


// -----------------------------------------------------------------------------
// Name: MT_GLDrawer::SaveAll
// Created: APE 2004-07-07
// -----------------------------------------------------------------------------
void MT_GLDrawer::SaveAll()
{
    ClearSelection();
    pSelectedPoint_ = 0;
    QString strFileName = QFileDialog::getSaveFileName(
                    "",
                    "Calque (*.txt *.xml *.ini)",
                    & glWidget_,
                    "",
                    tr( "Enregister les calques sous" ) );

    if( strFileName.isNull() )
        return;

    MT_XXmlOutputArchive archive;
    archive.EnableExceptions( true );
    try
    {
        archive.BeginList( "Calques", layers_.size() );
        for( IT_LayerVector it = layers_.begin(); it != layers_.end(); ++it )
        {
            archive.Section( "Calque" );
            (*it)->Write( archive );
            archive.EndSection();
        }
        archive.EndList();

        archive.WriteToFile( strFileName.ascii() );
    }
    catch(...)
    {
        QMessageBox::warning( &glWidget_, "MOSLight 2", tr("Erreur lors de l'enregistrement des calques.\nOpération annulée.") );
        return;
    } 
}
