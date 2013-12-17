// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticsRequestsSupplyWidget_h_
#define __LogisticsRequestsSupplyWidget_h_

#include "LogisticConsignsWidget.h"
#include "gaming/LogSupplyConsign.h"

class LogSupplyConsigns;
class LogisticsRequestsSupplyTable;
class Publisher_ABC;
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
    //! @name Constructors/Destructor
    //@{
             LogisticsRequestsSupplyWidget( QWidget* parent, kernel::Controllers& controllers, gui::DisplayExtractor& extractor,
                                            const kernel::Profile_ABC& profile, Publisher_ABC& publisher, Model& model );
    virtual ~LogisticsRequestsSupplyWidget();
    //@}

public:
    //! @name Operations
    //@{
    virtual void Purge();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void PurgeDetail();
    virtual void DisplayRequest( const LogisticsConsign_ABC& consign );
    virtual void OnRequestSelected( const LogisticsConsign_ABC& consign );
    QString GetRecipientsLinks( const LogSupplyConsign& consign );
    //@}

protected:
    //! @name Data Members
    //@{
    LogisticsRequestsSupplyTable* supplyTable_;
    //@}
};

#endif // __LogisticsRequestsSupplyWidget_h_
