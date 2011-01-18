// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __BypassableCapacity_h_
#define __BypassableCapacity_h_

#include "ObjectCapacity_ABC.h"

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
    void Bypass( MIL_Object_ABC& object, double rDeltaPercentage );
    bool IsBypassed( const MIL_Object_ABC& object ) const;
    double GetBypassSpeed() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    BypassableCapacity( const BypassableCapacity& );
    BypassableCapacity& operator=( const BypassableCapacity& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    double bypassSpeed_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( BypassableCapacity )

#endif // __BypassableCapacity_h_
