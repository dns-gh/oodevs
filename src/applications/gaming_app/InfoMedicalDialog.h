// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __InfoMedicalDialog_h_
#define __InfoMedicalDialog_h_

#include "InfoDialog.h"
#include "LogisticDialog_ABC.h"
#include "gaming/MedicalStates.h"
#include "gaming/Troops.h"

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

class SimulationController;
class MedicalStatusWidget;
class LogisticsRequestsMedicalWidget;
class Model;

// =============================================================================
/** @class  InfoMedicalDialog
    @brief  InfoMedicalDialog
*/
// Created: SBO 2007-02-20
// =============================================================================
class InfoMedicalDialog : public InfoDialog< MedicalStates >
                        , public tools::ElementObserver_ABC< Troops >
                        , public LogisticDialog_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             InfoMedicalDialog( QWidget* parent, kernel::Controllers& controllers,
                                gui::DisplayExtractor& extractor, const kernel::Profile_ABC& profile,
                                const SimulationController& simulationController, Model& model );
    virtual ~InfoMedicalDialog();
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
    LogisticsRequestsMedicalWidget* widget_;
    //@}
};

#endif // __InfoMedicalDialog_h_
