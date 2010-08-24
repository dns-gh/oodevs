// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __RcEntityResolver_h_
#define __RcEntityResolver_h_

#include "reports/RcEntityResolver_ABC.h"

namespace dispatcher
{
    class Model;
}

namespace plugins
{
namespace logger
{
// =============================================================================
/** @class  RcEntityResolver
    @brief  RcEntityResolver
*/
// Created: LDC 2010-03-18
// =============================================================================
class RcEntityResolver : public RcEntityResolver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit RcEntityResolver( const dispatcher::Model& model );
    virtual ~RcEntityResolver();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    RcEntityResolver( const RcEntityResolver& );            //!< Copy constructor
    RcEntityResolver& operator=( const RcEntityResolver& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual QString CreateLink( const QString& type, unsigned long id ) const;
    //@}

private:
    //! @name Member data
    //@{
    const dispatcher::Model& model_;
    //@}
};
}
}

#endif // __RcEntityResolver_h_
