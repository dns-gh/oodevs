// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticToolbar_h_
#define __LogisticToolbar_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "Options.h"

// =============================================================================
// Created: HME 2005-11-02
// =============================================================================
class LogisticToolbar : public QToolBar
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( LogisticToolbar );

public:
    //! @name Constructors/Destructor
    //@{
    LogisticToolbar( QMainWindow* pParent );
    ~LogisticToolbar();
    //@}

private slots:
    //void OnAutoSetLogisticLinks();
    void OnLogisticLinks( float rValue );
    void OnMissingLogisticLinks( float rValue );
    void OnRealTimeLog();
    void OnAggregateAll();
    void OnDesaggregateAll();

private:
    void FillButton( MT_ToolListButton& button, Options::E_State nState );


private:

    //QToolButton* pAutoSetLogisticLinksButton_;
    QToolButton* pRealTimeLogButton_;
    QToolButton*  pAggregateAllButton_;
    QToolButton*  pDesaggregateAllButton_;

};

#endif // __LogisticToolbar_h_
