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
    explicit ActivableCapacity( xml::xistream& xis );
             ActivableCapacity();
             ActivableCapacity( const ActivableCapacity& ); //!< Copy constructor
    virtual ~ActivableCapacity();
    //@}

     //! @name CheckPoints
    //@{    
    template< typename Archive > void serialize( Archive&, const uint );
    virtual void Register( Object& /*object*/ );
    //@}

    //! @name virtual
    //@{
    virtual void Instanciate( Object& object ) const;	
    //@}   

    //! @name 
    //@{    
    void Activate( Object& object );
    bool IsActivated( Object& object ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{    
    ActivableCapacity& operator=( const ActivableCapacity& ); //!< Assignment operator
    //@}
};

#endif // __ActivableCapacity_h_
