// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __PHY_DecisionCallbackAction_ABC_h_
#define __PHY_DecisionCallbackAction_ABC_h_

#include "PHY_Action_ABC.h"
#include "Decision/DEC_Decision_ABC.h"
class MIL_Entity_ABC;

// =============================================================================
/** @class  PHY_DecisionCallbackAction_ABC
    @brief  PHY_DecisionCallbackAction_ABC
*/
// Created: LDC 2009-06-26
// =============================================================================
class PHY_DecisionCallbackAction_ABC : public PHY_Action_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit PHY_DecisionCallbackAction_ABC( MIL_Entity_ABC& actor );
    virtual ~PHY_DecisionCallbackAction_ABC();
    //@}

protected:
    //! @name Operations
    //@{
    template< typename T > void Callback( T );
    void CallbackKnowledge( int id );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PHY_DecisionCallbackAction_ABC( const PHY_DecisionCallbackAction_ABC& );            //!< Copy constructor
    PHY_DecisionCallbackAction_ABC& operator=( const PHY_DecisionCallbackAction_ABC& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    DEC_Decision_ABC&    decision_;
    //@}
};

#include "PHY_DecisionCallbackAction_ABC.inl"

#endif // __PHY_DecisionCallbackAction_ABC_h_
