// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __CoordinateSystemsPanel_h_
#define __CoordinateSystemsPanel_h_

#include "PreferencePanel_ABC.h"
#include "clients_kernel/OptionsObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class CoordinateConverter_ABC;
    class Options;
}

enum E_CoordinateSystem;

namespace gui
{
    template< typename T > class RichWidget;
// =============================================================================
/** @class  CoordinateSystemsPanel
    @brief  CoordinateSystemsPanel
*/
// Created: AME 2010-03-15
// =============================================================================
class CoordinateSystemsPanel : public PreferencePanel_ABC
                             , public tools::Observer_ABC
                             , public kernel::OptionsObserver_ABC
{
    Q_OBJECT
public:
    //! @name Constructors/Destructor
    //@{
             CoordinateSystemsPanel( QWidget* parent,
                                     kernel::Controllers& controllers,
                                     kernel::CoordinateConverter_ABC& coordConverter );
    virtual ~CoordinateSystemsPanel();
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
     kernel::Controllers& controllers_;
     kernel::CoordinateConverter_ABC& coordConverter_;
     kernel::Options& options_;
     RichWidget< QComboBox >* coordSysComboBox_;
     E_CoordinateSystem previousCoordinateSystem_;
    //@}
};

}
#endif // __CoordinateSystemsPanel_h_
