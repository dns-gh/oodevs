// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __AttributeType_h_
#define __AttributeType_h_

#include "OptionalValue.h"

namespace xml
{
    class xistream;
}

namespace kernel
{
class EntryLabelType;

// =============================================================================
/** @class  AttributeType
    @brief  AttributeType
*/
// Created: JSR 2010-10-01
// =============================================================================
class AttributeType
{
public:
    //! @name Types
    //@{
    enum EType
    {
        ETypeBoolean,
        ETypeString,
        ETypeAlphanumeric,
        ETypeNumeric,
        ETypeDictionary,
        ETypeLoosyDictionary,
        ETypeDiffusionList
    };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    explicit AttributeType( xml::xistream& xis );
    virtual ~AttributeType();
    //@}

    //! @name Operations
    //@{
    bool Extends( const std::string& type ) const;
    EType GetType() const { return type_; }
    const std::string& GetName() const { return name_; }
    const std::string& GetLabel( const std::string& language, const std::string& kind ) const;
    void GetMinMaxLength( int& min, int& max ) const;
    void GetDictionaryValues( std::string& dictionary, std::string& kind, std::string& language ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AttributeType( const AttributeType& );            //!< Copy constructor
    AttributeType& operator=( const AttributeType& ); //!< Assignment operator
    //@}

private:
    //! @name Helpers
    //@{
    void SetType( const std::string& type );
    void ReadLabel( xml::xistream& xis );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< EntryLabelType* > T_Entries;
    typedef T_Entries::const_iterator    CIT_Entries;
    //@}

private:
    //! @name Member data
    //@{
    EType type_;
    std::string name_;
    OptionalValue< std::string > extends_;
    OptionalValue< std::string > dictionary_;
    OptionalValue< unsigned int > minLength_;
    OptionalValue< unsigned int > maxLength_;
    OptionalValue< std::string > dictionaryKind_;
    OptionalValue< std::string > dictionaryLanguage_;
    T_Entries labels_;
    //@}
};

}

#endif // __AttributeType_h_
