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

#include "clients_gui/ObjectPrototype_ABC.h"
#include "gaming/ASN_messages.h"
#include "clients_gui/LocationSerializer.h"

class StaticModel;
class Publisher_ABC;
class ObjectType;

// =============================================================================
/** @class  ObjectPrototype
    @brief  Object prototype
*/
// Created: SBO 2006-04-18
// =============================================================================
class ObjectPrototype : public ObjectPrototype_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ObjectPrototype( QWidget* parent, Controllers& controllers, const StaticModel& model, ParametersLayer& layer );
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
    const ObjectType& GetType() const;
    //@}

private:
    //! @name Member data
    //@{
    ASN_MsgObjectMagicAction msg_;
    ASN1T_MagicActionCreateObject creation_;
    LocationSerializer serializer_;
    //@}
};

#endif // __ObjectPrototype_h_
