// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MaintenanceConsignsWidget_h_
#define __MaintenanceConsignsWidget_h_

#include "gaming/LogMaintenanceConsign.h"
#include "LogisticConsignsWidget_ABC.h"

// =============================================================================
/** @class  MaintenanceConsignsWidget
    @brief  MaintenanceConsignsWidget
*/
// Created: SBO 2007-02-19
// =============================================================================
class MaintenanceConsignsWidget : public LogisticConsignsWidget_ABC< MaintenanceConsignsWidget, LogMaintenanceConsign, LogMaintenanceConsigns >
{

public:
    //! @name Constructors/Destructor
    //@{
             MaintenanceConsignsWidget( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~MaintenanceConsignsWidget();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MaintenanceConsignsWidget( const MaintenanceConsignsWidget& );            //!< Copy constructor
    MaintenanceConsignsWidget& operator=( const MaintenanceConsignsWidget& ); //!< Assignment operator
    //@}
};

#endif // __MaintenanceConsignsWidget_h_
