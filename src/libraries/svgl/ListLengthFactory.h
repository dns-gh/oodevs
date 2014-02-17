// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ListLengthFactory_h_
#define __ListLengthFactory_h_

#include "ListFactory_ABC.h"

namespace svg
{

// =============================================================================
/** @class  ListLengthFactory
    @brief  ListLengthFactory
*/
// Created: ZEBRE 2007-05-24
// =============================================================================
class ListLengthFactory : public ListFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ListLengthFactory();
    virtual ~ListLengthFactory();
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
    ListLengthFactory( const ListLengthFactory& );            //!< Copy constructor
    ListLengthFactory& operator=( const ListLengthFactory& ); //!< Assignment operator
    //@}
};

}

#endif // __ListLengthFactory_h_
