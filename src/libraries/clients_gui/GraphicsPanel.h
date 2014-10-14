// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __GraphicsPanel_h_
#define __GraphicsPanel_h_

#include "PreferencePanel_ABC.h"

namespace kernel
{
    class OptionsController;
}

namespace gui
{
    class GraphicPreferences;
    class TerrainSettings;

// =============================================================================
/** @class  GraphicsPanel
    @brief  Graphics panel
*/
// Created: SBO 2006-04-04
// =============================================================================
class GraphicsPanel : public PreferencePanel_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             GraphicsPanel( QWidget* parent,
                            kernel::OptionsController& options,
                            const std::shared_ptr< TerrainSettings >& settings );
    virtual ~GraphicsPanel();
    //@}

private:
    //! @name PreferencePanel_ABC implementation
    //@{
    virtual void Load( const GlProxy& );
    //@}

private:
    //! @name Member data
    //@{
    kernel::OptionsController& options_;
    GraphicPreferences* preferences_;
    //@}
};

} //! namespace gui

#endif // __GraphicsPanel_h_
