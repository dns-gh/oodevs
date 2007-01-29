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
             Terrain();
    virtual ~Terrain();
    //@}

    //! @name Operations
    //@{
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
    //@}
};

}

#endif // __Terrain_h_
