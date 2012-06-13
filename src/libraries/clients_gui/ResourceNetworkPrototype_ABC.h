// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ResourceNetworkPrototype_ABC_h_
#define __ResourceNetworkPrototype_ABC_h_

#include "ObjectAttributePrototype_ABC.h"

namespace gui {

// =============================================================================
/** @class  ResourceNetworkPrototype_ABC
    @brief  ResourceNetworkPrototype_ABC
*/
// Created: JSR 2011-02-23
// =============================================================================
class ResourceNetworkPrototype_ABC : public ObjectAttributePrototype_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ResourceNetworkPrototype_ABC( QWidget* parent );
    virtual ~ResourceNetworkPrototype_ABC();
    //@}

    //! @name Operations
    //@{
    virtual bool CheckValidity( const kernel::Team_ABC& ) const;
    //@}
};

} // end namespace gui

#endif // __ResourceNetworkPrototype_ABC_h_
