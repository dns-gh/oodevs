// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Troops_h_
#define __Troops_h_

#include "game_asn/Asn.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "Humans.h"

namespace kernel
{
    class Controller;
    class Entity_ABC;
}

// =============================================================================
/** @class  Troops
    @brief  Troops
*/
// Created: AGE 2006-02-13
// =============================================================================
class Troops : public kernel::Extension_ABC
             , public kernel::Updatable_ABC< ASN1T_MsgUnitAttributes >
{

public:
    //! @name Constructors/Destructor
    //@{
             Troops( kernel::Controller& controller, kernel::Entity_ABC& holder );
    virtual ~Troops();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Troops( const Troops& );            //!< Copy constructor
    Troops& operator=( const Troops& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::pair< kernel::E_TroopHealthState, ASN1T_EnumHumanRank > T_HumanState;
    typedef std::map< T_HumanState, int > T_Differences;
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgUnitAttributes& message );
    void Update( const T_Differences& differences );
    void AddDifference( T_Differences& differences, kernel::E_TroopHealthState state, ASN1T_EnumHumanRank rank, int value );
    //@}

public: // $$$$ AGE 2006-04-28: 
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    kernel::Entity_ABC& holder_;
    Humans humans_[kernel::eTroopHealthStateNbrStates];
    //@}
};

#endif // __Troops_h_
