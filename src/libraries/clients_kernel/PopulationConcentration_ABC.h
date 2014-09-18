// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationConcentration_ABC_h_
#define __PopulationConcentration_ABC_h_

#include "PopulationPart_ABC.h"

namespace kernel
{

// =============================================================================
/** @class  PopulationConcentration_ABC
    @brief  PopulationConcentration_ABC
*/
// Created: SBO 2006-08-23
// =============================================================================
class PopulationConcentration_ABC : public PopulationPart_ABC
{
public:
    //! @name Static
    //@{
    static const std::string typeName_;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             PopulationConcentration_ABC();
    virtual ~PopulationConcentration_ABC();
    //@}

    //! @name Operations
    //@{
    virtual const std::string& GetTypeName() const;

    virtual void Select( ActionController& controller ) const;
    virtual void MultipleSelect( ActionController& controller, const std::vector< const kernel::GraphicalEntity_ABC* >& elements ) const;
    virtual void ContextMenu( ActionController& controller, const QPoint& where, QObject* emitter ) const;
    virtual void Activate( ActionController& controller ) const;
    //@}
};

}

#endif // __PopulationConcentration_ABC_h_
