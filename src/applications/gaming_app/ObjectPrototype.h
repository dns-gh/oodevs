// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectPrototype_h_
#define __ObjectPrototype_h_

#include "LocationSerializer.h"
#include "clients_gui/ObjectPrototype_ABC.h"

namespace MsgsClientToSim
{
    class MsgMagicActionCreateObject;
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
             ObjectPrototype( QWidget* parent, kernel::Controllers& controllers, const StaticModel& model, gui::ParametersLayer& layer );
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

private:
    //! @name Member data
    //@{
    MsgsClientToSim::MsgMagicActionCreateObject creation_;
    LocationSerializer serializer_;
    //@}
};

#endif // __ObjectPrototype_h_
