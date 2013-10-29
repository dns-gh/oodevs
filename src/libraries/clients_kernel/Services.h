// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef CLIENTS_KERNEL_SERVICES_H__
#define CLIENTS_KERNEL_SERVICES_H__

#include <boost/noncopyable.hpp>
#include <set>

namespace sword
{
    enum EnumService;
}

namespace kernel
{
// =============================================================================
/** @class  Services
    @brief  Services
*/
// Created: AGE 2008-08-13
// =============================================================================
class Services : public boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Services();
    virtual ~Services();
    //@}

    //! @name Methods
    //@{
    bool Has( sword::EnumService id ) const;
    void Declare( sword::EnumService id );
    void Clear();
    //@}

private:
    //! @name Member data
    //@{
    std::set< sword::EnumService > services_;
    //@}
};
}

#endif // __Services_h_
