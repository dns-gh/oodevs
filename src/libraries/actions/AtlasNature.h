// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterAtlasNature_h_
#define __ActionParameterAtlasNature_h_

#include "Parameter.h"
#include "clients_kernel/AtlasNature.h"

namespace sword
{
    class Nature;
}

namespace kernel
{
    class AtlasNatures;
}

namespace actions
{
namespace parameters
{
// =============================================================================
/** @class  AtlasNature
    @brief  AtlasNature
*/
// Created: SBO 2007-05-24
// =============================================================================
class AtlasNature : public Parameter< kernel::AtlasNature >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit AtlasNature( const kernel::OrderParameter& parameter );
             AtlasNature( const kernel::OrderParameter& parameter, const kernel::AtlasNature& nature );
             AtlasNature( const kernel::OrderParameter& parameter, const sword::Nature& asn, const kernel::AtlasNatures& natures );
    virtual ~AtlasNature();
    //@}

    //! @name Operations
    //@{
    virtual QString GetDisplayName( kernel::DisplayExtractor_ABC& extractor ) const;
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void CommitTo( sword::MissionParameter& asn ) const;
    virtual void CommitTo( sword::MissionParameter_Value& asn ) const;
    virtual void Accept( ParameterVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual std::string SerializeType() const;
    virtual void Serialize( xml::xostream& xos ) const;
    //@}
};

}
}

#endif // __ActionParameterAtlasNature_h_
