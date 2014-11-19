// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticsRequestsMaintenanceWidget_h_
#define __LogisticsRequestsMaintenanceWidget_h_

#include "LogisticConsignsWidget.h"
#include "gaming/LogMaintenanceConsign.h"

class LogMaintenanceConsigns;
class SimulationController;
class Model;

// =============================================================================
/** @class  LogisticsRequestsMaintenanceWidget
    @brief  LogisticsRequestsMaintenanceWidget
*/
// Created: MMC 2013-09-16
// =============================================================================
class LogisticsRequestsMaintenanceWidget : public LogisticConsignsWidget< LogMaintenanceConsigns, LogMaintenanceConsign >
{
public:
             LogisticsRequestsMaintenanceWidget( QWidget* parent, kernel::Controllers& controllers,
                                                 gui::DisplayExtractor& extractor, const kernel::Profile_ABC& profile,
                                                 const SimulationController& simulationController, Model& model );
    virtual ~LogisticsRequestsMaintenanceWidget();

private:
    virtual bool IsActive( const LogMaintenanceConsign& consign ) const;
    virtual void OnRequestSelected( const LogisticsConsign_ABC& consign );
    QString GetBreakdown( const LogMaintenanceConsign& consign );
};

#endif // __LogisticsRequestsMaintenanceWidget_h_
