// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Diplomacies_h_
#define __Diplomacies_h_

#include "clients_kernel/Diplomacies_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Karma.h"
#include <tools/Resolver_ABC.h>

namespace sword
{
    class ChangeDiplomacyAck;
    class ChangeDiplomacy;
    class PartyCreation;
}

namespace kernel
{
    class Team_ABC;
    class Entity_ABC;
    class Controller;
}

// =============================================================================
/** @class  Diplomacies
    @brief  Diplomacies
*/
// Created: AGE 2006-02-14
// =============================================================================
class Diplomacies : public kernel::Diplomacies_ABC
                  , public kernel::Updatable_ABC< sword::PartyCreation >
                  , public kernel::Updatable_ABC< sword::ChangeDiplomacyAck >
                  , public kernel::Updatable_ABC< sword::ChangeDiplomacy >
{
public:
    //! @name Constructors/Destructor
    //@{
             Diplomacies( kernel::Controller& controller, const tools::Resolver_ABC< kernel::Team_ABC >& resolver );
    virtual ~Diplomacies();
    //@}

    //! @name Operations
    //@{
    virtual const kernel::Karma& GetDiplomacy( const kernel::Entity_ABC& team ) const;
    virtual const kernel::Karma& GetKarma() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Diplomacies( const Diplomacies& );            //!< Copy constructor
    Diplomacies& operator=( const Diplomacies& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    typedef std::map< const Diplomacies_ABC*, kernel::Karma > T_Diplomacies;
    typedef T_Diplomacies::const_iterator                   CIT_Diplomacies;
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const sword::PartyCreation& message );
    virtual void DoUpdate( const sword::ChangeDiplomacyAck& message );
    virtual void DoUpdate( const sword::ChangeDiplomacy& message );
    template< typename T >
    void UpdateData( const T& message );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const tools::Resolver_ABC< kernel::Team_ABC >& resolver_;
    T_Diplomacies diplomacies_;
    kernel::Karma karma_;
    //@}
};

#endif // __Diplomacies_h_
