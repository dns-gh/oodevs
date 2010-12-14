 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __StockAttribute_h_
#define _StockAttribute_h_

#include "ObjectAttribute_ABC.h"

namespace sword
{
    class StockResource;
}

namespace dispatcher
{
// =============================================================================
/** @class  StockAttribute
    @brief  StockAttribute
*/
// Created: NLD 2006-09-19
// =============================================================================
class StockAttribute : public ObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit StockAttribute( const sword::ObjectAttributes& asnMsg );
    virtual ~StockAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update( const sword::ObjectAttributes& asnMsg );
    virtual void Send  ( sword::ObjectAttributes& asnMsg ) const;
    //@}

private:
    //! @name Types
    //@{
    struct StockResource
    {
        StockResource();
        StockResource( const sword::StockResource& msg );

        unsigned resource;
        unsigned int current;
        unsigned int maximum;
    };
    //@}

private:

    //! @name Data members
    //@{
    std::vector< StockResource > stocks_;
    //@}
};

}

#endif // __StockAttribute_h_
