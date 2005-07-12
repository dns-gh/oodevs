// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-09-28 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_EventToolButton.h $
// $Author: Ape $
// $Modtime: 4/10/04 12:00 $
// $Revision: 1 $
// $Workfile: MOS_EventToolButton.h $
//
// *****************************************************************************

#ifndef __MOS_EventToolButton_h_
#define __MOS_EventToolButton_h_

#ifdef __GNUG__
#   pragma interface
#endif

class MOS_Agent;
class MOS_SelectedElement;

// =============================================================================
/** @class  MOS_EventToolButton
    @brief  
*/
// Created: APE 2004-09-28
// =============================================================================
class MOS_EventToolButton : public QToolButton
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( MOS_EventToolButton );

public:
    //! @name Constructors/Destructor
    //@{
     MOS_EventToolButton( const QIconSet& iconSet, const QString& toolTip, QToolBar* pParent, int nFilterId = -1 );
    ~MOS_EventToolButton();
    //@}

    //! @name Accessors
    //@{
    int GetFilterId() const;
    //@}

    //! @name Operations
    //@{
    void SetSignalsSlots( QObject* pEmitter, const char* pNewEventSignal, const char* pEventViewedSignal, const char* pFocusOnEventSlot );
    //@}

public slots:
    //! @name Slots
    //@{
    void OnNewEvent( int nEventId );
    void OnEventViewed( int nEventId );
    void ClearEvents();
    //@}

private slots:
    //! @name Private slots
    //@{
    void OnClicked();
    void OnTimer();
    //@}

signals:
    //! @name Signals
    //@{
    void FocusOnEvent( int nEventId, bool bCenterOnEvent );
    //@}

private:
    //! @name Member data
    //@{
    int nFilterId_;   // If nFilterId_ != -1, we must only take care of events with id nFilterId_;

    QTimer& timer_;
    QString toolTip_;
 
    T_IntVector currentEvents_;
    //@}
};


#include "MOS_EventToolButton.inl"

#endif // __MOS_EventToolButton_h_
