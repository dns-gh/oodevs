// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __WeaponRangesPanel_h_
#define __WeaponRangesPanel_h_

#include "PreferencePanel_ABC.h"

namespace kernel
{
    class OptionsController;
    class StaticModel;
    class VolumeType;
}

namespace gui
{

// =============================================================================
/** @class  WeaponRangesPanel
    @brief  WeaponRangesPanel
*/
// Created: MCO 2014-08-13
// =============================================================================
class WeaponRangesPanel : public PreferencePanel_ABC
{
    Q_OBJECT

public:
             WeaponRangesPanel( QWidget* parent,
                                kernel::OptionsController& options,
                                const kernel::StaticModel& model );
    virtual ~WeaponRangesPanel();

    virtual void Load( const GlProxy& );

private:
    kernel::OptionsController& options_;
    const kernel::StaticModel& model_;
    QComboBox* volumeCombo_;
    QComboBox* indirectWeaponCombo_;
};

} //! namespace gui

#endif // __WeaponRangesPanel_h_
