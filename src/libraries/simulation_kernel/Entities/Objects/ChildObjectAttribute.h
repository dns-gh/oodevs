// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __ChildObjectAttribute_h_
#define __ChildObjectAttribute_h_

#include "ObjectAttribute_ABC.h"

// =============================================================================
/** @class  ChildObjectAttribute
    @brief  ChildObjectAttribute
*/
// Created: SLG 2010-02-16
// =============================================================================
class ChildObjectAttribute : public ObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ChildObjectAttribute();
    explicit ChildObjectAttribute( MIL_Object_ABC& object );
    virtual ~ChildObjectAttribute();
    //@}

    //! @name CheckPoint
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name From ObjectAttribute_ABC
    //@{
    virtual void Register( MIL_Object_ABC& object ) const;
    //@}

    //! @name Operations
    //@{
    void AddChildObject( MIL_Object_ABC& object );
    MIL_Object_ABC* GetChildObject() const;
    //@}

private:
    //! @name Member data
    //@{
    MIL_Object_ABC* childObject_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( ChildObjectAttribute )

#endif // __ChildObjectAttribute_h_
