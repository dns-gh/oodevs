// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2005-04-21 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ReportPanel.h $
// $Author: Age $
// $Modtime: 21/04/05 16:06 $
// $Revision: 1 $
// $Workfile: ReportPanel.h $
//
// *****************************************************************************

#ifndef __ReportPanel_h_
#define __ReportPanel_h_

#include "InfoPanel_ABC.h"
#include "Observer_ABC.h"
#include "SelectionObserver_ABC.h"

class ReportListView;
//class FireResultListView;
class ReportFilterOptions;
class Controller;
class ActionController;
class Agent;

// =============================================================================
/** @class  ReportPanel
    @brief  ReportPanel
*/
// Created: AGE 2005-04-21
// =============================================================================
class ReportPanel : public InfoPanel_ABC
                  , private Observer_ABC
                  , public SelectionObserver< Agent >
{
public:
    //! @name Constructors/Destructor
    //@{
             ReportPanel( InfoPanel* pParent, Controller& controller, ActionController& actionController );
    virtual ~ReportPanel();
    //@}

private:
    //! @name Operations
    //@{
    virtual void NotifySelected( const Agent* element );
    //@}    

private:
    //! @name Copy/Assignment
    //@{
    ReportPanel( const ReportPanel& );
    ReportPanel& operator=( const ReportPanel& );
    //@}

private:
    //! @name Member data
    //@{
    ReportFilterOptions* pFilterOptions_;
    ReportListView*      pReportListView_;
//    FireResultListView*  pFireResultListView_;
    //@}
};

#endif // __ReportPanel_h_
