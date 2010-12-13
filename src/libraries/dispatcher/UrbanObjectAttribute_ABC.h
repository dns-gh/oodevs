// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UrbanObjectAttribute_ABC_h_
#define __UrbanObjectAttribute_ABC_h_

namespace sword
{
    class UrbanAttributes;
}

namespace dispatcher
{

// =============================================================================
/** @class  UrbanObjectAttribute_ABC
    @brief  UrbanObjectAttribute_ABC
*/
// Created: SLG 2009-12-04
// =============================================================================
class UrbanObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             UrbanObjectAttribute_ABC() {}
    virtual ~UrbanObjectAttribute_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Update( const sword::UrbanAttributes& message ) = 0;
    virtual void Send  (       sword::UrbanAttributes& message ) const = 0;
    //@}
};

}

#endif // __UrbanObjectAttribute_ABC_h_
