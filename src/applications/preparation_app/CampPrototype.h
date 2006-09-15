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

#include "clients_gui/CampPrototype_ABC.h"

namespace kernel
{
    class Object_ABC;
}

class ObjectsModel;

// =============================================================================
/** @class  CampPrototype
    @brief  CampPrototype
*/
// Created: SBO 2006-04-19
// =============================================================================
class CampPrototype : public gui::CampPrototype_ABC
{

public:
    //! @name Constructor/Destructor
    //@{
             CampPrototype( QWidget* parent, kernel::Controllers& controllers, kernel::Object_ABC*& creation );
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
    //! @name Member data
    //@{
    kernel::Object_ABC*& creation_;
    //@}
};

#endif // __ObjectPrototypeCampAttributes_h_
