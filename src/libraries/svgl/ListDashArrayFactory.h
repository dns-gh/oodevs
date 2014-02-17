// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ListDashArrayFactory_h_
#define __ListDashArrayFactory_h_

#include "ListFactory_ABC.h"
#include "BoundingBox.h"

namespace svg
{

// =============================================================================
/** @class  ListDashArrayFactory
    @brief  ListDashArrayFactory
*/
// Created: AGE 2007-10-30
// =============================================================================
class ListDashArrayFactory : public ListFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ListDashArrayFactory();
    virtual ~ListDashArrayFactory();
    //@}

protected:
    //! @name Operations
    //@{
    virtual bool IsFixed( const std::string& content ) const;
    virtual void CompileList( const std::string& content, RenderingContext_ABC& context );
    virtual Property_ABC* CreateBaseProperty( const std::string& content ) const;
    virtual Property_ABC* CreateListProperty( unsigned list ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ListDashArrayFactory( const ListDashArrayFactory& );            //!< Copy constructor
    ListDashArrayFactory& operator=( const ListDashArrayFactory& ); //!< Assignment operator
    //@}
};

}

#endif // __ListDashArrayFactory_h_
