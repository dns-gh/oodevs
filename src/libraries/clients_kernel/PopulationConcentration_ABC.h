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
    static const QString typeName_;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             PopulationConcentration_ABC();
    virtual ~PopulationConcentration_ABC();
    //@}

    //! @name Operations
    //@{
    virtual QString GetTypeName() const;

    virtual void Select( ActionController& controller ) const;
    virtual void ContextMenu( ActionController& controller, const QPoint& where ) const;
    virtual void Activate( ActionController& controller ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PopulationConcentration_ABC( const PopulationConcentration_ABC& );            //!< Copy constructor
    PopulationConcentration_ABC& operator=( const PopulationConcentration_ABC& ); //!< Assignement operator
    //@}
};

}

#endif // __PopulationConcentration_ABC_h_
