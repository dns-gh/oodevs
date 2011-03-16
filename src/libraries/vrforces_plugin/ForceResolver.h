// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ForceResolver_h_
#define __ForceResolver_h_

#include "ForceResolver_ABC.h"

namespace dispatcher
{
    class Model_ABC;
}

namespace plugins
{
namespace vrforces
{
// =============================================================================
/** @class  ForceResolver
    @brief  Force resolver implementation
*/
// Created: SBO 2011-03-16
// =============================================================================
class ForceResolver : public ForceResolver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ForceResolver( const dispatcher::Model_ABC& model );
    virtual ~ForceResolver();
    //@}

    //! @name Operations
    //@{
    virtual DtForceType Resolve( unsigned long identifier ) const;
    //@}

private:
    //! @name Member data
    //@{
    const dispatcher::Model_ABC& model_;
    //@}
};
}
}

#endif // __ForceResolver_h_
