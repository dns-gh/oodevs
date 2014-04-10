// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MiscLayer_h_
#define __MiscLayer_h_

#include "Layer.h"

namespace gui
{
// =============================================================================
/** @class  MiscLayer
    @brief  MiscLayer
    // $$$$ AGE 2006-06-30: renommer ou le fichier, ou supprimer
*/
// Created: AGE 2006-03-31
// =============================================================================
template< typename Panel >
class MiscLayer : public Layer
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MiscLayer( Panel& panel ) : panel_( panel ) {}
    virtual ~MiscLayer() {}
    //@}

    //! @name Operations
    //@{
    virtual void Paint( Viewport_ABC& viewport )
    {
        panel_.Draw( viewport );
    }
    //@}

private:
    //! @name Member data
    //@{
    Panel& panel_;
    //@}
};

}

#endif // __MissionLayer_h_
