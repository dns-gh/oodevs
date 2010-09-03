// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ResourceTools_h_
#define __ResourceTools_h_

#include "resource_network/ResourceTools_ABC.h"

// =============================================================================
/** @class  ResourceTools
    @brief  ResourceTools
*/
// Created: JSR 2010-09-03
// =============================================================================
class ResourceTools : public resource::ResourceTools_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ResourceTools();
    virtual ~ResourceTools();
    //@}

    //! @name Operations
    //@{
    virtual const std::string& GetResourceName( unsigned long id ) const;
    virtual unsigned long GetResourceId( const std::string& name ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ResourceTools( const ResourceTools& );            //!< Copy constructor
    ResourceTools& operator=( const ResourceTools& ); //!< Assignment operator
    //@}
};

#endif // __ResourceTools_h_
