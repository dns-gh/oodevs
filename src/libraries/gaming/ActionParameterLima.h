// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterLima_h_
#define __ActionParameterLima_h_

#include "ActionParameter.h"
#include "game_asn/Asn.h"

namespace kernel
{
    class Location_ABC;
    class CoordinateConverter_ABC;
}

// =============================================================================
/** @class  ActionParameterLima
    @brief  ActionParameterLima
*/
// Created: SBO 2007-04-26
// =============================================================================
class ActionParameterLima : public ActionParameter< QString >
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionParameterLima( const QString& name, const kernel::CoordinateConverter_ABC& converter, const kernel::Location_ABC& location );
             ActionParameterLima( const QString& name, const kernel::CoordinateConverter_ABC& converter, const ASN1T_LimaOrder& asn );
    virtual ~ActionParameterLima();
    //@}

    //! @name Operations
    //@{
    virtual bool IsContext() const;
    void AddFunction( unsigned int i );
    virtual void DisplayInToolTip( kernel::Displayer_ABC& displayer ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionParameterLima( const ActionParameterLima& );            //!< Copy constructor
    ActionParameterLima& operator=( const ActionParameterLima& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void Serialize( xml::xostream& xos ) const;
    //@}
};

#endif // __ActionParameterLima_h_
