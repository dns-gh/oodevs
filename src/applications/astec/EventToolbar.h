// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __EventToolbar_h_
#define __EventToolbar_h_

class Agent;
class Agent_ABC;
class SelectedElement;
class ActionContext;
class EventToolButton;
class Options;

// =============================================================================
// Created: APE 2004-09-21
// =============================================================================
class EventToolbar : public QToolBar
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( EventToolbar );

public:
    //! @name Constructors/Destructor
    //@{
    explicit EventToolbar( QMainWindow* pParent );
    virtual ~EventToolbar();
    //@}

private slots:
    //! @name Slots
    //@{
    void FillRemotePopup( QPopupMenu& popupMenu, const ActionContext& context );
    void SubscribeToAgent();
    void UnsubscribeFromAgent();
    void OnTeamChanged();
    void ClearSubscriptions();

    void OnAgentOutOfGas( Agent& agent );
    void OnAgentRefueled( Agent& agent );

    void OnConflictStarted( Agent_ABC& origin );
    void OnConflictEnded  ( Agent_ABC& origin );

    void OnReportCreated  ( Agent_ABC& agent );
    void OnReadingReports ( Agent_ABC& agent );

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

    void ElementSelected( SelectedElement& selectedElement );
    void CenterOnPoint( const MT_Vector2D& vPoint );
    //@}

private:
    //! @name Member data
    //@{
    Options& options_;
    Agent_ABC* pSelectedAgent_;

    typedef std::vector< EventToolButton* >   T_ButtonVector;
    typedef T_ButtonVector::iterator              IT_ButtonVector;
    T_ButtonVector agentButtons_;
    //@}
};

#endif // __EventToolbar_h_
