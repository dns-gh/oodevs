// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Troops_h_
#define __Troops_h_


#include "HierarchicExtension_ABC.h"
#include "Humans.h"
#include "clients_kernel/Updatable_ABC.h"
#include "protocol/Protocol.h"
#include "protocol/Protocol.h"

namespace kernel
{
    class Controller;
    class Entity_ABC;
    class Automat_ABC;
}

namespace MsgsSimToClient
{
    class MsgUnitAttributes;
}
// =============================================================================
/** @class  Troops
    @brief  Troops
*/
// Created: AGE 2006-02-13
// =============================================================================
class Troops : public kernel::Extension_ABC
             , public HierarchicExtension_ABC
             , public kernel::Updatable_ABC< MsgsSimToClient::MsgUnitAttributes >
{

public:
    //! @name Constructors/Destructor
    //@{
             Troops( kernel::Controller& controller, const tools::Resolver_ABC< kernel::Automat_ABC >& automatResolver, const tools::Resolver_ABC< kernel::Formation_ABC >& formationResolver, const tools::Resolver_ABC< kernel::Team_ABC >& teamResolver );
    virtual ~Troops();
    //@}

    //! @name Operators
    //@{
    int Troops::GetTotalHumans() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Troops( const Troops& );            //!< Copy constructor
    Troops& operator=( const Troops& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::pair< kernel::E_TroopHealthState, Common::EnumHumanRank > T_HumanState;
    typedef std::map< T_HumanState, int > T_Differences;
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const MsgsSimToClient::MsgUnitAttributes& message );
    virtual void SetSuperior( const kernel::Entity_ABC& superior );
    void Update( const T_Differences& differences );
    void AddDifference( T_Differences& differences, kernel::E_TroopHealthState state, Common::EnumHumanRank rank, int value );
    //@}

public: // $$$$ AGE 2006-04-28:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    Humans humans_[kernel::eTroopHealthStateNbrStates];
    //@}
};

#endif // __Troops_h_
