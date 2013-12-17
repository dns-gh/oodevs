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

namespace gui
{
    class GraphicPreferences;

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
             GraphicsPanel( QWidget* parent, GraphicPreferences& preferences );
    virtual ~GraphicsPanel();
    //@}

    //! @name Operations
    //@{
    virtual void Commit();
    virtual void Reset();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    GraphicsPanel( const GraphicsPanel& );            //!< Copy constructor
    GraphicsPanel& operator=( const GraphicsPanel& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    GraphicPreferences& preferences_;
    //@}
};

}

#endif // __GraphicsPanel_h_
