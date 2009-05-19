// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Resources_h_
#define __Resources_h_

#include "Extractors.h"
#include "FilterHelper.h"
#pragma warning( push )
#pragma warning( disable : 4702 )
#include <map>
#pragma warning( pop )

namespace extractors
{

// =============================================================================
/** @class  Resources
    @brief  Resources
            dotations='id1,id2,...'
*/
// Created: AGE 2007-10-23
// =============================================================================
class Resources : public Extractor< NumericValue >
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
    //@{*
    FilterHelper< int > filter_;
    std::map< int, int > resources_;
    //@}
};

}

#endif // __Resources_h_
