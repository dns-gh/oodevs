// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Resources_h_
#define __Resources_h_

#include "Extractors.h"
#include <set>
#include <map>

namespace xml
{
    class xistream;
}

namespace extractors
{

// =============================================================================
/** @class  Resources
    @brief  Resources
    // $$$$ AGE 2007-10-23: dotations ?
*/
// Created: AGE 2007-10-23
// =============================================================================
class Resources : public Extractor< int >
{
public:
    //! @name Types
    //@{
    enum { has_parameter = true };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
                 Resources();
    /*implicit*/ Resources( xml::xistream& xis );
    //@}

    //! @name Operations
    //@{
    bool HasFlag( const ASN1T_MsgUnitAttributes& attributes ) const
    { 
        return attributes.m.dotation_eff_ressourcePresent;
    }
    int Extract( const ASN1T_MsgUnitAttributes& attributes );
    //@}

private:
    //! @name Member data
    //@{
    std::set< int >      filter_;
    std::map< int, int > resources_;
    //@}
};

}

#endif // __Resources_h_
