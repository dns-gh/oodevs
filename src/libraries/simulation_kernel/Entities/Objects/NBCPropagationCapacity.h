// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __NBCPropagationCapacity_h_
#define __NBCPropagationCapacity_h_

#include "PropagationCapacity_ABC.h"
#include <boost/serialization/export.hpp>

class MIL_PropagationManager;

class NBCPropagationCapacity : public PropagationCapacity_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit NBCPropagationCapacity( MIL_PropagationManager& manager );
             NBCPropagationCapacity();
    virtual ~NBCPropagationCapacity();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    virtual void Register( MIL_Object_ABC& /*object*/ );
    //@}

    //! @name virtual
    //@{
    virtual void Instanciate( MIL_Object_ABC& object ) const;
    virtual void Update( MIL_Object_ABC& object, float time );
    //@}

private:
    //! @name Copy
    //@{
    NBCPropagationCapacity( const NBCPropagationCapacity& from );
    //@}

    //! @name Update
    //@{
    void UpdateShape( MIL_Object_ABC& object , MT_Vector2D vNormalizedWind , MT_Vector2D vPerpendicularToWind , MT_Float windSpeed );
    bool UpdateState( MIL_Object_ABC& object , MT_Vector2D vNormalizedWind , MT_Vector2D vPerpendicularToWind , MT_Float windSpeed );
    void Propagate( const MT_Vector2D& vOrigin, MIL_Object_ABC& object );
    //@}

    //! @name Helpers
    //@{
    TER_Localisation GetLocalisation( const MT_Vector2D& pt ) const;
    //@}

private:
    //! @name
    //@{
    MIL_PropagationManager* pManager_;
    float                   timeOfLastUpdate_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( NBCPropagationCapacity )

#endif
