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

class TER_Localisation;

namespace sword
{
namespace perception
{
    class PerceptionObserver_ABC;

class PerceptionRecoObjectsReco : public PerceptionLocalisation
{
public:
    explicit PerceptionRecoObjectsReco( const wrapper::View& perception, const wrapper::View& entity );

    bool IsInside        ( const DEC_Knowledge_Object& knowledge )         const;
    void GetObjectsInside( const wrapper::View&, Perception_ABC::T_ObjectVector& ) const;


private:
    PerceptionRecoObjectsReco& operator = ( const PerceptionRecoObjectsReco& );

private:
    const MT_Vector2D       vCenter_;
    const TER_Localisation* localisation_;
    const double            rCurrentSize_;
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
    virtual void                   Execute( const wrapper::View& perceiver, const SurfacesObject_ABC& surfaces, const T_ObjectVector& perceivableObjects );
    virtual const PerceptionLevel& Compute( const wrapper::View& perceiver, const SurfacesObject_ABC& surfaces, const DEC_Knowledge_Object& knowledge ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void AddLocalisation( const std::string& /*key*/, const wrapper::View& perception, const wrapper::View& entity );
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
