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

namespace kernel
{
    class CoordinateConverter_ABC;
    class OptionsController;
}

namespace gui
{

// =============================================================================
/** @class  CoordinateSystemsPanel
    @brief  CoordinateSystemsPanel
*/
// Created: AME 2010-03-15
// =============================================================================
class CoordinateSystemsPanel : public PreferencePanel_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             CoordinateSystemsPanel( QWidget* parent,
                                     kernel::OptionsController& options,
                                     kernel::CoordinateConverter_ABC& coordConverter );
    virtual ~CoordinateSystemsPanel();
    //@}

    //! @name Operations
    //@{
    virtual void Load( const GlProxy& );
    //@}

private:
    //! @name Member data
    //@{
     kernel::CoordinateConverter_ABC& coordConverter_;
     QComboBox* coordSysComboBox_;
    //@}
};

} //! namespace gui

#endif // __CoordinateSystemsPanel_h_
