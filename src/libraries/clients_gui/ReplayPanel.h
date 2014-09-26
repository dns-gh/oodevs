// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef ReplayPanel_h
#define ReplayPanel_h

#include "PreferencePanel_ABC.h"
#include "clients_kernel/OptionsObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class OptionsController;
}

namespace gui
{
// =============================================================================
/** @class  ReplayPanel
    @brief  ReplayPanel
*/
// Created: SLI 2014-06-06
// =============================================================================
class ReplayPanel : public PreferencePanel_ABC
                  , public tools::Observer_ABC
                  , public kernel::OptionsObserver_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ReplayPanel( QWidget* parent, kernel::Controllers& controllers );
    virtual ~ReplayPanel();
    //@}

    //! @name Operations
    //@{
    virtual void Commit();
    virtual void Reset();
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
    //@}

private:
    //! @name Member data
    //@{
     kernel::OptionsController& options_;
     kernel::Controllers& controllers_;
     QCheckBox* checkbox_;
    //@}
};

}

#endif // ReplayPanel_h
