// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __InteractWithSideCapacity_h_
#define __InteractWithSideCapacity_h_

#include "ObjectCapacity_ABC.h"

namespace xml
{
    class xistream;
}

class MIL_Army_ABC;
class MIL_Agent_ABC;

// =============================================================================
/** @class  InteractWithSideCapacity
    @brief  InteractWithSideCapacity
*/
// Created: CMA 2012-05-10
// =============================================================================
class InteractWithSideCapacity : public ObjectCapacity_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             InteractWithSideCapacity();
    explicit InteractWithSideCapacity( xml::xistream& xis );
    virtual ~InteractWithSideCapacity();
    //@}

    //! @name Operations
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    virtual void Instanciate( MIL_Object_ABC& object ) const;
    virtual void Register( MIL_Object_ABC& object );
    bool IsPossible( const MIL_Army_ABC& army1, const MIL_Agent_ABC& agent ) const;
    bool IsPossible( const MIL_Army_ABC& army1, const MIL_Army_ABC& army2 ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    InteractWithSideCapacity& operator=( const InteractWithSideCapacity& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    bool friendSide_;
    bool enemySide_;
    bool neutralSide_;
    bool civilian_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( InteractWithSideCapacity )

#endif // __InteractWithSideCapacity_h_
