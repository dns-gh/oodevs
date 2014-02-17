// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Path_h_
#define __Path_h_

#include "Node.h"

namespace svg 
{

class PathCommand_ABC;
class PathParser;
class PathContext;

// =============================================================================
/** @class  Path
    @brief  Path
*/
// Created: AGE 2006-08-10
// =============================================================================
class Path : public Node
{

public:
    //! @name Constructors/Destructor
    //@{
             Path( xml::xistream& input, References_ABC& references, const PropertyFactory& factory );
             Path( xml::xistream& input, References_ABC& references, const PropertyFactory& factory, const std::string& data );
    virtual ~Path();
    //@}

    //! @name Operations
    //@{
    virtual Node& CompileInternal( RenderingContext_ABC& context, References_ABC& references ) const;
    virtual void DrawInternal( RenderingContext_ABC& context, References_ABC& references ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Path( const Path& );            //!< Copy constructor
    Path& operator=( const Path& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void Parse( const std::string& data );
    PathCommand_ABC* CreateCommand( PathParser& data, PathContext& context );
    //@}

    //! @name Types
    //@{
    typedef std::vector< PathCommand_ABC* >  T_PathCommands;
    typedef T_PathCommands::const_iterator CIT_PathCommands;
    //@}

private:
    //! @name Member data
    //@{
    T_PathCommands commands_;
    //@}
};

}

#endif // __Path_h_
