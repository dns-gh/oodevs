// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __AutomateFactory_ABC_h_
#define __AutomateFactory_ABC_h_

#include <tools/Resolver.h>
#include "Entities/Automates/MIL_Automate.h"

class MIL_AutomateType;
class MIL_DictionaryExtensions;
class MIL_Formation;

namespace xml
{
    class xistream;
}
// =============================================================================
/** @class  AutomateFactory_ABC
    @brief  AutomateFactory_ABC
*/
// Created: MGD 2009-08-13
// =============================================================================
class AutomateFactory_ABC : public tools::Resolver< MIL_Automate >
                          , private boost::noncopyable
{
public:
    //! @name Constructor/Destructor
    //@{
             AutomateFactory_ABC() {}
    virtual ~AutomateFactory_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual MIL_Automate& Create( xml::xistream& xis, MIL_Entity_ABC& parent ) = 0;
    virtual MIL_Automate& Create( const MIL_AutomateType& type, unsigned int knowledgeGroup, const std::string& name, MIL_Entity_ABC& parent, unsigned int context, const MIL_DictionaryExtensions& extensions ) = 0;
    //@}

    //! @name CheckPoint
    //@{
    template< typename Archive > void serialize( Archive& file, const unsigned int );
    //@}
};

namespace boost {
namespace serialization {
    template<>
    struct is_abstract<const AutomateFactory_ABC> : boost::true_type { } ;

    template<>
    struct is_abstract< const tools::Resolver< MIL_Automate > > : boost::true_type { } ;

    template<class Archive>
    void serialize( Archive &, tools::Resolver< MIL_Automate >&, const unsigned int ) {}

} // namespace serialization
}

// -----------------------------------------------------------------------------
// Name: template< typename Archive > void AutomateFactory_ABC::serialize
// Created: MGD 2009-10-24
// -----------------------------------------------------------------------------
template< typename Archive >
void AutomateFactory_ABC::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< tools::Resolver< MIL_Automate > >( *this );
    file & elements_;
}

#endif // __AutomateFactory_ABC_h_
