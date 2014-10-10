// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __VisualisationScalesPanel_h_
#define __VisualisationScalesPanel_h_

#include "PreferencePanel_ABC.h"

namespace kernel
{
    class OptionsController;
}

namespace gui
{

// =============================================================================
/** @class  VisualisationScalesPanel
    @brief  VisualisationScalesPanel
*/
// Created: JSR 2010-06-14
// =============================================================================
class VisualisationScalesPanel : public PreferencePanel_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             VisualisationScalesPanel( QWidget* parent, kernel::OptionsController& options );
    virtual ~VisualisationScalesPanel();
    //@}

    //! @name Operations
    //@{
    virtual void Load( const GlProxy& );
    //@}

private:
    //! @name Types
    //@{
    typedef struct Scale
    {
        int min_;
        int max_;
    };
    //@}

private:
    //! @name Member data
    //@{
    kernel::OptionsController& options_;
    Scale currentScales_[ 14 ];
    QComboBox* minCombos_[ 14 ];
    QComboBox* maxCombos_[ 14 ];
    //@}
};

} //! namespace gui

#endif // __VisualisationScalesPanel_h_
