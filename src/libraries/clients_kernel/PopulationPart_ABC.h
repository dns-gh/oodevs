// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationPart_ABC_h_
#define __PopulationPart_ABC_h_

#include "Entity_ABC.h"

namespace kernel
{

// =============================================================================
/** @class  PopulationPart_ABC
    @brief  PopulationPart_ABC
*/
// Created: SBO 2006-08-23
// =============================================================================
class PopulationPart_ABC : public Entity_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             PopulationPart_ABC() {}
    virtual ~PopulationPart_ABC() {}
    //@}

    //! @name Accessors
    //@{
    virtual QString GetName() const = 0;
    virtual unsigned long GetId() const = 0;
    virtual unsigned int GetDeadHumans() const = 0;
    virtual unsigned int GetLivingHumans() const = 0;
    virtual unsigned int GetDensity() const = 0;
    virtual std::string  GetAttitude() const = 0;
    //@}

    //! @name Operations
    //@{
    virtual void Select( ActionController& controller ) const = 0;
    virtual void ContextMenu( ActionController& controller, const QPoint& where ) const = 0;
    virtual void Activate( ActionController& controller ) const = 0;
    //@}
};

}

#endif // __PopulationPart_ABC_h_
