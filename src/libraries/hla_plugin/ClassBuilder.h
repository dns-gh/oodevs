// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_ClassBuilder_h
#define plugins_hla_ClassBuilder_h

#include "ClassBuilder_ABC.h"

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  ClassBuilder
    @brief  Class factory
*/
// Created: SLI 2011-07-26
// =============================================================================
class ClassBuilder : public ClassBuilder_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ClassBuilder();
    virtual ~ClassBuilder();
    //@}

    //! @name Operations
    //@{
    virtual void Build( Federate_ABC& federate, ::hla::Class< HlaObject_ABC >& hlaClass, bool publish, bool subscribe ) const;
    //@}
};

}
}

#endif // plugins_hla_ClassBuilder_h
