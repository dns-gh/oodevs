// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __ElementTypeResolver_h_
#define __ElementTypeResolver_h_

#include <boost/shared_ptr.hpp>

namespace indicators
{
    class DataType_ABC;

// =============================================================================
/** @class  ElementTypeResolver
    @brief  ElementTypeResolver
*/
// Created: SBO 2009-04-15
// =============================================================================
class ElementTypeResolver
{

public:
    //! @name Constructors/Destructor
    //@{
             ElementTypeResolver();
    virtual ~ElementTypeResolver();
    //@}

    //! @name Operations
    //@{
    std::string Resolve( const std::string& type ) const;
    void AddElement( const DataType_ABC& definition, const DataType_ABC& instance );
    static std::string ToSimpleType( const std::string& type );
    static bool IsCompatible( const std::string& lhs, const std::string& rhs );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ElementTypeResolver( const ElementTypeResolver& );            //!< Copy constructor
    ElementTypeResolver& operator=( const ElementTypeResolver& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void Update();
    void ConvertElementTypes( std::vector< std::string >& type1, std::vector< std::string >& type2 ) const;
    static bool IsNumericCompatible( const std::string& lhs, const std::string& rhs );
    //@}

    //! @name Types
    //@{
    typedef std::map< std::string, const DataType_ABC* > T_Instances;
    //@}

private:
    //! @name Member data
    //@{
    std::map< std::string, std::string > dictionary_;
    T_Instances instances_;
    //@}
};

}

#endif // __ElementTypeResolver_h_
