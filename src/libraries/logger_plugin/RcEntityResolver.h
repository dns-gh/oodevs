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
#include <xeumeuleu/xml.hpp>

inline std::ostream& operator<<( std::ostream& os, const QString& s )
{
    return os << s.toStdString();
}

inline xml::xostream& operator<<( xml::xostream& xos, const QString& s )
{
    return xos << s.toStdString();
}

namespace dispatcher
{
    class Model_ABC;
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
    explicit RcEntityResolver( const dispatcher::Model_ABC& model );
    virtual ~RcEntityResolver();
    //@}

private:
    virtual QString CreateLink( const kernel::Entity_ABC* entity, const std::string& type, unsigned long id ) const;

private:
    //! @name Member data
    //@{
    const dispatcher::Model_ABC& model_;
    //@}
};
}
}

#endif // __RcEntityResolver_h_
