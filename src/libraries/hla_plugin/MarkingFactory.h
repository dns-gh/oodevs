// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_MarkingFactory_h
#define plugins_hla_MarkingFactory_h

#include "MarkingFactory_ABC.h"
#include <xeumeuleu/streams/xisubstream.hpp>

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  MarkingFactory
    @brief  Marking factory
*/
// Created: SLI 2011-11-07
// =============================================================================
class MarkingFactory : public MarkingFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MarkingFactory( xml::xisubstream xis );
    virtual ~MarkingFactory();
    //@}

    //! @name Operations
    //@{
    virtual AggregateMarking CreateAggregateMarking( const std::string& name, unsigned long identifier ) const;
    virtual Marking CreateMarking( const std::string& name, unsigned long identifier ) const;
    //@}

private:
    //! @name Member data
    //@{
    bool addIdentifier_;
    //@}
};

}
}

#endif // plugins_hla_MarkingFactory_h
