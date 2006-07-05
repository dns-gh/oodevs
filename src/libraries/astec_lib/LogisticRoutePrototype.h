// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticRoutePrototype_h_
#define __LogisticRoutePrototype_h_

#include "ObjectPrototypeAttributes_ABC.h"

struct ASN1T_AttrObjectItineraireLogistique;

// =============================================================================
/** @class  LogisticRoutePrototype
    @brief  LogisticRoutePrototype
*/
// Created: SBO 2006-04-20
// =============================================================================
class LogisticRoutePrototype : public ObjectPrototypeAttributes_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit LogisticRoutePrototype( QWidget* parent );
    virtual ~LogisticRoutePrototype();
    //@}

    //! @name Operations
    //@{
    virtual bool CheckValidity() const;
    virtual void Serialize( ASN1T_MagicActionCreateObject& msg );
    virtual void Clean();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    LogisticRoutePrototype( const LogisticRoutePrototype& );            //!< Copy constructor
    LogisticRoutePrototype& operator=( const LogisticRoutePrototype& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    QSpinBox* flow_;
    QSpinBox* width_;
    QSpinBox* length_;
    QSpinBox* maxWeight_;
    QCheckBox* equipped_;
    ASN1T_AttrObjectItineraireLogistique* attr_;
    //@}
};

#endif // __LogisticRoutePrototype_h_
