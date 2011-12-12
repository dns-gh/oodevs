// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ProfileFactory_h_
#define __ProfileFactory_h_

#include "ProfileFactory_ABC.h"

namespace kernel
{
    class Controller;
}

class Model;

// =============================================================================
/** @class  ProfileFactory
    @brief  ProfileFactory
*/
// Created: SBO 2007-01-16
// =============================================================================
class ProfileFactory : public ProfileFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ProfileFactory( kernel::Controller& controller, const Model& model );
    virtual ~ProfileFactory();
    //@}

    //! @name Operations
    //@{
    virtual UserProfile* Create( xml::xistream& xis ) const;
    virtual UserProfile* Create( const QString& name ) const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const Model& model_;
    //@}
};

#endif // __ProfileFactory_h_
