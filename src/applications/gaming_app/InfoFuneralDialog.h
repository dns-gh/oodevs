// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __InfoFuneralDialog_h_
#define __InfoFuneralDialog_h_

#include "InfoDialog.h"
#include "LogisticDialog_ABC.h"
#include "clients_kernel/SafePointer.h"
#include "gaming/Troops.h"

namespace kernel
{
    class Controllers;
    class Profile_ABC;
}

namespace gui
{
    class DisplayExtractor;
}

class SimulationController;
class LogisticsRequestsFuneralWidget;
class Model;

// =============================================================================
/** @class  InfoFuneralDialog
    @brief  InfoFuneralDialog
*/
// Created: SBO 2007-02-20
// =============================================================================
class InfoFuneralDialog : public InfoDialog_Base
                        , public tools::ElementObserver_ABC< Troops >
                        , public LogisticDialog_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             InfoFuneralDialog( QWidget* parent, kernel::Controllers& controllers, gui::DisplayExtractor& extractor,
                                const kernel::Profile_ABC& profile, const SimulationController& simulationController, Model& model );
    virtual ~InfoFuneralDialog();
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
    virtual void NotifyUpdated( const Troops& troops );
    //@}

private:
    //! @name Member data
    //@{
    LogisticsRequestsFuneralWidget* widget_;
    //@}
};

#endif // __InfoFuneralDialog_h_
