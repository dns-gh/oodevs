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
#include "tools/Resolver_ABC.h"
#include "clients_kernel/Serializable_ABC.h"
#include "EnumTypes.h"

namespace kernel
{
    class Team_ABC;
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
    //! @name Types
    //@{
    typedef std::map< std::string, QString >     T_Extensions;
    typedef T_Extensions::iterator              IT_Extensions;
    typedef T_Extensions::const_iterator       CIT_Extensions;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             Inhabitant( const kernel::InhabitantType& type, int number, const QString& name, kernel::Controller& controller, IdManager& idManager );
             Inhabitant( xml::xistream& xis, kernel::Controller& controller, IdManager& idManager, const tools::Resolver_ABC< kernel::InhabitantType, std::string >& types );
    virtual ~Inhabitant();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const kernel::GlTools_ABC& tools ) const;
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
    void ReadExtension( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::InhabitantType& type_;
    unsigned long                 healthy_;
    unsigned long                 wounded_;
    unsigned long                 dead_;
    QString                       text_;
    T_Extensions extensions_;
    //@}
};

#endif // __Population_h_
