// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Extendable_h_
#define __Extendable_h_

#include <vector>

// =============================================================================
/** @class  Extendable
    @brief  Extendable stuff
*/
// Created: AGE 2006-02-06
// =============================================================================
template< typename BaseType >
class Extendable
{

public:
    //! @name Constructors/Destructor
    //@{
             Extendable() {};
    virtual ~Extendable();
    //@}

    //! @name Operations
    //@{
    template< typename Extension > 
    void Attach( Extension& extension );

    template< typename Extension >
    Extension* Retreive();

    template< typename Extension >
    Extension& Get();
    //@}

protected:
    //! @name Operations
    //@{
    virtual void AddExtension( BaseType*& where, BaseType* ext );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Extendable( const Extendable& );            //!< Copy constructor
    Extendable& operator=( const Extendable& ); //!< Assignement operator
    //@}

private:
    //! @name Types
    //@{
    typedef          std::vector< BaseType* >       T_Extensions;
    typedef typename T_Extensions::iterator        IT_Extensions;
    typedef typename T_Extensions::const_iterator CIT_Extensions;
    //@}

    //! @name Helpers
    //@{
    template< typename Extension >
    static unsigned int GetExtensionId();

    template< typename Extension >
    BaseType*& Find();
    //@}

private:
    //! @name Member data
    //@{
    static unsigned int id_;
    T_Extensions extensions_;
    //@}
};

#include "Extendable.inl"

#endif // __Extendable_h_
