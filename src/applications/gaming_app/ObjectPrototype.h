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
#include "game_asn/SimulationSenders.h"
#include "LocationSerializer.h"

namespace kernel
{
    class ObjectType;
}

namespace gui
{
    class SymbolIcons;
}

class StaticModel;
class Publisher_ABC;


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
             ObjectPrototype( QWidget* parent, kernel::Controllers& controllers, const StaticModel& model, gui::ParametersLayer& layer, gui::SymbolIcons& icons );
    virtual ~ObjectPrototype();
    //@}

    //! @name Operations
    //@{
    void Commit( Publisher_ABC& publisher );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ObjectPrototype( const ObjectPrototype& );            //!< Copy constructor
    ObjectPrototype& operator=( const ObjectPrototype& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    ASN1T_MagicActionCreateObject& GetMessage();
    const kernel::ObjectType& GetType() const;
    //@}

private:
    //! @name Member data
    //@{
    simulation::ObjectMagicAction msg_;
    ASN1T_MagicActionCreateObject creation_;
    LocationSerializer serializer_;
    //@}
};

#endif // __ObjectPrototype_h_
