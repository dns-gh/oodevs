// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __BypassableCapacity_h_
#define __BypassableCapacity_h_

#include "ObjectCapacity_ABC.h"
#include "MT_Tools/MT_Tools_Types.h"

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  BypassableCapacity
    @brief  BypassableCapacity
*/
// Created: JCR 2008-05-30
// =============================================================================
class BypassableCapacity : public ObjectCapacity_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             BypassableCapacity();
    explicit BypassableCapacity( xml::xistream& xis );
    virtual ~BypassableCapacity();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name Operations
    //@{
    virtual void Instanciate( MIL_Object_ABC& object ) const;
    virtual void Register( MIL_Object_ABC& object );
    void Bypass( MIL_Object_ABC& object, MT_Float rDeltaPercentage );
    //@}

private:
    //! @name Member data
    //@{
    unsigned int bypassSpeed_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( BypassableCapacity )

#endif // __BypassableCapacity_h_
