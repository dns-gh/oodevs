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

#include "clients_gui/LogisticRoutePrototype_ABC.h"

struct ASN1T_ObjectAttributesLogisticRoute;
struct ASN1T_MagicActionCreateObject;

// =============================================================================
/** @class  LogisticRoutePrototype
    @brief  LogisticRoutePrototype
*/
// Created: SBO 2006-04-20
// =============================================================================
class LogisticRoutePrototype : public gui::LogisticRoutePrototype_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             LogisticRoutePrototype( QWidget* parent, ASN1T_MagicActionCreateObject& msg );
    virtual ~LogisticRoutePrototype();
    //@}

    //! @name Operations
    //@{
    virtual void Commit();
    virtual void Clean();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    LogisticRoutePrototype( const LogisticRoutePrototype& );            //!< Copy constructor
    LogisticRoutePrototype& operator=( const LogisticRoutePrototype& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    ASN1T_MagicActionCreateObject& msg_;
    ASN1T_ObjectAttributesLogisticRoute* attr_;
    //@}
};

#endif // __LogisticRoutePrototype_h_
