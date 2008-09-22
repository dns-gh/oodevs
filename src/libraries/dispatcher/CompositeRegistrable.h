// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CompositeRegistrable_h_
#define __CompositeRegistrable_h_

#include "Registrable_ABC.h"
#include <boost/ptr_container/ptr_vector.hpp>

namespace dispatcher
{

// =============================================================================
/** @class  CompositeRegistrable
    @brief  CompositeRegistrable
*/
// Created: AGE 2008-06-12
// =============================================================================
class CompositeRegistrable : public Registrable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             CompositeRegistrable();
    virtual ~CompositeRegistrable();
    //@}

    //! @name Operations
    //@{
    void Add( Registrable_ABC* r );

    virtual void RegisterIn( directia::Brain& brain );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    CompositeRegistrable( const CompositeRegistrable& );            //!< Copy constructor
    CompositeRegistrable& operator=( const CompositeRegistrable& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    boost::ptr_vector< Registrable_ABC > registrables_;
    //@}
};

// =============================================================================
/** @class  RegistrableProxy
    @brief  RegistrableProxy
*/
// Created: AGE 2008-06-12
// =============================================================================
class RegistrableProxy : public Registrable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit RegistrableProxy( Registrable_ABC& forward ) : forward_( forward ) {}
    virtual ~RegistrableProxy() {};
    //@}

    //! @name Operations
    //@{
    virtual void RegisterIn( directia::Brain& brain ) 
    {
        forward_.RegisterIn( brain );
    }
    //@}

private:
    //! @name Copy/Assignment
    //@{
    RegistrableProxy( const RegistrableProxy& );            //!< Copy constructor
    RegistrableProxy& operator=( const RegistrableProxy& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    Registrable_ABC& forward_;
    //@}
};

}

#endif // __CompositeRegistrable_h_
