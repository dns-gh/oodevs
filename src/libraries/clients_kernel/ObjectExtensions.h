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

namespace kernel
{
    class ObjectExtension : public kernel::Extension_ABC
                    , public kernel::Displayable_ABC
                    , public kernel::Updatable_ABC< sword::ObjectKnowledgeUpdate >
                    , public kernel::Updatable_ABC< sword::ObjectUpdate >
    {
    public:
                 ObjectExtension() {}
        virtual ~ObjectExtension() {}

        virtual void DoUpdate( const sword::ObjectKnowledgeUpdate& ) {}
        virtual void DoUpdate( const sword::ObjectUpdate& ) {}
    };
    class ConstructionAttribute_ABC : public ObjectExtension {};
    class MineAttribute_ABC : public ObjectExtension {};
    class ActivityTimeAttribute_ABC : public ObjectExtension {};
    class NBCAttribute_ABC : public ObjectExtension {};
    class BypassAttribute_ABC : public ObjectExtension {};
    class ObstacleAttribute_ABC : public ObjectExtension
    {
    public:
        virtual bool IsReservedObstacle() const = 0;
        virtual bool IsReservedObstacleActivated() const = 0;
    };
    class LodgingAttribute_ABC : public ObjectExtension {};
    class LogisticAttribute_ABC : public ObjectExtension {};
    class CrossingSiteAttribute_ABC : public ObjectExtension {};
    class SupplyRouteAttribute_ABC : public ObjectExtension {};
    class ToxicCloudAttribute_ABC : public ObjectExtension {};
    class FireAttribute_ABC : public ObjectExtension {};
    class BurnAttribute_ABC : public ObjectExtension {};
    class BurnSurfaceAttribute_ABC : public ObjectExtension {};
    class MedicalTreatmentAttribute_ABC : public ObjectExtension {};
    class OccupantAttribute_ABC : public ObjectExtension {};
    class DelayAttribute_ABC : public ObjectExtension {};
    class FloodAttribute_ABC : public ObjectExtension {};
    class AltitudeModifierAttribute_ABC : public ObjectExtension {};
}

#endif // __ObjectExtensions_h_
