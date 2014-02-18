// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PropertyGroup_h_
#define __PropertyGroup_h_

#include "RenderingContext_ABC.h"

namespace svg
{
    class PropertyFactory;
    class Property_ABC;
    class References_ABC;

// =============================================================================
/** @class  PropertyGroup
    @brief  PropertyGroup
*/
// Created: AGE 2006-08-15
// =============================================================================
class PropertyGroup
{

public:
    //! @name Constructors/Destructor
    //@{
             PropertyGroup( const std::string& css, const PropertyFactory& factory );
             PropertyGroup( xml::xistream& input, const PropertyFactory& factory );
             PropertyGroup( const PropertyGroup& rhs );
    virtual ~PropertyGroup();
    //@}

    //! @name Operations
    //@{
    void Add( RenderingContext_ABC::E_Properties, Property_ABC& property );
    void Push( RenderingContext_ABC& context, References_ABC& references ) const;
    void Pop ( RenderingContext_ABC& context, References_ABC& references ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PropertyGroup& operator=( const PropertyGroup& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::pair< RenderingContext_ABC::E_Properties, Property_ABC* > T_Property;
    typedef std::vector< T_Property >                                      T_Properties;
    typedef T_Properties::const_iterator                                 CIT_Properties;
    //@}

private:
    //! @name Member data
    //@{
    T_Properties properties_;
    std::string class_;
    //@}
};

}

#endif // __PropertyGroup_h_
