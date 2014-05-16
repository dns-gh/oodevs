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

namespace boost
{
    template< typename T > class shared_ptr;
}

class DEC_Decision_ABC;
class DEC_Knowledge_Object;
class MIL_Entity_ABC;
class MIL_AgentPion;

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
    void CallbackKnowledge( boost::shared_ptr< DEC_Knowledge_Object > id );
    virtual void StopAction() = 0;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PHY_DecisionCallbackAction_ABC( const PHY_DecisionCallbackAction_ABC& );            //!< Copy constructor
    PHY_DecisionCallbackAction_ABC& operator=( const PHY_DecisionCallbackAction_ABC& ); //!< Assignment operator
    //@}

    //! @name Operations
    //@{
    virtual void Stop();
    //@}

private:
    //! @name Member data
    //@{
    DEC_Decision_ABC& decision_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: template< typename T > void PHY_DecisionCallbackAction_ABC::Callback
// Created: LDC 2009-06-26
// -----------------------------------------------------------------------------
template< typename T > 
void PHY_DecisionCallbackAction_ABC::Callback( T value )
{
    decision_.Callback( GetId(), value );
}

#endif // __PHY_DecisionCallbackAction_ABC_h_
