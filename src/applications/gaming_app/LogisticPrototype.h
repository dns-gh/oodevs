// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticPrototype_h_
#define __LogisticPrototype_h_

#include "clients_gui/LogisticPrototype_ABC.h"

struct ASN1T_MagicActionCreateObject;

// =============================================================================
/** @class  LogisticPrototype
    @brief  LogisticPrototype
*/
// Created: SBO 2006-04-19
// =============================================================================
class LogisticPrototype : public gui::LogisticPrototype_ABC
{

public:
    //! @name Constructor/Destructor
    //@{
             LogisticPrototype( QWidget* parent, kernel::Controllers& controllers, ASN1T_MagicActionCreateObject& msg );
    virtual ~LogisticPrototype();
    //@}

    //! @name Operations
    //@{
    virtual void Commit();
    virtual void Clean();
    //@}

private:
    //! @name Copy constructor/Assignment
    //@{
    LogisticPrototype( const LogisticPrototype& );
    LogisticPrototype& operator=( const LogisticPrototype& );
    //@}

private:
    //! @name Member Data
    //@{
    ASN1T_MagicActionCreateObject& msg_;
    //@}
};

#endif // __LogisticPrototype_h_
