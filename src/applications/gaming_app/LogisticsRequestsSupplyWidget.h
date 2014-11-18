// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __LogisticsRequestsSupplyWidget_h_
#define __LogisticsRequestsSupplyWidget_h_

#include "LogisticConsignsWidget.h"
#include "gaming/LogSupplyConsign.h"

class LogSupplyConsigns;
class LogisticsRequestsSupplyTable;
class SimulationController;
class Model;

// =============================================================================
/** @class  LogisticsRequestsSupplyWidget
    @brief  LogisticsRequestsSupplyWidget
*/
// Created: MMC 2013-09-16
// =============================================================================
class LogisticsRequestsSupplyWidget : public LogisticConsignsWidget< LogSupplyConsigns, LogSupplyConsign >
{
    Q_OBJECT
    
public:
             LogisticsRequestsSupplyWidget( QWidget* parent, kernel::Controllers& controllers, gui::DisplayExtractor& extractor,
                                            const kernel::Profile_ABC& profile, const SimulationController& simulationController, Model& model );
    virtual ~LogisticsRequestsSupplyWidget();

public:
    virtual void Purge();

private:
    virtual void PurgeDetail();
    virtual void OnRequestSelected( const LogisticsConsign_ABC& consign );
    QString GetRecipientsLinks( const LogSupplyConsign& consign, bool link );
    virtual void DisplayHistory( const LogSupplyConsign::History& history );
    void FillSupplyTable( const LogSupplyConsign& consign );
    virtual void NotifyUpdated( const kernel::Entity_ABC& entity );
    virtual bool IsActive( const LogSupplyConsign& consign ) const;

protected:
    LogisticsRequestsSupplyTable* supplyTable_;
};

#endif // __LogisticsRequestsSupplyWidget_h_
