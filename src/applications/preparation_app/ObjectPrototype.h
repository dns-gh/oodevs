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
    class Object_ABC;
}

namespace tools
{
    class GeneralConfig;
}

class ObjectAttributesContainer;
class StaticModel;
class ObjectsModel;

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
             ObjectPrototype( QWidget* parent, kernel::Controllers& controllers, const StaticModel& model, ObjectsModel& objectsModel, gui::ParametersLayer& layer, const tools::GeneralConfig& config );
    virtual ~ObjectPrototype();
    //@}

    //! @name Operations
    //@{
    virtual void Commit();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ObjectPrototype( const ObjectPrototype& );            //!< Copy constructor
    ObjectPrototype& operator=( const ObjectPrototype& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    ObjectsModel& model_;
    kernel::Object_ABC* creation_;
    ObjectAttributesContainer* container_;
    //@}
};

#endif // __ObjectPrototype_h_
