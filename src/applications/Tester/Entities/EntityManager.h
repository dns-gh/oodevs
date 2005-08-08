// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-11 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/EntityManager.h $
// $Author: Sbo $
// $Modtime: 5/07/05 18:01 $
// $Revision: 10 $
// $Workfile: EntityManager.h $
//
// *****************************************************************************

#ifndef __EntityManager_h_
#define __EntityManager_h_

#include "Types.h"

namespace TEST
{

// =============================================================================
/** @class  EntityManager
    @brief  EntityManager
    @par    Using example
    @code
    EntityManager;
    @endcode
*/
// Created: SBO 2005-05-11
// =============================================================================
class EntityManager
{
	MT_COPYNOTALLOWED( EntityManager );

public:
    //! @name Constructors/Destructor
    //@{
             EntityManager();
    virtual ~EntityManager();
    //@}

    //! @name Operations
    //@{
    void Initialize( XmlInputArchive& archive ) const;
    //@}
};


} // end namespace TEST

#endif // __EntityManager_h_
