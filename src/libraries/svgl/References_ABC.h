// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __References_ABC_h_
#define __References_ABC_h_

namespace svg
{

class RenderingContext_ABC;
class Node_ABC;
class Paint_ABC;
class Style_ABC;

// =============================================================================
/** @class  References_ABC
    @brief  References_ABC
*/
// Created: AGE 2006-08-22
// =============================================================================
class References_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             References_ABC() {};
    virtual ~References_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual void Register( const std::string& id, const Node_ABC& element ) = 0;
    virtual void Register( const std::string& id, const Paint_ABC& paint ) = 0;
    virtual void Register( const std::string& id, const Style_ABC& style ) = 0;

    virtual void Draw( const std::string& id, RenderingContext_ABC& context ) = 0;
    virtual bool Setup( const std::string& id, float opacity ) = 0;
    virtual void Push( const std::string& id, RenderingContext_ABC& context ) = 0;
    virtual void Pop ( const std::string& id, RenderingContext_ABC& context ) = 0;
    //@}
};

}

#endif // __References_ABC_h_
