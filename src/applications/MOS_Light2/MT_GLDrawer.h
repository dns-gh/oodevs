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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MT_GLDrawer.h $
// $Author: Age $
// $Modtime: 19/05/05 15:08 $
// $Revision: 2 $
// $Workfile: MT_GLDrawer.h $
//
// *****************************************************************************

#ifndef __MT_GLDrawer_h_
#define __MT_GLDrawer_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MT_GLDShape_ABC.h"
#include "MT_GLDShape.h"
#include "MT_GLDShapeProperties.h"

class MT_GLDLayer;
class MT_GLDLayerDialog;
class MT_GLDShapeDialog;
class MT_GLFont;

class QGLWidget;
class QMainWindow;

typedef MT_Vector2D (*T_ScreenToGLFunc)( const MT_Vector2D& vScreenPos, QGLWidget& glWidget );


// =============================================================================
/** @class  MT_GLDrawer
    @brief  MT_GLDrawer
    @par    Using example
    @code
    MT_GLDrawer;
    @endcode
*/
// Created: APE 2004-06-23
// =============================================================================
class MT_GLDrawer : public QObject
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( MT_GLDrawer );

public:
    //! @name Constructors/Destructor
    //@{
     MT_GLDrawer( QGLWidget& glWidget, T_ScreenToGLFunc pFunc, QMainWindow& mainWindow );
    ~MT_GLDrawer();
    //@}

    //! @name Operations
    //@{
    void SetDefaults( float rMinFontSize, float rMaxFontSize, const MT_GLDShapeProperties& properties = MT_GLDShapeProperties() );

    void Draw();
    MT_Vector2D ScreenToGL( const MT_Vector2D& vScreenPos );
    //@}

    //! @name Accessors
    //@{
    QGLWidget& GetGLWidget();
    MT_GLFont& GetFont();

    float GetMinFontSize();
    float GetMaxFontSize();

    const MT_GLDShapeProperties& GetDefautShapeProperties() const;
    //@}

public slots:
    void SetActive( bool bOn );
    
    void NewLayer();
    void DeleteCurrentLayer();
    void SetCurrentLayer( MT_GLDLayer* pLayer );

    void OnComboBoxChange();
    void UpdateLayerInCombo( MT_GLDLayer& layer );

    void OnShapeAction( int nAction );
    void CancelCreation();
    void ValidateCreation();

    void MoveSelectionToLayer( int nLayer );
    void SetCurrentLayerVisible( bool bVisible );

    void GroupSelection();
    void DisgroupSelection();

    void Load();
    void Save();
    void SaveAll();

private:
    //! @name Event handeling
    //@{
    bool eventFilter( QObject* pObject, QEvent* pEvent );

    bool MouseRightClickEvent( QMouseEvent* pEvent );
    bool MouseLeftClickEvent( QMouseEvent* pEvent );
    bool MouseMoveEvent( QMouseEvent* pEvent );
    //@}

    //! @name Selection handeling
    //@{
    void SetSelection( MT_GLDShape_ABC& shape );
    void SetSelection( MT_GLDShape_ABC::T_ShapeVector& shapes );
    void AddToSelection( MT_GLDShape_ABC& shape );
    void RemoveFromSelection( MT_GLDShape_ABC& shape );
    void ClearSelection();
    //@}

private:
    typedef std::vector< MT_GLDLayer* >    T_LayerVector;
    typedef T_LayerVector::iterator        IT_LayerVector;
    typedef T_LayerVector::const_iterator  CIT_LayerVector;

private:
    //! @name Member data
    //@{
    QGLWidget& glWidget_;
    T_ScreenToGLFunc pScreenToGLFunc_;
    bool bOn_;

    T_LayerVector layers_;
    MT_GLDLayer* pCurrentLayer_;
    int nLayerCounter_;

    MT_GLDShape_ABC::T_ShapeVector selection_;
    MT_Vector2D* pSelectedPoint_;
    MT_GLDShape_ABC* pUnderCreation_;

    QPopupMenu* pPopupMenu_;

    MT_GLDLayerDialog* pLayerDialog_;
    MT_GLDShapeDialog* pShapeDialog_;

    QToolBar*  pToolBar_;
    MT_ValuedComboBox<MT_GLDLayer*>* pCombo_;
    QButtonGroup* pToolButtonGroup_;
    QToolButton* pVisibleButton_;
    QToolButton* pLayerVisibleButton_;
    QToolButton* pLoadButton_;
    QToolButton* pSaveButton_;
    QToolButton* pSaveAllButton_;
    QToolButton* pDefaultButton_;
    QToolButton* shapeButtons_[MT_GLDShape::eNbrShapes];

    MT_GLFont* pGLFont_;

    float rMinFontSize_;
    float rMaxFontSize_;
    MT_GLDShapeProperties defaultShapeProperties_;

    MT_Vector2D vOldPos_;
    //@}
};


#ifdef MT_USE_INLINE
#   include "MT_GLDrawer.inl"
#endif

#endif // __MT_GLDrawer_h_
