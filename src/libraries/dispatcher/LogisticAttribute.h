 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticAttribute_h_
#define __LogisticAttribute_h_

#include "ObjectAttribute_ABC.h"
#include "tools/Resolver.h"
#include <boost/noncopyable.hpp>

namespace dispatcher
{
    class LogisticEntity_ABC;
    class Automat_ABC;
    class Formation_ABC;
    class Model_ABC;

// =============================================================================
/** @class  LogisticAttribute
    @brief  LogisticAttribute
*/
// Created: NLD 2006-09-19
// =============================================================================
class LogisticAttribute : private boost::noncopyable
                        , public ObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             LogisticAttribute( const Model_ABC& model, const sword::ObjectAttributes& asnMsg );
    virtual ~LogisticAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update( const sword::ObjectAttributes& asnMsg );
    virtual void Send( sword::ObjectAttributes& asnMsg ) const;
    //@}

private:
    //! @name Member data
    //@{
    const tools::Resolver_ABC< dispatcher::Automat_ABC >& automats_;
    const tools::Resolver_ABC< dispatcher::Formation_ABC >& formations_;
    const LogisticEntity_ABC* pLogisticBase_;
    //@}
};

}

#endif // __LogisticAttribute_h_
