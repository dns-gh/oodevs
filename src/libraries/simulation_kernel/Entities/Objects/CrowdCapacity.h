// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CrowdCapacity_h_
#define __CrowdCapacity_h_

#include "ObjectCapacity_ABC.h"
#include "MIL.h"
#include <boost/serialization/export.hpp>

class MIL_PopulationType;
class MIL_Entity_ABC;

// =============================================================================
/** @class  CrowdCapacity
    @brief  Mobility capacity
*/
// Created: JCR 2008-06-02
// =============================================================================
class CrowdCapacity : public ObjectCapacity_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             CrowdCapacity();
    explicit CrowdCapacity( const MIL_PopulationType& type, double density );
    virtual ~CrowdCapacity();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Operations
    //@{
    virtual void Register( MIL_Object_ABC& object );
    virtual void Instanciate( MIL_Object_ABC& object ) const;
    double ApplySpeedPolicy( const MIL_Entity_ABC& entity ) const;
    double GetDensity() const;
    void SetDensityFactor( double densityFactor );
    //@}

private:
    //! @name Copy
    //@{
    CrowdCapacity( const CrowdCapacity& from );
    //@}

private:
    //! @name Member data
    //@{
    const MIL_PopulationType* type_;
    double densityFactor_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( CrowdCapacity )

#endif
