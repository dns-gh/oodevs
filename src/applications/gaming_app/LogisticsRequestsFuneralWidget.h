// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __LogisticsRequestsFuneralWidget_h_
#define __LogisticsRequestsFuneralWidget_h_

#include "LogisticConsignsWidget.h"
#include "gaming/LogFuneralConsign.h"

class LogFuneralConsigns;
class SimulationController;
class Model;

// =============================================================================
/** @class  LogisticsRequestsFuneralWidget
    @brief  LogisticsRequestsFuneralWidget
*/
// Created: MMC 2013-09-16
// =============================================================================
class LogisticsRequestsFuneralWidget : public LogisticConsignsWidget< LogFuneralConsigns, LogFuneralConsign >
{
public:
             LogisticsRequestsFuneralWidget( QWidget* parent, kernel::Controllers& controllers,
                                             gui::DisplayExtractor& extractor, const kernel::Profile_ABC& profile,
                                             const SimulationController& simulationController, Model& model );
    virtual ~LogisticsRequestsFuneralWidget();

private:
    virtual void OnRequestSelected( const LogisticsConsign_ABC& consign );
};

#endif // __LogisticsRequestsFuneralWidget_h_
