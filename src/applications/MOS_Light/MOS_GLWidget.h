//*****************************************************************************
//
// $Created: FBD 02-11-22 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_GLWidget.h $
// $Author: Age $
// $Modtime: 8/02/05 17:43 $
// $Revision: 12 $
// $Workfile: MOS_GLWidget.h $
//
//*****************************************************************************

#ifndef __MOS_GLWidget_h_
#define __MOS_GLWidget_h_

#include "MOS_Types.h"
#include "MT_Tools/MT_Rect.h"

class QTimer;
class MOS_GLMouse_ABC;

//*****************************************************************************
// Created: FBD 02-11-22
//*****************************************************************************
class MOS_GLWidget : public QGLWidget
{
    Q_OBJECT

        
public:

    typedef std::vector< MOS_GLMouse_ABC* >      T_GLMousePtrVector;
    typedef const T_GLMousePtrVector             CT_GLMousePtrVector;
    typedef T_GLMousePtrVector::iterator         IT_GLMousePtrVector;
    typedef T_GLMousePtrVector::const_iterator   CIT_GLMousePtrVector;
    typedef T_GLMousePtrVector::reverse_iterator RIT_GLMousePtrVector;

public:
    explicit MOS_GLWidget( QWidget* pParent );
    virtual ~MOS_GLWidget();

    //-------------------------------------------------------------------------
    /** @name Init */
    //-------------------------------------------------------------------------
    //@{
    void Initialize();
    void Terminate();
    //@}


    //-------------------------------------------------------------------------
    /** @name Tools Link*/
    //-------------------------------------------------------------------------
    //@{
    void RegisterLink           ( MOS_GLMouse_ABC* pLink );
    void UnregisterLink         ( MOS_GLMouse_ABC* pLink );
    void DrawLink               ();
    int GetNbLink              () const;
    bool RMBPressEventLink      ( const MT_Vector2D& vPos );
    bool LMBPressEventLink      ( const MT_Vector2D& vPos );
    void mouseMoveEventLink     ( const MT_Vector2D& vPos );
    void RMBReleaseEventLink    ( const MT_Vector2D& vPos );
    void LMBReleaseEventLink    ( const MT_Vector2D& vPos );
    //@}

    //-------------------------------------------------------------------------
    /** @name */
    //-------------------------------------------------------------------------
    //@{
    void        SetCenter      ( const MT_Vector2D& vCenter );
    MT_Vector2D GetCenter      () const;
	void        SetWantedCenter( const MT_Vector2D& vWantedCenter );
    MT_Vector2D GetValueTrans  ();
    void        SetCurrentGL   ();
    //@}

    void AddConflict( uint nId, const MT_Vector2D& vStart, const MT_Vector2D& vEnd );
    void DeleteConflict( uint nId );

public slots:
    //-------------------------------------------------------------------------
    /** @name QT Slots */
    //-------------------------------------------------------------------------
    //@{
    void SlotSetXTranslationLeft ();
    void SlotSetXTranslationRight();
    void SlotSetYTranslationUp   ();
    void SlotSetYTranslationDown ();
    void SlotSetCenter           ();
    void SlotSetZoom             (int);
    //@}
    
protected:
    //-------------------------------------------------------------------------
    /** @name */
    //-------------------------------------------------------------------------
    //@{
    virtual void resizeGL           ( int w, int h );
    virtual void paintGL            ();
    virtual void mousePressEvent    ( QMouseEvent* pMouseEvent );
    virtual void mouseMoveEvent     ( QMouseEvent* pMouseEvent );
    virtual void mouseReleaseEvent  ( QMouseEvent* pMouseEvent );
    //@}

private:
    //-------------------------------------------------------------------------
    /** @name */
    //-------------------------------------------------------------------------
    //@{
    virtual void initializeGL();
            void UpdateCenter();
    //@}

    //-------------------------------------------------------------------------
    /** @name Tools */
    //-------------------------------------------------------------------------
    //@{
    void DrawMeasure();
    void ScreenToGL( const MT_Vector2D& vPosScreen, MT_Vector2D& vPosGL );
    void CalculateViewport( int w, int h );
    void Update( unsigned int nWait = 0 );
    void SetupNextUpdate();
    //@}
    
private:
    QTimer*             pTimer_;
    bool                bUpdating_;
    MT_Vector2D         vCenter_;
    MT_Vector2D         vWantedCenter_;
    MT_Vector2D         vGrabCoordinates_;
    bool                bGrab_;

    MT_Float            rZoom_;
    MT_Float            rWantedZoom_;
    MT_Float            rMaxZoom_;


    T_GLMousePtrVector  linkVector_;

    int                 nDrawMeasure_;
    MT_Vector2D         vPosStartMeasure_;
    MT_Vector2D         vPosEndMeasure_;

    MT_Float            rRatio_;
    MT_Rect             viewport_;

    typedef std::pair< MT_Vector2D, MT_Vector2D > T_VectorPair;
    typedef std::map< uint, T_VectorPair >        T_ConflictMap;
    typedef T_ConflictMap::iterator              IT_ConflictMap;
    T_ConflictMap conflicts_;
};

#ifdef MOS_USE_INLINE
#	include "MOS_GLWidget.inl"
#endif


#endif // __MOS_GLWidget_h_
