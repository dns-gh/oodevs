// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __NBCAttribute_h_
#define __NBCAttribute_h_

#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/Serializable_ABC.h"
#include "EnumTypes.h"
#include <tools/Resolver_ABC.h>
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Displayer_ABC;
    class NBCAgent;
    class Entity_ABC;
}

namespace gui
{
    class PropertiesDictionary;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  NBCAttribute
    @brief  NBCAttribute
*/
// Created: AGE 2006-02-14
// =============================================================================
class NBCAttribute : public kernel::NBCAttribute_ABC
                   , public kernel::Serializable_ABC
                   , private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef std::vector< kernel::NBCAgent* > T_NBCAgents;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    explicit NBCAttribute( gui::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity );
             NBCAttribute( xml::xistream& xis, const tools::Resolver_ABC< kernel::NBCAgent, std::string >& nbcAgents,
                           gui::PropertiesDictionary& dico, const kernel::Entity_ABC& entity );
    virtual ~NBCAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void SerializeObjectAttributes( xml::xostream& xos ) const;
    //@}

    //! @name Modifiers
    //@{
    void AddAgent( const kernel::NBCAgent& agent );
    void SetState( const std::string& state );
    void SetDanger( const unsigned int value );
    //@}

private:
    //! @name Helpers
    //@{
    void CreateDictionary( gui::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity );
    void ReadNbcAgent( xml::xistream& xis, const tools::Resolver_ABC< kernel::NBCAgent, std::string >& nbcAgents );
    //@}

public:
    //! @name Member data
    //@{
    T_NBCAgents   agents_;
    Enum_NbcState state_;
    int           danger_;
    //@}
};

#endif // __NBCAttribute_h_
