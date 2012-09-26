// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_PERCEPTION_PERCEPTION_RECO_OBJECTS_H
#define SWORD_PERCEPTION_PERCEPTION_RECO_OBJECTS_H

#include "PerceptionWithLocation.h"
#include "PerceptionLocalisation.h"
#include "MT_Tools/MT_Vector2D.h"
#include "wrapper/View.h"

namespace sword
{
namespace perception
{
    class PerceptionObserver_ABC;

class PerceptionRecoObjectsReco : public PerceptionLocalisation
{
public:
    explicit PerceptionRecoObjectsReco( const wrapper::View& perception, const wrapper::View& entity );

    bool IsInside        ( const wrapper::View& knowledge ) const;
    void GetObjectsInside( const wrapper::View&, const wrapper::View&, Perception_ABC::T_ObjectVector& ) const;

private:
    PerceptionRecoObjectsReco& operator = ( const PerceptionRecoObjectsReco& );

private:
    const MT_Vector2D   vCenter_;
    const wrapper::View localisation_;
    const double        rCurrentSize_;
};

// =============================================================================
// @class  PerceptionRecoObjects
// Created: JVT 2004-10-21
// =============================================================================
class PerceptionRecoObjects : public PerceptionWithLocation< PerceptionRecoObjectsReco >
{
public:
    //! @name Constructors/Destructor
    //@{
             PerceptionRecoObjects( const wrapper::View& model, const wrapper::View& entity, PerceptionObserver_ABC& observer );
    virtual ~PerceptionRecoObjects();
    //@}

    //! @name Execution
    //@{
    virtual void                   ExecuteObjects( const wrapper::View& model, const wrapper::View& perceiver, const SurfacesObject_ABC& surfaces, const T_ObjectVector& perceivableObjects );
    virtual const PerceptionLevel& ComputeObject( const wrapper::View& perceiver, const SurfacesObject_ABC& surfaces, const wrapper::View& knowledge ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void AddLocalisation( const wrapper::View& perception, const wrapper::View& entity );
    //@}

private:
    //! @name Member data
    //@{
    PerceptionObserver_ABC& observer_;
    //@}
};

}
}

#endif // SWORD_PERCEPTION_PERCEPTION_RECO_OBJECTS_H
