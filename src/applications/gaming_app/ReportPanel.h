// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ReportPanel_h_
#define __ReportPanel_h_

#include "clients_gui/InfoPanel_ABC.h"
#include "clients_kernel/Observer_ABC.h"
#include "gaming/AgentSelectionObserver.h"

namespace kernel
{
    class Controllers;
}

namespace gui
{
    class ItemFactory_ABC;
}

class ReportListView;
class FireResultListView;
class ReportFilterOptions;

// =============================================================================
/** @class  ReportPanel
    @brief  ReportPanel
*/
// Created: AGE 2005-04-21
// =============================================================================
class ReportPanel : public gui::InfoPanel_ABC
                  , public kernel::Observer_ABC
                  , public AgentSelectionObserver
{
public:
    //! @name Constructors/Destructor
    //@{
             ReportPanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory);
    virtual ~ReportPanel();
    //@}

private:
    //! @name Operations
    //@{
    virtual void NotifySelected( const kernel::Entity_ABC* element );
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
    kernel::Controllers& controllers_;
    const kernel::Entity_ABC*     selected_; // $$$$ AGE 2006-04-20: watch deletions !
    ReportFilterOptions* pFilterOptions_;
    ReportListView*      pReportListView_;
    FireResultListView*  pFireResultListView_;
    //@}
};

#endif // __ReportPanel_h_
