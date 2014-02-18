// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Node_h_
#define __Node_h_

#include "Node_ABC.h"
#include <vector>

namespace xml
{
    class xistream;
}

namespace svg 
{
    class RenderingContext_ABC;
    class PropertyGroup;
    class PropertyFactory;
    class Transformations;
    class References_ABC;

// =============================================================================
/** @class  Node
    @brief  Node
*/
// Created: AGE 2006-08-10
// =============================================================================
class Node : public Node_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Node( xml::xistream& input, References_ABC& references, const PropertyFactory& factory );
    virtual ~Node();
    //@}

    //! @name Operations
    //@{
    virtual Node& Compile( RenderingContext_ABC& context, References_ABC& references ) const;
    virtual void  Draw( RenderingContext_ABC& context, References_ABC& references ) const;
    //@}

    //! @name Accessors
    //@{
    const std::string& GetId() const;
    //@}

protected:
    //! @name Helpers
    //@{
    Node( const Node& );            //!< Copy constructor
    virtual void DrawInternal( RenderingContext_ABC& context, References_ABC& references ) const = 0;
    virtual Node& CompileInternal( RenderingContext_ABC& context, References_ABC& references ) const = 0;
    //@} 

private:
    //! @name Copy/Assignement
    //@{
    Node& operator=( const Node& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    std::string id_;
    PropertyGroup* properties_;
    Transformations* transformations_;
    //@}
};

}

#endif // __Node_h_
