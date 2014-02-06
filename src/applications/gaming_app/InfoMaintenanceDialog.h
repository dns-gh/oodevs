// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __InfoMaintenanceDialog_h_
#define __InfoMaintenanceDialog_h_

#include "InfoDialog.h"
#include "LogisticDialog_ABC.h"
#include "clients_kernel/MaintenanceStates_ABC.h"
#include "gaming/Equipments.h"
#include <boost/optional.hpp>

namespace kernel
{
    class Controllers;
    class Profile_ABC;
}

namespace gui
{
    class ItemFactory_ABC;
    class DisplayExtractor;
    class LogisticBase;
}

class SimulationController;
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
                            , public tools::ElementObserver_ABC< gui::LogisticBase >
                            , public LogisticDialog_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             InfoMaintenanceDialog( QWidget* parent, kernel::Controllers& controllers,
                                    gui::DisplayExtractor& extractor, const kernel::Profile_ABC& profile,
                                    const SimulationController& simulationController, Model& model );
    virtual ~InfoMaintenanceDialog();
    //@}

public:
    //! @name Operations
    //@{
    virtual void Purge();
    virtual void FillCurrentModel( const kernel::Entity_ABC& entity );
    virtual void FillHistoryModel();
    virtual bool IsVisible() const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual bool ShouldDisplay( const kernel::Entity_ABC& element ) const;
    virtual void NotifySelected( const kernel::Entity_ABC* entity );
    virtual void NotifyUpdated( const Equipments& entity );
    virtual void NotifyUpdated( const gui::LogisticBase& ext );
    virtual void UpdateTitle( boost::optional< bool > manual );
    //@}

private:
    //! @name Member data
    //@{
    LogisticsRequestsMaintenanceWidget* widget_;
    //@}
};

#endif // __InfoMaintenanceDialog_h_
