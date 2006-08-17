// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __NBCPrototype_h_
#define __NBCPrototype_h_

#include "clients_gui/NBCPrototype_ABC.h"

struct ASN1T_AttrObjectZoneNBC;
struct ASN1T_AttrObjectNuageNBC;
struct ASN1T_MagicActionCreateObject;

// =============================================================================
/** @class  NBCPrototype
    @brief  NBCPrototype
*/
// Created: SBO 2006-04-20
// =============================================================================
class NBCPrototype : public NBCPrototype_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             NBCPrototype( QWidget* parent, const Resolver< NBCAgent >& resolver, ASN1T_MagicActionCreateObject& msg );
    virtual ~NBCPrototype();
    //@}

    //! @name Operations
    //@{
    virtual void Commit();
    virtual void Clean();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    NBCPrototype( const NBCPrototype& );            //!< Copy constructor
    NBCPrototype& operator=( const NBCPrototype& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    ASN1T_MagicActionCreateObject& msg_;
    ASN1T_AttrObjectZoneNBC* nbcArea_;
    ASN1T_AttrObjectNuageNBC* nbcCloud_;
    //@}
};

#endif // __NBCPrototype_h_
