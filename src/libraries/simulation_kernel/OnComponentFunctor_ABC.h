// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __ConsumptionOperator_ABC_h_
#define __ConsumptionOperator_ABC_h_

// =============================================================================
/** @class   ConsumptionOperator_ABC
    @brief   ConsumptionOperator_ABC
*/
// Created: MGD 2009-08-24
// =============================================================================
class PHY_ComposantePion;

class OnComponentFunctor_ABC : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             OnComponentFunctor_ABC();
    virtual ~OnComponentFunctor_ABC();
    //@}

    //! @name Operation
    //@{
    virtual void operator()( const PHY_ComposantePion& component ) = 0;
    //@}
};


#endif // __ConsumptionOperator_ABC_h_
