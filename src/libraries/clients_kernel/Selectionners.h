// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Selectionners_h_
#define __Selectionners_h_

namespace kernel
{
class Selectable_ABC;

// =============================================================================
/** @class  Selectionner_ABC
    @brief  Selectionner_ABC
*/
// Created: JSR 2011-05-31
// =============================================================================
class Selectionner_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Selectionner_ABC() {}
    virtual ~Selectionner_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual bool IsOfSameType( const Selectable_ABC* selectable ) const = 0;
    //@}
};

template< typename T >
class Selectionner : public Selectionner_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Selectionner() {}
    virtual ~Selectionner() {}
    //@}

    //! @name Operations
    //@{
    virtual bool IsOfSameType( const Selectable_ABC* selectable ) const
    {
        return dynamic_cast< const T* >( selectable ) != 0;
    }
    //@}
};

}

#endif // __Selectionners_h_
