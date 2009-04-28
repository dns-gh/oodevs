// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __IndicatorElementDeclarator_ABC_h_
#define __IndicatorElementDeclarator_ABC_h_

// =============================================================================
/** @class  IndicatorElementDeclarator_ABC
    @brief  IndicatorElementDeclarator_ABC
*/
// Created: SBO 2009-04-28
// =============================================================================
class IndicatorElementDeclarator_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             IndicatorElementDeclarator_ABC() {}
    virtual ~IndicatorElementDeclarator_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Declare( boost::shared_ptr< IndicatorElement_ABC > element ) = 0;
    //@}
};

#endif // __IndicatorElementDeclarator_ABC_h_
