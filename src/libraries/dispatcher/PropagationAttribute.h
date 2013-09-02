 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PropagationAttribute_h_
#define __PropagationAttribute_h_

#include "ObjectAttribute_ABC.h"
#include "protocol/Protocol.h"
#include <string>

namespace dispatcher
{

// =============================================================================
/** @class  PropagationAttribute
    @brief  Propagation attribute
*/
// Created: LGY 2012-10-12
// =============================================================================
class PropagationAttribute : public ObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit PropagationAttribute( const sword::ObjectAttributes& message );
    virtual ~PropagationAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update( const sword::ObjectAttributes& message );
    virtual void Send( sword::ObjectAttributes& message ) const;
    const std::string& GetModel() const;
    const std::string& GetDate() const;
    //@}

private:
    //! @name Data member
    //@{
    std::string model_;
    std::string date_;
    //@}
};

}

#endif // __PropagationAttribute_h_
