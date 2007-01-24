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

#include "network/Simulation_Asn.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Resolver_ABC.h"

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
class Diplomacies : public kernel::Extension_ABC
                  , public kernel::Updatable_ABC< ASN1T_MsgSideCreation >
                  , public kernel::Updatable_ABC< ASN1T_MsgChangeDiplomatieAck >
                  , public kernel::Updatable_ABC< ASN1T_MsgChangeDiplomatie >
{

public:
    //! @name Constructors/Destructor
    //@{
             Diplomacies( kernel::Controller& controller, const kernel::Resolver_ABC< kernel::Team_ABC >& resolver );
    virtual ~Diplomacies();
    //@}

    //! @name Operations
    //@{
    char GetKarma() const;
    ASN1T_EnumDiplomatie GetRelationship( const kernel::Entity_ABC& rhs ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Diplomacies( const Diplomacies& );            //!< Copy constructor
    Diplomacies& operator=( const Diplomacies& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::map< const Diplomacies*, ASN1T_EnumDiplomatie > T_Diplomacies;
    typedef T_Diplomacies::const_iterator                      CIT_Diplomacies;
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgSideCreation& message );
    virtual void DoUpdate( const ASN1T_MsgChangeDiplomatieAck& message );
    virtual void DoUpdate( const ASN1T_MsgChangeDiplomatie& message );
    template< typename T >
    void UpdateData( const T& message );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::Resolver_ABC< kernel::Team_ABC >& resolver_;
    T_Diplomacies diplomacies_;
    char karma_;
    //@}
};

#endif // __Diplomacies_h_
