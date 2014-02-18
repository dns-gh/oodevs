// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Use_h_
#define __Use_h_

#include "Node.h"

namespace svg 
{
    class Style;

// =============================================================================
/** @class  Use
    @brief  Use
*/
// Created: AGE 2006-08-10
// =============================================================================
class Use : public Node
{

public:
    //! @name Constructors/Destructor
    //@{
             Use( xml::xistream& input, References_ABC& references, const PropertyFactory& factory );
    virtual ~Use();
    //@}

private:
    //! @name Operations
    //@{
    virtual Node& CompileInternal( RenderingContext_ABC& context, References_ABC& references ) const;
    virtual void DrawInternal( RenderingContext_ABC& context, References_ABC& references ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Use( const Use& );            //!< Copy constructor
    Use& operator=( const Use& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    std::string target_;
    Transformations* translation_;
    //@}
};

}

#endif // __Use_h_
