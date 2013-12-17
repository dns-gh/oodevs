// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __OrbatPanel_h_
#define __OrbatPanel_h_

#include "clients_gui/PreferencePanel_ABC.h"
#include "clients_kernel/OptionsObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class OptionVariant;
}

namespace gui
{
    class ColorButton;

// =============================================================================
/** @class  OrbatPanel
    @brief  Orbat panel
*/
// Created: LGY 2012-03-22
// =============================================================================
class OrbatPanel : public gui::PreferencePanel_ABC
                 , public tools::Observer_ABC
                 , public kernel::OptionsObserver_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             OrbatPanel( QWidget* parent, kernel::Controllers& controllers );
    virtual ~OrbatPanel();
    //@}

    //! @name Operations
    //@{
    virtual void Commit();
    virtual void Reset();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    gui::ColorButton* pNColor_;
    gui::ColorButton* pADColor_;
    gui::ColorButton* pODColor_;
    gui::ColorButton* pHealthyColor_;
    gui::ColorButton* pContaminatedColor_;
    gui::ColorButton* pWoundedColor_;
    gui::ColorButton* pDeadColor_;
    gui::ColorButton* pMostlyHealthyColor_;
    //@}
};

}

#endif // __OrbatPanel_h_
