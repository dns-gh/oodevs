// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __RotaPrototype_h_
#define __RotaPrototype_h_

#include "clients_gui/RotaPrototype_ABC.h"

struct ASN1T_AttrObjectROTA;
struct ASN1T_MagicActionCreateObject;

// =============================================================================
/** @class  RotaPrototype
    @brief  RotaPrototype
*/
// Created: SBO 2006-04-20
// =============================================================================
class RotaPrototype : public RotaPrototype_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             RotaPrototype( QWidget* parent, const Resolver< NBCAgent >& resolver, ASN1T_MagicActionCreateObject& msg );
    virtual ~RotaPrototype();
    //@}

    //! @name Operations
    //@{
    virtual void Commit();
    virtual void Clean();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    RotaPrototype( const RotaPrototype& );            //!< Copy constructor
    RotaPrototype& operator=( const RotaPrototype& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    ASN1T_MagicActionCreateObject& msg_;
    ASN1T_AttrObjectROTA* attr_;
    //@}
};

#endif // __RotaPrototype_h_
