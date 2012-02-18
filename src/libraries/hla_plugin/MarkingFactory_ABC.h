// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_MarkingFactory_ABC_h
#define plugins_hla_MarkingFactory_ABC_h

#include "AggregateMarking.h"
#include <boost/noncopyable.hpp>
#include <string>

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  MarkingFactory_ABC
    @brief  Marking factory definition
*/
// Created: SLI 2011-11-07
// =============================================================================
class MarkingFactory_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             MarkingFactory_ABC() {}
    virtual ~MarkingFactory_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual AggregateMarking CreateAggregateMarking( const std::string& name, unsigned int identifier ) const = 0;
    virtual Marking CreateMarking( const std::string& name, unsigned int identifier ) const = 0;
    //@}
};

}
}
#endif // plugins_hla_MarkingFactory_ABC_h
