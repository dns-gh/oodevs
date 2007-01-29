// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TerrainFactory_h_
#define __TerrainFactory_h_

#include "TerrainFactory_ABC.h"

namespace kernel
{
    class Controllers;
}

namespace frontend
{

// =============================================================================
/** @class  TerrainFactory
    @brief  TerrainFactory
*/
// Created: SBO 2007-01-29
// =============================================================================
class TerrainFactory : public TerrainFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit TerrainFactory( kernel::Controllers& controllers );
    virtual ~TerrainFactory();
    //@}

    //! @name Operations
    //@{
    virtual Terrain* Create();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TerrainFactory( const TerrainFactory& );            //!< Copy constructor
    TerrainFactory& operator=( const TerrainFactory& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    //@}
};

}

#endif // __TerrainFactory_h_
