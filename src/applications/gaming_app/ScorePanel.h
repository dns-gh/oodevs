// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __ScorePanel_h_
#define __ScorePanel_h_

#include "tools/ElementObserver_ABC.h"

namespace gui
{
    class LinkInterpreter_ABC;
}

namespace kernel
{
    class Controllers;
    class DisplayExtractor_ABC;
    class ModelUnLoaded;
}

namespace tools
{
    class ExerciseConfig;
}

class IndicatorRequest;
class IndicatorPlotFactory;
class Score;
class IndicatorExportDialog;
class ScoreModel;

// =============================================================================
/** @class  ScorePanel
    @brief  ScorePanel
*/
// Created: SBO 2009-03-12
// =============================================================================
class ScorePanel : public QDockWidget
                 , public tools::Observer_ABC
                 , public tools::ElementObserver_ABC< Score >
                 , public tools::ElementObserver_ABC< kernel::ModelUnLoaded >
                 , public tools::ElementObserver_ABC< IndicatorRequest >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ScorePanel( QMainWindow* mainWindow, kernel::Controllers& controllers, kernel::DisplayExtractor_ABC& extractor, gui::LinkInterpreter_ABC& interpreter, IndicatorPlotFactory& plotFactory, IndicatorExportDialog& exportDialog, ScoreModel& model, const tools::ExerciseConfig& config );
    virtual ~ScorePanel();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnShowGraph();
    void OnExportData();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyCreated( const Score& element );
    virtual void NotifyUpdated( const Score& element );
    virtual void NotifyDeleted( const Score& element );
    virtual void NotifyUpdated( const kernel::ModelUnLoaded& );
    virtual void NotifyUpdated( const IndicatorRequest& request );
    void Display( const Score& score, QTreeWidgetItem* item );
    //@}

    //! @name Types
    //@{
    typedef std::vector< const IndicatorRequest* > T_PendingRequests;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    kernel::DisplayExtractor_ABC& extractor_;
    IndicatorPlotFactory& plotFactory_;
    ScoreModel& model_;
    IndicatorExportDialog& exportDialog_;
    QDialog* reportDialog_;
    QTreeWidget* scores_;
    T_PendingRequests graphRequests_;
    T_PendingRequests exportRequests_;
    //@}
};

#endif // __ScorePanel_h_
