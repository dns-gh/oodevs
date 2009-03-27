// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectPrototype_h_
#define __ObjectPrototype_h_

#include "clients_gui/ObjectPrototype_ABC.h"

namespace kernel
{
    class ObjectType;
    class Object_ABC;
}

class ObjectAttributesContainer;
class StaticModel;
class TeamsModel;

// =============================================================================
/** @class  ObjectPrototype
    @brief  Object prototype
*/
// Created: SBO 2006-04-18
// =============================================================================
class ObjectPrototype : public gui::ObjectPrototype_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ObjectPrototype( QWidget* parent, kernel::Controllers& controllers, const StaticModel& model, TeamsModel& teamsModel, gui::ParametersLayer& layer, gui::SymbolIcons& icons );
    virtual ~ObjectPrototype();
    //@}

    //! @name Operations
    //@{
    void Commit();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ObjectPrototype( const ObjectPrototype& );            //!< Copy constructor
    ObjectPrototype& operator=( const ObjectPrototype& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    const kernel::ObjectType& GetType() const;
    //@}

private:
    //! @name Member data
    //@{
    TeamsModel&                 teamsModel_;
    kernel::Object_ABC*         creation_;
    ObjectAttributesContainer*  container_;   
    //@}
};

#endif // __ObjectPrototype_h_
