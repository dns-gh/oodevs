// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectPrototypeCampAttributes_h_
#define __ObjectPrototypeCampAttributes_h_

#include "astec_gui/CampPrototype_ABC.h"

struct ASN1T_MagicActionCreateObject;
struct ASN1T_AttrObjectCampPrisonniers;
struct ASN1T_AttrObjectCampRefugies;

// =============================================================================
/** @class  CampPrototype
    @brief  CampPrototype
*/
// Created: SBO 2006-04-19
// =============================================================================
class CampPrototype : public CampPrototype_ABC
{

public:
    //! @name Constructor/Destructor
    //@{
             CampPrototype( QWidget* parent, Controllers& controllers, ASN1T_MagicActionCreateObject& msg );
    virtual ~CampPrototype();
    //@}

    //! @name Operations
    //@{
    virtual void Commit();
    virtual void Clean();
    //@}

private:
    //! @name Copy constructor/Assignment
    //@{
    CampPrototype( const CampPrototype& );
    CampPrototype& operator=( const CampPrototype& );
    //@}

private:
    //! @name Member Data
    //@{
    ASN1T_MagicActionCreateObject& msg_;
    ASN1T_AttrObjectCampPrisonniers* attrPrisonners_;
    ASN1T_AttrObjectCampRefugies* attrRefugees_;
    //@}
};

#endif // __ObjectPrototypeCampAttributes_h_
