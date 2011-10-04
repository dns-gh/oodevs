// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_NetnClassBuilder_h
#define plugins_hla_NetnClassBuilder_h

#include "ClassBuilder_ABC.h"

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  NetnClassBuilder
    @brief  Netn class builder
*/
// Created: SLI 2011-07-26
// =============================================================================
class NetnClassBuilder : public ClassBuilder_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit NetnClassBuilder( const ClassBuilder_ABC& builder );
    virtual ~NetnClassBuilder();
    //@}

    //! @name Operations
    //@{
    virtual void BuildAggregate( Federate_ABC& federate, ::hla::Class< HlaObject_ABC >& hlaClass, bool publish, bool subscribe ) const;
    virtual void BuildSurfaceVessel( Federate_ABC& federate, ::hla::Class< HlaObject_ABC >& hlaClass, bool publish, bool subscribe ) const;
    //@}

private:
    //! @name Member data
    //@{
    const ClassBuilder_ABC& builder_;
    //@}
};

}
}

#endif // __NetnClassBuilder_h_
