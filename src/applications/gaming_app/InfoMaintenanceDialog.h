// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __InfoMaintenanceDialog_h_
#define __InfoMaintenanceDialog_h_

#include "InfoDialog.h"
#include "LogisticDialog_ABC.h"
#include "clients_kernel/MaintenanceStates_ABC.h"
#include "gaming/Equipments.h"

namespace kernel
{
    class Controllers;
    class Profile_ABC;
}

namespace gui
{
    class ItemFactory_ABC;
    class DisplayExtractor;
}

class Publisher_ABC;
class LogisticsRequestsMaintenanceWidget;
class Model;

// =============================================================================
/** @class  InfoMaintenanceDialog
    @brief  InfoMaintenanceDialog
*/
// Created: SBO 2007-02-20
// =============================================================================
class InfoMaintenanceDialog : public InfoDialog< kernel::MaintenanceStates_ABC >
                            , public tools::ElementObserver_ABC< Equipments >
                            , public LogisticDialog_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             InfoMaintenanceDialog( QWidget* parent, kernel::Controllers& controllers,
                                    gui::DisplayExtractor& extractor, const kernel::Profile_ABC& profile, Publisher_ABC& publisher,
                                    Model& model );
    virtual ~InfoMaintenanceDialog();
    //@}

public:
    //! @name Operations
    //@{
    virtual void Purge();
    virtual void Fill( const kernel::Entity_ABC& entity );
    //@}

private:
    //! @name Helpers
    //@{
    virtual bool ShouldDisplay( const kernel::Entity_ABC& element ) const;
    virtual void NotifySelected( const kernel::Entity_ABC* entity );
    virtual void NotifyUpdated( const Equipments& entity );
    //@}

private:
    //! @name Member data
    //@{
    LogisticsRequestsMaintenanceWidget* widget_;
    //@}
};

#endif // __InfoMaintenanceDialog_h_
