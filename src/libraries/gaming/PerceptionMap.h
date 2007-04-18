// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PerceptionMap_h_
#define __PerceptionMap_h_

#include "game_asn/Asn.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Resolver_ABC.h"
#include "Perception.h"

namespace kernel
{
    class Automat_ABC;
    class Controller;
}

// =============================================================================
/** @class  PerceptionMap
    @brief  PerceptionMap
*/
// Created: AGE 2006-02-22
// =============================================================================
class PerceptionMap : public kernel::Extension_ABC
                    , public kernel::Updatable_ABC< ASN1T_MsgUnitKnowledgeUpdate >
{

public:
    //! @name Constructors/Destructor
    //@{
             PerceptionMap( kernel::Controller& controller, const kernel::Resolver_ABC< kernel::Automat_ABC >& resolver );
    virtual ~PerceptionMap();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PerceptionMap( const PerceptionMap& );            //!< Copy constructor
    PerceptionMap& operator=( const PerceptionMap& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgUnitKnowledgeUpdate& message );
    //@}

    //! @name Types
    //@{
    typedef std::vector< Perception > T_Perceptions;
    //@}

public:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::Resolver_ABC< kernel::Automat_ABC >& resolver_;
    T_Perceptions perceptions_;
    //@}
};

#endif // __PerceptionMap_h_
