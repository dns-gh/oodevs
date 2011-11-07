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

#include "PreferencePanel_ABC.h"
#include "clients_kernel/OptionsObserver_ABC.h"
#include <boost/noncopyable.hpp>

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
    @brief  OrbatPanel
*/
// Created: LGY 2011-10-21
// =============================================================================
class OrbatPanel : public PreferencePanel_ABC
                 , public tools::Observer_ABC
                 , public kernel::OptionsObserver_ABC
                 , private boost::noncopyable
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
    ColorButton* pNColor_;
    ColorButton* pADColor_;
    ColorButton* pODColor_;
    //@}
};

}
#endif // __OrbatPanel_h_
