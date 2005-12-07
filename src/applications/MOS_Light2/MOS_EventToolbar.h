// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-09-21 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_EventToolbar.h $
// $Author: Ape $
// $Modtime: 4/10/04 14:38 $
// $Revision: 1 $
// $Workfile: MOS_EventToolbar.h $
//
// *****************************************************************************

#ifndef __MOS_EventToolbar_h_
#define __MOS_EventToolbar_h_

#ifdef __GNUG__
#   pragma interface
#endif

class MOS_Agent;
class MOS_Agent_ABC;
class MOS_SelectedElement;
class MOS_ActionContext;
class MOS_EventToolButton;


// =============================================================================
// Created: APE 2004-09-21
// =============================================================================
class MOS_EventToolbar : public QToolBar
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( MOS_EventToolbar );

public:
    //! @name Constructors/Destructor
    //@{
     MOS_EventToolbar( QMainWindow* pParent );
    ~MOS_EventToolbar();
    //@}

private slots:
    //! @name Slots
    //@{
    void FillRemotePopup( QPopupMenu& popupMenu, const MOS_ActionContext& context );
    void SubscribeToAgent();
    void UnsubscribeFromAgent();
    void OnTeamChanged();
    void ClearSubscriptions();

    void OnAgentOutOfGas( MOS_Agent& agent );
    void OnAgentRefueled( MOS_Agent& agent );

    void OnConflictStarted( MOS_Agent_ABC& origin );
    void OnConflictEnded  ( MOS_Agent_ABC& origin );

    void OnReportCreated  ( MOS_Agent_ABC& agent );
    void OnReadingReports ( MOS_Agent_ABC& agent );

    void FocusOnAgent( int nId, bool bCenter );
    //@}

signals:
    //! @name Signals
    //@{
    void AgentOutOfGas( int nEventId );
    void AgentRefueled( int nEventId );

    void ConflictStarted( int nEventId );
    void ConflictEnded( int nEventId );

    void ReportCreated( int nEventId );
    void ReadingReports( int nEventId );

    void ElementSelected( MOS_SelectedElement& selectedElement );
    void CenterOnPoint( const MT_Vector2D& vPoint );
    //@}

private:
    //! @name Member data
    //@{
    MOS_Agent_ABC* pSelectedAgent_;

    typedef std::vector< MOS_EventToolButton* >   T_ButtonVector;
    typedef T_ButtonVector::iterator              IT_ButtonVector;
    T_ButtonVector agentButtons_;
    //@}
};

#endif // __MOS_EventToolbar_h_
