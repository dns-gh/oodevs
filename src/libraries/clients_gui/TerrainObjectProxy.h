// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TerrainObjectProxy_h_
#define __TerrainObjectProxy_h_

#include "clients_kernel/Extension_ABC.h"

namespace urban
{
    class TerrainObject_ABC;
}

namespace gui
{

// =============================================================================
/** @class  UrbanModel
@brief  UrbanModel
*/
// Created: SLG 2009-02-10
// =============================================================================
class TerrainObjectProxy : public kernel::Extension_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit TerrainObjectProxy( urban::TerrainObject_ABC& object ); 
    virtual ~TerrainObjectProxy();
    //@}


public:
    //! @name Member data
    //@{
    urban::TerrainObject_ABC* object_;
    //@}

public:
    //! @name Copy/Assignement
    //@{
    TerrainObjectProxy( const TerrainObjectProxy& );            //!< Copy constructor
    TerrainObjectProxy& operator=( const TerrainObjectProxy& ); //!< Assignement operator
    bool operator==( const TerrainObjectProxy& ) const;
    //@}
};

}

#endif // __TerrainObjectProxy_h_
