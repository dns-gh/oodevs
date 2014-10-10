// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __PresetDialog_h_
#define __PresetDialog_h_

namespace kernel
{
    class OptionsController;
}

namespace gui
{
    class Gradient;
    class GradientPreferences;

// =============================================================================
/** @class  PresetDialog
    @brief  PresetDialog
*/
// Created: LGY 2010-09-24
// =============================================================================
class PresetDialog : public QDialog
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             PresetDialog( QWidget* parent,
                           kernel::OptionsController& options,
                           const std::shared_ptr< Gradient >& gradient,
                           GradientPreferences& preferences );
    virtual ~PresetDialog();
    //@}
};

} //! namespace gui

#endif // __PresetDialog_h_
