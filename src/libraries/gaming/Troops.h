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

#include <boost/noncopyable.hpp>
#include "HierarchicExtension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "HumanState.h"

namespace kernel
{
    class Controller;
}

namespace sword
{
    class UnitAttributes;
}

// =============================================================================
/** @class  Troops
    @brief  Troops
*/
// Created: AGE 2006-02-13
// =============================================================================
class Troops : public kernel::Extension_ABC
             , public HierarchicExtension_ABC
             , public kernel::Updatable_ABC< sword::UnitAttributes >
             , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Troops( kernel::Controller& controller, const tools::Resolver_ABC< kernel::Automat_ABC >& automatResolver, const tools::Resolver_ABC< kernel::Formation_ABC >& formationResolver, const tools::Resolver_ABC< kernel::Team_ABC >& teamResolver );
    virtual ~Troops();
    //@}

    //! @name Operators
    //@{
    unsigned int GetTotalHumans() const;
    unsigned int GetTotalByRank( E_HumanRank rank ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< HumanState >            T_HumanStateVector;
    typedef T_HumanStateVector::iterator        IT_HumanStateVector;
    typedef T_HumanStateVector::const_iterator CIT_HumanStateVector;
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const sword::UnitAttributes& message );
    virtual void SetSuperior( const kernel::Entity_ABC& superior );
    void Update( const T_HumanStateVector& differences );

    void RemoveHumanStates();
    HumanState* FindHumanState( T_HumanStateVector& container, const HumanState& state ) const;
    //@}

public:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    T_HumanStateVector  elements_;
    //@}
};

#endif // __Troops_h_
