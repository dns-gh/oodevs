// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __FormationPositions_h_
#define __FormationPositions_h_

#include "clients_kernel/Moveable_ABC.h"
#include "clients_kernel/Aggregatable_ABC.h"
#include <boost/function.hpp>

namespace kernel
{
    class Entity_ABC;
}

// =============================================================================
/** @class  FormationPositions
    @brief  Formation positions
*/
// Created: SBO 2009-02-02
// =============================================================================
class FormationPositions : public kernel::Moveable_ABC
                         , public kernel::Aggregatable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit FormationPositions( const kernel::Entity_ABC& formation );
    virtual ~FormationPositions();
    //@}

    //! @name Operations
    //@{
    virtual geometry::Point2f GetPosition( bool aggregated ) const;
    virtual float GetHeight( bool aggregated ) const;
    virtual bool IsIn( const geometry::Rectangle2f& rectangle ) const;
    virtual geometry::Rectangle2f GetBoundingBox() const;
    virtual void Accept( kernel::LocationVisitor_ABC& visitor ) const;
    virtual void Aggregate( const bool& );
    virtual bool CanAggregate() const;
    virtual bool IsAggregated() const;
    virtual void Move( const geometry::Point2f& point );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FormationPositions( const FormationPositions& );            //!< Copy constructor
    FormationPositions& operator=( const FormationPositions& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    bool IsAgent( const kernel::Entity_ABC& entity ) const;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Entity_ABC& formation_;
    bool aggregated_;
    //@}
};

#endif // __FormationPositions_h_
