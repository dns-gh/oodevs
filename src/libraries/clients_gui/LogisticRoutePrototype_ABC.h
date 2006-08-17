// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticRoutePrototype_ABC_h_
#define __LogisticRoutePrototype_ABC_h_

#include "ObjectPrototypeAttributes_ABC.h"

// =============================================================================
/** @class  LogisticRoutePrototype_ABC
    @brief  Logistic route prototype definition
*/
// Created: SBO 2006-04-20
// =============================================================================
class LogisticRoutePrototype_ABC : public ObjectPrototypeAttributes_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit LogisticRoutePrototype_ABC( QWidget* parent );
    virtual ~LogisticRoutePrototype_ABC();
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
    LogisticRoutePrototype_ABC( const LogisticRoutePrototype_ABC& );            //!< Copy constructor
    LogisticRoutePrototype_ABC& operator=( const LogisticRoutePrototype_ABC& ); //!< Assignement operator
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

#endif // __LogisticRoutePrototype_ABC_h_
