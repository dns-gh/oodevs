// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __RefreshRatePanel_h_
#define __RefreshRatePanel_h_

#include "PreferencePanel_ABC.h"
#include "clients_kernel/OptionsObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class OptionsController;
}

namespace gui
{
    class RichSpinBox;
// =============================================================================
/** @class  RefreshRatePanel
    @brief  RefreshRatePanel
*/
// Created: LDC 2014-02-21
// =============================================================================
class RefreshRatePanel : public PreferencePanel_ABC
                       , public tools::Observer_ABC
                       , public kernel::OptionsObserver_ABC
{
    Q_OBJECT
public:
    //! @name Constructors/Destructor
    //@{
             RefreshRatePanel( QWidget* parent, kernel::Controllers& controllers );
    virtual ~RefreshRatePanel();
    //@}

    //! @name Operations
    //@{
    virtual void Reset();
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
    //@}

private:
    //! @name Member data
    //@{
     kernel::OptionsController& options_;
     kernel::Controllers& controllers_;
     gui::RichSpinBox* spinBox_;
    //@}
};
}
#endif // __RefreshRatePanel_h_
