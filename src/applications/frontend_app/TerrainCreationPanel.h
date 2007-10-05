// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TerrainCreationPanel_h_
#define __TerrainCreationPanel_h_

#include "Panel_ABC.h"

// =============================================================================
/** @class  TerrainCreationPanel
    @brief  TerrainCreationPanel
*/
// Created: SBO 2007-10-04
// =============================================================================
class TerrainCreationPanel : public Panel_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             TerrainCreationPanel( QWidgetStack* widget, QAction& action );
    virtual ~TerrainCreationPanel();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TerrainCreationPanel( const TerrainCreationPanel& );            //!< Copy constructor
    TerrainCreationPanel& operator=( const TerrainCreationPanel& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __TerrainCreationPanel_h_
