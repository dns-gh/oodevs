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
#include "AgentSelectionObserver.h"

class ReportListView;
class FireResultListView;
class ReportFilterOptions;
class Controllers;
class ItemFactory_ABC;

// =============================================================================
/** @class  ReportPanel
    @brief  ReportPanel
*/
// Created: AGE 2005-04-21
// =============================================================================
class ReportPanel : public InfoPanel_ABC
                  , public Observer_ABC
                  , public AgentSelectionObserver
{
public:
    //! @name Constructors/Destructor
    //@{
             ReportPanel( InfoPanels* pParent, Controllers& controllers, ItemFactory_ABC& factory);
    virtual ~ReportPanel();
    //@}

private:
    //! @name Operations
    //@{
    virtual void NotifySelected( const Agent_ABC* element );
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
    Controllers& controllers_;
    const Agent_ABC*     selected_; // $$$$ AGE 2006-04-20: watch deletions !
    ReportFilterOptions* pFilterOptions_;
    ReportListView*      pReportListView_;
    FireResultListView*  pFireResultListView_;
    //@}
};

#endif // __ReportPanel_h_
