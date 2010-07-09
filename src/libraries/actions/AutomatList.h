// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterAutomatList_h_
#define __ActionParameterAutomatList_h_

#include "Parameter.h"

namespace Common
{
    class MsgAutomatList;
}

namespace kernel
{
    class Automat_ABC;
    class Controller;
    class EntityResolver_ABC;
}

namespace actions {
    namespace parameters {

// =============================================================================
/** @class  AutomatList
    @brief  AutomatList
*/
// Created: SBO 2007-05-23
// =============================================================================
class AutomatList : public Parameter< QString >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit AutomatList( const kernel::OrderParameter& parameter );
             AutomatList( const kernel::OrderParameter& parameter, const Common::MsgAutomatList& message, const kernel::EntityResolver_ABC& resolver, kernel::Controller& controller );
             AutomatList( const kernel::OrderParameter& parameter, xml::xistream& xis, const kernel::EntityResolver_ABC& resolver, kernel::Controller& controller );
    virtual ~AutomatList();
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
    AutomatList( const AutomatList& );            //!< Copy constructor
    AutomatList& operator=( const AutomatList& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadAutomat( xml::xistream& xis, const kernel::EntityResolver_ABC& resolver, kernel::Controller& controller );
    //@}
};
    }
}

#endif // __ActionParameterAutomatList_h_
