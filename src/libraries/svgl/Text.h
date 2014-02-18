// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Text_h_
#define __Text_h_

#include "Node.h"

namespace svg
{
    class TextRenderer;

// =============================================================================
/** @class  Text
    @brief  Text
*/
// Created: AGE 2006-10-20
// =============================================================================
class Text : public Node
{

public:
    //! @name Constructors/Destructor
    //@{
             Text( xml::xistream& input, References_ABC& references, TextRenderer& renderer, const PropertyFactory& factory );
    virtual ~Text();
    //@}

private:
    //! @name Operations
    //@{
    virtual Node& CompileInternal( RenderingContext_ABC& context, References_ABC& references ) const;
    virtual void DrawInternal( RenderingContext_ABC& context, References_ABC& references ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Text( const Text& );            //!< Copy constructor
    Text& operator=( const Text& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    TextRenderer& renderer_;
    Transformations* translation_;
    std::string text_;
    //@}
};

}

#endif // __Text_h_
