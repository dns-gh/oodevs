// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef gui_InhabitantPanel_h
#define gui_InhabitantPanel_h

#include "PreferencePanel_ABC.h"
#include "clients_kernel/OptionsObserver_ABC.h"
#include "tools/Observer_ABC.h"

namespace kernel
{
    class Controllers;
    class Options;
    class OptionVariant;
    class OptionsObserver_ABC;
}

namespace gui
{
    class CheckBox;
    class DensityWidget;

// =============================================================================
/** @class  InhabitantPanel
    @brief  Inhabitant panel
*/
// Created: LGY 2010-12-31
// =============================================================================
class InhabitantPanel : public PreferencePanel_ABC
                      , public tools::Observer_ABC
                      , public kernel::OptionsObserver_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             InhabitantPanel( QWidget* parent, kernel::Controllers& controllers );
    virtual ~InhabitantPanel();
    //@}

    //! @name Operations
    //@{
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnChanged( bool value );
    //@}

private:
    //! @name Member Data
    //@{
    kernel::Options& options_;
    CheckBox* density_;
    DensityWidget* widget_;
    bool loaded_;
    //@}
};

}

#endif // gui_InhabitantPanel_h
