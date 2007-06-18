// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CrossingSitePrototype_h_
#define __CrossingSitePrototype_h_

#include "clients_gui/CrossingSitePrototype_ABC.h"

struct ASN1T_MagicActionCreateObject;
struct ASN1T_ObjectAttributesCrossingSite;

// =============================================================================
/** @class  CrossingSitePrototype
    @brief  CrossingSitePrototype
*/
// Created: SBO 2006-04-19
// =============================================================================
class CrossingSitePrototype : public gui::CrossingSitePrototype_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             CrossingSitePrototype( QWidget* parent, ASN1T_MagicActionCreateObject& msg );
    virtual ~CrossingSitePrototype();
    //@}

    //! @name Operations
    //@{
    virtual void Commit();
    virtual void Clean();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    CrossingSitePrototype( const CrossingSitePrototype& );            //!< Copy constructor
    CrossingSitePrototype& operator=( const CrossingSitePrototype& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    ASN1T_MagicActionCreateObject& msg_;
    ASN1T_ObjectAttributesCrossingSite* attr_;
    //@}
};

#endif // __CrossingSitePrototype_h_
