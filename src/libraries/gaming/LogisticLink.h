// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticLink_h_
#define __LogisticLink_h_

#include "Dotation.h"
#include <tools/Resolver_ABC.h>
#include <boost/noncopyable.hpp>

namespace sword
{
    class SeqOfDotationQuota;
}

namespace kernel
{
    class Entity_ABC;
}

namespace gui
{
    class GlTools_ABC;
    class Viewport_ABC;
}

// =============================================================================
/** @class  LogisticLink
    @brief  LogisticLink
*/
// Created: AGE 2006-02-13
// =============================================================================
class LogisticLink : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit LogisticLink( const kernel::Entity_ABC& superior );
    virtual ~LogisticLink();
    //@}

    //! @name Operations
    //@{
    virtual void Draw  ( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const;
    virtual void Update( const sword::SeqOfDotationQuota& msg, const tools::Resolver_ABC< kernel::DotationType >& dotationResolver );
    //@}

    //! @name Accessors
    //@{
    const kernel::Entity_ABC& GetSuperior() const { return superior_; }
    const std::vector< Dotation >& GetQuotas() const { return quotas_; }
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< Dotation > T_Dotations;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Entity_ABC& superior_;
    T_Dotations quotas_;
    //@}
};

#endif // __LogisticLink_h_
