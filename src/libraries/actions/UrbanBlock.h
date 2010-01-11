// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterUrbanBlock_h_
#define __ActionParameterUrbanBlock_h_

#include "Parameter.h"


namespace urban
{
    class TerrainObject_ABC;
    class Block;
}

namespace actions {

namespace parameters {

// =============================================================================
/** @class  UrbanBlock
    @brief  UrbanBlock
*/
// Created: MGD 2009-11-05
// =============================================================================
class UrbanBlock : public Parameter< unsigned long >
{

public:
    //! @name Constructors/Destructor
    //@{
    UrbanBlock( const kernel::OrderParameter& parameter, unsigned long id );
    UrbanBlock( const kernel::OrderParameter& parameter, const ASN1T_UrbanBlock& asn );
    UrbanBlock( const kernel::OrderParameter& parameter, xml::xistream& xis );
    virtual ~UrbanBlock();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    UrbanBlock( const UrbanBlock& );            //!< Copy constructor
    UrbanBlock& operator=( const UrbanBlock& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void Serialize( xml::xostream& xos ) const;
    //@}
};

}

}

#endif // __ActionParameterUrbanBlock_h_
