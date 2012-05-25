// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UrbanExtensions_h_
#define __UrbanExtensions_h_

#include "Displayable_ABC.h"
#include "Extension_ABC.h"
#include "Serializable_ABC.h"
#include "Updatable_ABC.h"

namespace sword
{
    class UrbanUpdate;
    class UrbanCreation;
}

#define DECLARE_EXTENSION( extension )                                 \
class extension : public kernel::Extension_ABC                         \
                , public kernel::Displayable_ABC                       \
                , public kernel::Updatable_ABC< sword::UrbanUpdate >   \
                , public kernel::Updatable_ABC< sword::UrbanCreation > \
{                                                                      \
public:                                                                \
             extension() {}                                            \
    virtual ~extension() {}                                            \
    virtual void DoUpdate( const sword::UrbanCreation& ) {}            \
    virtual void DoUpdate( const sword::UrbanUpdate& ) {}              \
};

namespace kernel
{
    DECLARE_EXTENSION( StructuralStateAttribute_ABC )
    DECLARE_EXTENSION( InfrastructureAttribute_ABC )
}

#undef DECLARE_EXTENSION

#endif // __UrbanExtensions_h_
