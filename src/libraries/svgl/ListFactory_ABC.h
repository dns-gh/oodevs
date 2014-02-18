// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ListFactory_ABC_h_
#define __ListFactory_ABC_h_

#include "PropertyFactory_ABC.h"
#include "BoundingBox.h"
#include <map>

namespace svg
{
    class RenderingContext_ABC;

// =============================================================================
/** @class  ListFactory_ABC
    @brief  ListFactory_ABC
*/
// Created: AGE 2007-10-30
// =============================================================================
class ListFactory_ABC : public PropertyFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ListFactory_ABC();
    virtual ~ListFactory_ABC();
    //@}

    //! @name Operations
    //@{
    void SetViewport( const geometry::BoundingBox& viewport, unsigned w, unsigned h );
    virtual Property_ABC* CreateProperty( const std::string& content ) const;
    //@}

protected:
    //! @name Operations
    //@{
    virtual bool IsFixed( const std::string& content ) const = 0;
    virtual void CompileList( const std::string& content, RenderingContext_ABC& context ) = 0;
    virtual Property_ABC* CreateBaseProperty( const std::string& content ) const = 0;
    virtual Property_ABC* CreateListProperty( unsigned list ) const = 0;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ListFactory_ABC( const ListFactory_ABC& );            //!< Copy constructor
    ListFactory_ABC& operator=( const ListFactory_ABC& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    typedef std::map< std::string, unsigned > T_Lists;
    typedef T_Lists::const_iterator         CIT_Lists;
    //@}

private:
    //! @name Member data
    //@{
    mutable T_Lists lists_;
    //@}
};

}

#endif // __ListFactory_ABC_h_
