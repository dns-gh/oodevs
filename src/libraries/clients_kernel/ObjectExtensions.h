// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectExtensions_h_
#define __ObjectExtensions_h_

#include "Displayable_ABC.h"
#include "Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"

namespace sword
{
    class ObjectKnowledgeUpdate;
    class ObjectUpdate;
}

#define DECLARE_EXTENSION( extension )                                                      \
class extension : public kernel::Extension_ABC                                              \
                , public kernel::Displayable_ABC                                            \
                , public kernel::Updatable_ABC< sword::ObjectKnowledgeUpdate > \
                , public kernel::Updatable_ABC< sword::ObjectUpdate >          \
{                                                                                           \
public:                                                                                     \
             extension() {};                                                                \
    virtual ~extension() {};                                                                \
                                                                                            \
    virtual void DoUpdate( const sword::ObjectKnowledgeUpdate& ) {}            \
    virtual void DoUpdate( const sword::ObjectUpdate& ) {}                     \
};

namespace kernel
{
    DECLARE_EXTENSION( ConstructionAttribute_ABC )
    DECLARE_EXTENSION( MineAttribute_ABC )
    DECLARE_EXTENSION( ActivityTimeAttribute_ABC )
    DECLARE_EXTENSION( NBCAttribute_ABC )
    DECLARE_EXTENSION( BypassAttribute_ABC )
    DECLARE_EXTENSION( ObstacleAttribute_ABC )
    DECLARE_EXTENSION( LogisticAttribute_ABC )
    DECLARE_EXTENSION( CrossingSiteAttribute_ABC )
    DECLARE_EXTENSION( SupplyRouteAttribute_ABC )
    DECLARE_EXTENSION( ToxicCloudAttribute_ABC )
    DECLARE_EXTENSION( FireAttribute_ABC )
    DECLARE_EXTENSION( MedicalTreatmentAttribute_ABC )
    DECLARE_EXTENSION( StockAttribute_ABC )
    DECLARE_EXTENSION( OccupantAttribute_ABC )
    DECLARE_EXTENSION( DelayAttribute_ABC )
    DECLARE_EXTENSION( SealOffAttribute_ABC )
}

#undef DECLARE_EXTENSION

#endif // __ObjectExtensions_h_
