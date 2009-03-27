// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActivityTimePrototype_h_
#define __ActivityTimePrototype_h_

#include "clients_gui/ActivityTimePrototype_ABC.h"

struct ASN1T_MagicActionCreateObject;

// =============================================================================
/** @class  ActivityTimePrototype
    @brief  ActivityTimePrototype
*/
// Created: SBO 2006-04-19
// =============================================================================
class ActivityTimePrototype : public gui::ActivityTimePrototype_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ActivityTimePrototype( QWidget* parent, ASN1T_MagicActionCreateObject& msg );
    virtual ~ActivityTimePrototype();
    //@}

    //! @name Operations
    //@{
    virtual void Commit();
    virtual void Clean();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ActivityTimePrototype( const ActivityTimePrototype& );            //!< Copy constructor
    ActivityTimePrototype& operator=( const ActivityTimePrototype& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    ASN1T_MagicActionCreateObject& msg_;
    //@}
};

#endif // __ActivityTimePrototype_h_
