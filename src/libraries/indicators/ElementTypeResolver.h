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

namespace indicators
{
    class ElementType;

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
    void AddElement( const ElementType& instance, const ElementType& definition );
    static std::string ToSimpleType( const std::string& type );
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
    //@}

private:
    //! @name Member data
    //@{
    std::map< std::string, std::string > dictionary_;
    std::map< const ElementType*, const ElementType* > instances_;
    //@}
};

}

#endif // __ElementTypeResolver_h_
