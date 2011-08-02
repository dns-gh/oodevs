// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __OptionsPanel_h_
#define __OptionsPanel_h_

#include "clients_kernel/OptionsObserver_ABC.h"
#include "PreferencePanel_ABC.h"

namespace kernel
{
    class Controllers;
    class Options;
}

namespace gui
{
// =============================================================================
// Created: APE 2004-07-09
// =============================================================================
class OptionsPanel : public PreferencePanel_ABC
                   , public tools::Observer_ABC
                   , public kernel::OptionsObserver_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             OptionsPanel( QWidget* pParent, kernel::Controllers& controllers );
    virtual ~OptionsPanel();
    //@}

    //! @name Operations
    //@{
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
    //@}

private:
    //! @name Copy / Assignment
    //@{
    OptionsPanel( const OptionsPanel& );
    OptionsPanel& operator=( const OptionsPanel& );
    //@}

private slots:
    //! @name Helpers
    //@{
    void Change3dDistanceOption( bool );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    kernel::Options&     options_;

    QCheckBox* pCompute3dDistance_;

    //@}
};

}

#endif // __OptionsPanel_h_
