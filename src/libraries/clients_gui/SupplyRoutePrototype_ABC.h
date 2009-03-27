// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SupplyRoutePrototype_ABC_h_
#define __SupplyRoutePrototype_ABC_h_

#include "ObjectAttributePrototype_ABC.h"


namespace gui
{

// =============================================================================
/** @class  SupplyRoutePrototype_ABC
    @brief  Logistic route prototype definition
*/
// Created: SBO 2006-04-20
// =============================================================================
class SupplyRoutePrototype_ABC : public ObjectAttributePrototype_ABC
{    
public:
    //! @name Constructors/Destructor
    //@{
    explicit SupplyRoutePrototype_ABC( QWidget* parent );
    virtual ~SupplyRoutePrototype_ABC();
    //@}

    //! @name Operations
    //@{
    virtual bool CheckValidity() const;
    virtual void Commit() = 0;
    virtual void Clean() = 0;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    SupplyRoutePrototype_ABC( const SupplyRoutePrototype_ABC& );            //!< Copy constructor
    SupplyRoutePrototype_ABC& operator=( const SupplyRoutePrototype_ABC& ); //!< Assignement operator
    //@}

protected:
    //! @name Member data
    //@{
    QSpinBox* flow_;
    QSpinBox* width_;
    QSpinBox* length_;
    QSpinBox* maxWeight_;
    QCheckBox* equipped_;
    //@}
};

}

#endif // __SupplyRoutePrototype_ABC_h_
