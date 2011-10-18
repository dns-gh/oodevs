// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Inhabitant_h_
#define __Inhabitant_h_

#include "clients_kernel/EntityImplementation.h"
#include "clients_kernel/Inhabitant_ABC.h"
#include "tools/Resolver.h"
#include "clients_kernel/Serializable_ABC.h"

namespace kernel
{
    class Controller;
    class InhabitantType;
}

namespace xml
{
    class xistream;
}

class IdManager;

// =============================================================================
/** @class  Inhabitant
    @brief  Inhabitant
*/
// Created: SBO 2006-11-08
// =============================================================================
class Inhabitant : public kernel::EntityImplementation< kernel::Inhabitant_ABC >
                 , public kernel::Extension_ABC
                 , public kernel::Serializable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Inhabitant( const kernel::InhabitantType& type, int number, const QString& name, kernel::Controller& controller, IdManager& idManager );
             Inhabitant( xml::xistream& xis, kernel::Controller& controller, IdManager& idManager, const tools::StringResolver< kernel::InhabitantType >& types );
    virtual ~Inhabitant();
    //@}

public:
    //! @name Operations
    //@{
    virtual const kernel::InhabitantType& GetType() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Inhabitant( const Inhabitant& );            //!< Copy constructor
    Inhabitant& operator=( const Inhabitant& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( kernel::Controller& controller );
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::InhabitantType& type_;
    float healthNeed_;
    QString text_;
    //@}
};

#endif // __Population_h_
