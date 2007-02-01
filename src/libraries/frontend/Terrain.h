// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Terrain_h_
#define __Terrain_h_

#include "Messages.h"

namespace kernel
{
    class Controller;
}

namespace frontend
{

// =============================================================================
/** @class  Terrain
    @brief  Terrain
*/
// Created: SBO 2007-01-29
// =============================================================================
class Terrain
{

public:
    //! @name Constructors/Destructor
    //@{
             Terrain( const ASN1T_MsgTerrainCreation& message, kernel::Controller& controller );
    virtual ~Terrain();
    //@}

    //! @name Operations
    //@{
    QString GetName() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Terrain( const Terrain& );            //!< Copy constructor
    Terrain& operator=( const Terrain& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    QString name_;
    //@}
};

}

#endif // __Terrain_h_
