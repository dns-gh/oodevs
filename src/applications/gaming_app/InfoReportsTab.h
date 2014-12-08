// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __InfoReportsTab_h_
#define __InfoReportsTab_h_

namespace gui
{
    class DisplayExtractor;
}

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class Time_ABC;
}

class AgentsModel;
class Publisher_ABC;
class ReportFactory;
class ReportListView;
class ReportsModel;

// =============================================================================
/** @class  InfoReportsTab
    @brief  InfoReportsTab
*/
// Created: SBO 2007-02-06
// =============================================================================
class InfoReportsTab : public Q3VBox
{
public:
    //! @name Constructors/Destructor
    //@{
             InfoReportsTab( QTabWidget* parent, kernel::Controllers& controllers, gui::DisplayExtractor& extractor,
                             const ReportFactory& factory, Publisher_ABC& publisher, ReportsModel& model,
                             const AgentsModel& agents, const kernel::Time_ABC& time );
    virtual ~InfoReportsTab();
    //@}

    void Purge();

private:
    //! @name Member data
    //@{
    ReportListView* reports_;
    //@}
};

#endif // __InfoReportsTab_h_
