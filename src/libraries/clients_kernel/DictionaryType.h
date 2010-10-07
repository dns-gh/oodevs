// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __DictionaryType_h_
#define __DictionaryType_h_

#include "OptionalValue.h"
#include "tools/Resolver.h"

namespace xml
{
    class xistream;
}

namespace kernel
{
class DictionaryEntryType;

// =============================================================================
/** @class  DictionaryType
    @brief  DictionaryType
*/
// Created: JSR 2010-10-01
// =============================================================================
class DictionaryType : public tools::Resolver< DictionaryEntryType >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit DictionaryType( xml::xistream& xis );
    virtual ~DictionaryType();
    //@}

    //! @name Operations
    //@{
    void GetStringList( QStringList& list, const std::string& kind, const std::string& language ) const;
    const std::string& GetString( unsigned int id, const std::string& kind, const std::string& language ) const;
    int GetId( const std::string& string, const std::string& kind, const std::string& language ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DictionaryType( const DictionaryType& );            //!< Copy constructor
    DictionaryType& operator=( const DictionaryType& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadEntry( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    OptionalValue< std::string > alias_;
    //@}
};

}

#endif // __DictionaryType_h_
