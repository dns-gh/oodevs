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

#include "game_asn/Simulation.h"
#include "Parameter.h"

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
             PathList( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const ASN1T_PathList& asn );
             PathList( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, xml::xistream& xis );
    virtual ~PathList();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    virtual void Clean( ASN1T_MissionParameter& asn ) const;
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
