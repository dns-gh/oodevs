// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __StructureAttribute_h_
#define __StructureAttribute_h_

#include "UrbanObjectAttribute_ABC.h"

namespace dispatcher
{

// =============================================================================
/** @class  StructureAttribute
    @brief  StructureAttribute
*/
// Created: SLG 2009-12-04
// =============================================================================
class StructureAttribute : public UrbanObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit StructureAttribute( const sword::UrbanAttributes& message );
    virtual ~StructureAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update( const sword::UrbanAttributes& message );
    virtual void Send  (       sword::UrbanAttributes& message ) const;
    //@}

private:
    //! @name Member data
    //@{
    unsigned int structuralState_;
    //@}
};

}

#endif // __StructureAttribute_h_
