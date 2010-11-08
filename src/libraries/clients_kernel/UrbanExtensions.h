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
#include "clients_kernel/Updatable_ABC.h"

namespace MsgsSimToClient
{
    class MsgUrbanUpdate;
    class MsgUrbanCreation;
}

#define DECLARE_EXTENSION( extension )                                                      \
class extension : public kernel::Extension_ABC                                              \
                , public kernel::Displayable_ABC                                            \
                , public kernel::Updatable_ABC< MsgsSimToClient::MsgUrbanUpdate >           \
                , public kernel::Updatable_ABC< MsgsSimToClient::MsgUrbanCreation >         \
{                                                                                           \
public:                                                                                     \
             extension() {};                                                                \
    virtual ~extension() {};                                                                \
};

namespace kernel
{
    DECLARE_EXTENSION( StructuralStateAttribute_ABC )
}

#undef DECLARE_EXTENSION

#endif // __UrbanExtensions_h_
