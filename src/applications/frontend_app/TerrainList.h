// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TerrainList_h_
#define __TerrainList_h_

#include "frontend/Terrain.h"
#include "ElementListView.h"

namespace kernel
{
    class Controllers;
}

// =============================================================================
/** @class  TerrainList
    @brief  TerrainList
*/
// Created: SBO 2007-02-01
// =============================================================================
class TerrainList : public ElementListView< frontend::Terrain >
{

public:
    //! @name Constructors/Destructor
    //@{
             TerrainList( QWidget* parent, kernel::Controllers& controllers );
    virtual ~TerrainList();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TerrainList( const TerrainList& );            //!< Copy constructor
    TerrainList& operator=( const TerrainList& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __TerrainList_h_
