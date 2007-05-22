// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterLimit_h_
#define __ActionParameterLimit_h_

#include "game_asn/Asn.h"
#include "ActionParameter_ABC.h"

namespace kernel
{
    class CoordinateConverter_ABC;
    class Location_ABC;
}

namespace xml
{
    class xistream;
}

class ActionParameterLocation;

// =============================================================================
/** @class  ActionParameterLimit
    @brief  ActionParameterLimit
*/
// Created: SBO 2007-04-13
// =============================================================================
class ActionParameterLimit : public ActionParameter_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionParameterLimit( const QString& name, const kernel::CoordinateConverter_ABC& converter, const ASN1T_Line& line );
             ActionParameterLimit( const QString& name, const kernel::CoordinateConverter_ABC& converter, const kernel::Location_ABC& location );
             ActionParameterLimit( const kernel::CoordinateConverter_ABC& converter, xml::xistream& xis );
    virtual ~ActionParameterLimit();
    //@}

    //! @name Operations
    //@{
    virtual bool IsContext() const;
    void CommitTo( ASN1T_Line& asn ) const;
    void Clean( ASN1T_Line& asn ) const;
    virtual void Accept( ActionParameterVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionParameterLimit( const ActionParameterLimit& );            //!< Copy constructor
    ActionParameterLimit& operator=( const ActionParameterLimit& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void Serialize( xml::xostream& xos ) const;
    //@}

private:
    //! @name Member data
    //@{
    ActionParameterLocation* location_;
    //@}
};

#endif // __ActionParameterLimit_h_
