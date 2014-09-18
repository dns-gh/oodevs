// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationFlow_ABC_h_
#define __PopulationFlow_ABC_h_

#include "PopulationPart_ABC.h"

namespace kernel
{

// =============================================================================
/** @class  PopulationFlow_ABC
    @brief  PopulationFlow_ABC
*/
// Created: SBO 2006-08-23
// =============================================================================
class PopulationFlow_ABC : public PopulationPart_ABC
{
public:
    //! @name Static
    //@{
    static const std::string typeName_;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             PopulationFlow_ABC();
    virtual ~PopulationFlow_ABC();
    //@}

    //! @name Operations
    //@{
    virtual const std::string& GetTypeName() const;

    virtual void Select( ActionController& controller ) const;
    virtual void MultipleSelect( ActionController& controller, const std::vector< const kernel::GraphicalEntity_ABC* >& elements ) const;
    virtual void ContextMenu( ActionController& controller, const QPoint& where, QObject* emitter ) const;
    virtual void Activate( ActionController& controller ) const;
    virtual geometry::Point2f GetNearestPosition( const geometry::Point2f& position ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PopulationFlow_ABC( const PopulationFlow_ABC& );            //!< Copy constructor
    PopulationFlow_ABC& operator=( const PopulationFlow_ABC& ); //!< Assignment operator
    //@}
};

}

#endif // __PopulationFlow_ABC_h_
