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
    class AtlasNature;
}

namespace kernel
{
    class AtlasNatures;
}

namespace actions {
    namespace parameters {

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
             AtlasNature( const kernel::OrderParameter& parameter, const kernel::AtlasNature& nature );
             AtlasNature( const kernel::OrderParameter& parameter, const sword::AtlasNature& asn, const kernel::AtlasNatures& natures );
             AtlasNature( const kernel::OrderParameter& parameter, xml::xistream& xis, const kernel::AtlasNatures& natures );
    virtual ~AtlasNature();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void CommitTo( sword::MissionParameter& asn ) const;
    virtual void CommitTo( sword::MissionParameter_Value& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AtlasNature( const AtlasNature& );            //!< Copy constructor
    AtlasNature& operator=( const AtlasNature& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void Serialize( xml::xostream& xos ) const;
    //@}
};

    }
}

#endif // __ActionParameterAtlasNature_h_
