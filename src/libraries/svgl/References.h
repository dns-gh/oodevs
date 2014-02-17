// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __References_h_
#define __References_h_

#include "References_ABC.h"
#include <map>

namespace svg
{

// =============================================================================
/** @class  References
    @brief  References
*/
// Created: AGE 2006-08-22
// =============================================================================
class References : public References_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             References();
    virtual ~References();
    //@}

    //! @name Operations
    //@{
    void Clear();

    virtual void Register( const std::string& id, const Node_ABC& element );
    virtual void Draw( const std::string& id, RenderingContext_ABC& context );

    virtual void Register( const std::string& id, const Paint_ABC& paint );
    virtual bool Setup( const std::string& id, float opacity );

    virtual void Register( const std::string& id, const Style_ABC& style );
    virtual void Push( const std::string& id, RenderingContext_ABC& context );
    virtual void Pop ( const std::string& id, RenderingContext_ABC& context );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    References( const References& );            //!< Copy constructor
    References& operator=( const References& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::map< std::string, const Node_ABC* >  T_Nodes;
    typedef std::map< std::string, const Paint_ABC* > T_Paints;
    typedef std::map< std::string, const Style_ABC* > T_Styles;
    //@}

private:
    //! @name Member data
    //@{
    T_Nodes nodes_;
    T_Paints paints_;
    T_Styles styles_;
    //@}
};

}

#endif // __References_h_
