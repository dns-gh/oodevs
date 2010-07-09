// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterPathList_h_
#define __ActionParameterPathList_h_

#include "Parameter.h"

namespace Common
{
    class MsgPathList;
}

namespace kernel
{
    class CoordinateConverter_ABC;
}

namespace actions {
    namespace parameters {

// =============================================================================
/** @class  PathList
    @brief  PathList
*/
// Created: SBO 2007-05-21
// =============================================================================
class PathList : public Parameter< QString >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit PathList( const kernel::OrderParameter& parameter );
             PathList( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const Common::MsgPathList& message );
             PathList( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, xml::xistream& xis );
    virtual ~PathList();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( Common::MsgMissionParameter& message ) const;
    virtual void Clean( Common::MsgMissionParameter& message ) const;
    virtual bool IsSet() const;
    //@}

protected:
    //! @name Operations
    //@{
    virtual void DisplayTooltip( const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PathList( const PathList& );            //!< Copy constructor
    PathList& operator=( const PathList& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadPath( xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter );
    //@}
};

    }
}

#endif // __ActionParameterPathList_h_
