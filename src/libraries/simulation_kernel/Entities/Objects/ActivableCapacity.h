// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __ActivableCapacity_h_
#define __ActivableCapacity_h_

#include "ObjectCapacity_ABC.h"

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  ActivableCapacity
    @brief  ActivableCapacity
*/
// Created: JCR 2008-05-30
// =============================================================================
class ActivableCapacity : public ObjectCapacity_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ActivableCapacity();
             ActivableCapacity( const ActivableCapacity& );
    explicit ActivableCapacity( xml::xistream& xis );
    virtual ~ActivableCapacity();
    //@}

     //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name Operations
    //@{
    virtual void Instanciate( MIL_Object_ABC& object ) const;
    virtual void Register( MIL_Object_ABC& object );
    virtual void Update( MIL_Object_ABC& object, unsigned int time );
    void Activate( MIL_Object_ABC& object );
    //@}

    //! @name Accessors
    //@{
    bool IsActivated( MIL_Object_ABC& object ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActivableCapacity& operator=( const ActivableCapacity& ); //!< Assignment operator
    //@}

private:
    //! @name Members
    //@{
    unsigned int timeOfCreation_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( ActivableCapacity )

#endif // __ActivableCapacity_h_
