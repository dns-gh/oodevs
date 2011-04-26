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
    virtual ~Resources() {}
    //@}

    //! @name Operations
    //@{
    bool HasFlag( const sword::UnitAttributes& attributes ) const
    {
        return attributes.has_resource_dotations();
    }
    int Extract( const sword::UnitAttributes& attributes );
    //@}

private:
    //! @name Member data
    //@{
    FilterHelper< int > filter_;
    std::map< int, int > resources_;
    //@}
};

}

#endif // __Resources_h_
