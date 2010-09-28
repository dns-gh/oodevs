 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ConstructionAttribute_h_
#define __ConstructionAttribute_h_

#include "ObjectAttribute_ABC.h"

namespace dispatcher
{

// =============================================================================
/** @class  ConstructionAttribute
    @brief  ConstructionAttribute
*/
// Created: NLD 2006-09-19
// =============================================================================
class ConstructionAttribute : public ObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ConstructionAttribute( const Common::ObjectAttributes& asnMsg );
    virtual ~ConstructionAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update( const Common::ObjectAttributes& asnMsg );
    virtual void Send  ( Common::ObjectAttributes& asnMsg ) const;
    //@}

private:
    //! @name Data members
    //@{
    unsigned     dotation_;
    unsigned int nPercentageConstruction_;
    unsigned int nNbrDotationForConstruction_;
    //@}

};

}

#endif // __ConstructionAttribute_h_
