// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Container_h_
#define __Container_h_

#include "Node.h"

namespace svg
{
    class Factory_ABC;

// =============================================================================
/** @class  Container
    @brief  Container
*/
// Created: AGE 2006-08-29
// =============================================================================
class Container : public Node
{

public:
    //! @name Constructors/Destructor
    //@{
             Container( xml::xistream& input, const Factory_ABC& factory, References_ABC& references, const PropertyFactory& propertyFactory );
    virtual ~Container();
    //@}

protected:
    //! @name Helpers
    //@{
    Container( const Container& );            //!< Copy constructor
    void DrawChildren( RenderingContext_ABC& context, References_ABC& references ) const;
    Container& CompileChildren( Container& newNode, RenderingContext_ABC& context, References_ABC& references ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Container& operator=( const Container& ); //!< Assignement operator
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< Node_ABC* >     T_Children;
    typedef T_Children::const_iterator CIT_Children;
    //@}

    //! @name Helpers
    //@{
    void ReadChild( const std::string& name, xml::xistream& input, const Factory_ABC& factory, References_ABC& references );
    //@}

private:
    //! @name Member data
    //@{
    T_Children children_;
    //@}
};

}

#endif // __Container_h_
