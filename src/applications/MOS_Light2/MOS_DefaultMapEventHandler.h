// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-03-26 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_DefaultMapEventHandler.h $
// $Author: Ape $
// $Modtime: 5/10/04 15:58 $
// $Revision: 10 $
// $Workfile: MOS_DefaultMapEventHandler.h $
//
// *****************************************************************************

#ifndef __MOS_DefaultMapEventHandler_h_
#define __MOS_DefaultMapEventHandler_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_MapEventFilter_ABC.h"
#include "MOS_Types.h"
#include "MOS_SelectedElement.h"

class MOS_Agent;
class MOS_ShapeEditorMapEventFilter;
class MOS_ActionContext;
class MOS_TacticalLine_ABC;
class MOS_Object_ABC;
class MOS_MapMouseEvent;
class QPopupMenu;


// =============================================================================
/** @class  MOS_DefaultMapEventHandler
    @brief  MOS_DefaultMapEventHandler
    @par    Using example
    @code
    MOS_DefaultMapEventHandler;
    @endcode
*/
// Created: APE 2004-03-26
// =============================================================================
class MOS_DefaultMapEventHandler : public QObject, public MOS_MapEventFilter_ABC
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( MOS_DefaultMapEventHandler );

    friend class MOS_GLTool;

public:
    //! @name Constructors/Destructor
    //@{
    MOS_DefaultMapEventHandler( QObject* pParent );
    ~MOS_DefaultMapEventHandler();
    //@}

    //! @name Operations
    //@{
    void Draw() const;
    //@}

    //! @name Accessors
    //@{
    QObject* GetObject();
    //@}

public slots:
    void SetSelectedElement( MOS_SelectedElement& selectedElement );
    void ClearSelection();

    void OnObjectDeleted( MOS_Object_ABC& object );
    void OnTacticalLineDeleted( MOS_TacticalLine_ABC& line );
    void OnAgentKnowledgeDeleted( MOS_Gtia& gtia, MOS_AgentKnowledge& knowledge );
    void OnObjectKnowledgeDeleted( MOS_Team& team, MOS_ObjectKnowledge& knowledge );

    void SwitchToLineCreation( int nType );     //$$$ Transfer those 2 to a new class...
    void LineCreated();

private:
    //! @name Event filter & related helpers
    //@{
    bool eventFilter( QObject* pSender, QEvent* pEvent );

    bool OnMousePress     ( const MOS_MapMouseEvent& mouseEvent );
    bool OnMouseRelease   ( const MOS_MapMouseEvent& mouseEvent );
    bool OnMouseMove      ( const MOS_MapMouseEvent& mouseEvent );
    bool OnKeyPress       ( const QKeyEvent& keyEvent );
    //@}

    //! @name Helpers
    //@{
    void SelectElementAtPos( const MT_Vector2D& vGLPos, float rDistancePerPixel );

    MOS_Agent* GetAgentAtPos( const MT_Vector2D& vGLPos );
    bool IsAgentAtPos( const MOS_Agent& agent, const MT_Vector2D& vGLPos );

    MOS_AgentKnowledge* GetAgentKnowledgeAtPos( const MT_Vector2D& vGLPos );
    bool IsAgentKnowledgeAtPos( const MOS_AgentKnowledge& agent, const MT_Vector2D& vGLPos );

    MOS_TacticalLine_ABC* GetLineAtPos( const MT_Vector2D& vGLPos, float rDistancePerPixel );
    bool IsLineAtPos( const MOS_TacticalLine_ABC& line, const MT_Vector2D& vGLPos, float rDistancePerPixel );
    int GetPointAtPos( MOS_TacticalLine_ABC& line, const MT_Vector2D& vGLPos, float rDistancePerPixel );

    MOS_Object_ABC* GetObjectAtPos( const MT_Vector2D& vGLPos, float rDistancePerPixel );
    bool IsObjectAtPos( const MOS_Object_ABC& object, const MT_Vector2D& vGLPos, float rDistancePerPixel );

    MOS_ObjectKnowledge* GetObjectKnowledgeAtPos( const MT_Vector2D& vGLPos, float rDistancePerPixel );
    bool IsObjectKnowledgeAtPos( const MOS_ObjectKnowledge& object, const MT_Vector2D& vGLPos, float rDistancePerPixel );

    void PopupMenu( const MT_Vector2D& vGLPos, float rDistancePerPixel );
    //@}

signals:
    void ElementSelected( MOS_SelectedElement& selectedElement );
    void NewPopupMenu( QPopupMenu& popupMenu, const MOS_ActionContext& context );

private:
    //! @name Member data
    //@{
    MOS_SelectedElement selectedElement_;

    QPopupMenu* pPopupMenu_;

    MT_Vector2D vOldMousePos_;

    MOS_ShapeEditorMapEventFilter* pLineEditor_;
    int nLineType_;
    //@}
};


#ifdef MOS_USE_INLINE
#   include "MOS_DefaultMapEventHandler.inl"
#endif

#endif // __MOS_DefaultMapEventHandler_h_
