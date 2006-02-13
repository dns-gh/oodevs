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
// $Archive: /MVW_v10/Build/SDK/Light2/src/DefaultMapEventHandler.h $
// $Author: Ape $
// $Modtime: 5/10/04 15:58 $
// $Revision: 10 $
// $Workfile: DefaultMapEventHandler.h $
//
// *****************************************************************************

#ifndef __DefaultMapEventHandler_h_
#define __DefaultMapEventHandler_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MapEventFilter_ABC.h"
#include "Types.h"
#include "SelectedElement.h"

class Agent;
class ShapeEditorMapEventFilter;
class ActionContext;
class TacticalLine_ABC;
class Object_ABC;
class MapMouseEvent;
class QPopupMenu;
class Agent_ABC;
class Population;
class PopulationConcentration;
class PopulationFlow;


// =============================================================================
// Created: APE 2004-03-26
// =============================================================================
class DefaultMapEventHandler : public QObject, public MapEventFilter_ABC
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( DefaultMapEventHandler );

    friend class GLTool;

public:
    //! @name Constructors/Destructor
    //@{
    DefaultMapEventHandler( QObject* pParent );
    ~DefaultMapEventHandler();
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
    void SetSelectedElement( SelectedElement& selectedElement );
    void ClearSelection();

    void OnObjectDeleted( Object_ABC& object );
    void OnPopulationConcentrationDeleted( const PopulationConcentration& concentration );
    void OnPopulationFlowDeleted( const PopulationFlow& flow );
    void OnPopulationDeleted( Population& pop );
    void OnTacticalLineDeleted( TacticalLine_ABC& line );
    void OnAgentKnowledgeDeleted( Gtia& gtia, AgentKnowledge& knowledge );
    void OnObjectKnowledgeDeleted( Team& team, ObjectKnowledge& knowledge );

    void SwitchToLineCreation( int nType );     //$$$ Transfer those 2 to a new class...
    void LineCreated();

private:
    //! @name Event filter & related helpers
    //@{
    bool eventFilter( QObject* pSender, QEvent* pEvent );

    bool OnMousePress     ( const MapMouseEvent& mouseEvent );
    bool OnMouseRelease   ( const MapMouseEvent& mouseEvent );
    bool OnMouseMove      ( const MapMouseEvent& mouseEvent );
    bool OnKeyPress       ( const QKeyEvent& keyEvent );
    //@}

    //! @name Helpers
    //@{
    void SelectElementAtPos( const MT_Vector2D& vGLPos, float rDistancePerPixel, bool bAutomata = false );
    void HoverElementAtPos( const MT_Vector2D& vGLPos, float rDistancePerPixel );

    bool                         GetAgentAtPos                  ( Agent*&, const MT_Vector2D& vGLPos );
    bool                         IsAgentAtPos                   ( const Agent&   agent, const MT_Vector2D& vGLPos );

    Population*              GetPopulationAtPos             ( const MT_Vector2D& vGLPos );
    bool                         GetPopulationConcentrationAtPos( PopulationConcentration*&, const MT_Vector2D& vGLPos );
    bool                         GetPopulationFlowAtPos         ( PopulationFlow*&, const MT_Vector2D& vGLPos, float rDistancePerPixel );
    bool                         IsPopulationConcentrationAtPos ( const PopulationConcentration& concentration, const MT_Vector2D& vGLPos ) const;
    bool                         IsPopulationFlowAtPos          ( const PopulationFlow& flow, const MT_Vector2D& vGLPos, float rDistancePerPixel ) const;

    bool                         GetAgentKnowledgeAtPos         ( AgentKnowledge*&, const MT_Vector2D& vGLPos );
    bool                         IsAgentKnowledgeAtPos          ( const AgentKnowledge& agent, const MT_Vector2D& vGLPos );

    TacticalLine_ABC*        GetLineAtPos                   ( const MT_Vector2D& vGLPos, float rDistancePerPixel );
    bool                         IsLineAtPos                    ( const TacticalLine_ABC& line, const MT_Vector2D& vGLPos, float rDistancePerPixel );
    int                          GetPointAtPos                  ( TacticalLine_ABC& line, const MT_Vector2D& vGLPos, float rDistancePerPixel );

    bool                         GetObjectAtPos                 ( Object_ABC*&, const MT_Vector2D& vGLPos, float rDistancePerPixel );
    bool                         IsObjectAtPos                  ( const Object_ABC& object, const MT_Vector2D& vGLPos, float rDistancePerPixel );

    bool                         GetObjectKnowledgeAtPos        ( ObjectKnowledge*&, const MT_Vector2D& vGLPos, float rDistancePerPixel );
    bool                         IsObjectKnowledgeAtPos         ( const ObjectKnowledge& object, const MT_Vector2D& vGLPos, float rDistancePerPixel );

    void                         PopupMenu                      ( const MT_Vector2D& vGLPos, float rDistancePerPixel );
    //@}

signals:
    void ElementSelected( SelectedElement& selectedElement );
    void ElementHovered( SelectedElement& hoveredElement );
    void NewPopupMenu( QPopupMenu& popupMenu, const ActionContext& context );
    void CenterOnPoint( const MT_Vector2D& );

private:
      enum E_PhaseSelection
    {
        ePhaseSelectionAgent = 0,
        ePhaseSelectionObject,
        ePhaseSelectionAgentKnowledge,
        ePhaseSelectionObjectKnowledge,
        ePhaseSelectionPopulationConcentration,
        ePhaseSelectionPopulationFlow,
        ePhaseSelectionLooping,
    };

private:
    //! @name Member data
    //@{
    SelectedElement selectedElement_;

    SelectedElement hoveredElement_;

    QPopupMenu* pPopupMenu_;

    MT_Vector2D vOldMousePos_;

    ShapeEditorMapEventFilter* pLineEditor_;
    int nLineType_;

    E_PhaseSelection nSelectionState;
    //@}
};

#   include "DefaultMapEventHandler.inl"

#endif // __DefaultMapEventHandler_h_
