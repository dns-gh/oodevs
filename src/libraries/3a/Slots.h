// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Slots_h_
#define __Slots_h_

// =============================================================================
/** @class  Slot_ABC
    @brief  Slot_ABC
*/
// Created: AGE 2007-09-10
// =============================================================================
class Slot_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Slot_ABC() {}
    virtual ~Slot_ABC() {}
    //@}

    //! @name Accessors
    //@{
    virtual std::string GetName() const = 0;
    //@}

    //! @name Operations
    //@{
    template< typename Target >
    Target& Cast( unsigned slot )
    {
        Target* target = dynamic_cast< Target* >( GetSubSlot( slot ) );
        if( ! target )
            throw std::runtime_error( GetName() + " is not of expected type '" + typeid( Target ).name() );
        return *target;
    }
    //@}

protected:
    //! @name Operations
    //@{
    virtual Slot_ABC* GetSubSlot( unsigned int ) { return this; }
    //@}
};

#endif // __Slots_h_
