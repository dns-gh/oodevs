// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __DictionaryEntryType_h_
#define __DictionaryEntryType_h_

namespace xml
{
    class xistream;
}

namespace kernel
{
    class EntryLabelType;

// =============================================================================
/** @class  DictionaryEntryType
    @brief  DictionaryEntryType
*/
// Created: JSR 2010-10-01
// =============================================================================
class DictionaryEntryType
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit DictionaryEntryType( xml::xistream& xis );
    virtual ~DictionaryEntryType();
    //@}

    //! @name Operations
    //@{
    std::string  GetKey() const;
    unsigned int GetId() const;
    std::string  GetLabel( const std::string& kind, const std::string& language ) const;
    std::string  GetAlias() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DictionaryEntryType( const DictionaryEntryType& );            //!< Copy constructor
    DictionaryEntryType& operator=( const DictionaryEntryType& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadLabel( xml::xistream& xis );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< EntryLabelType* > T_Labels;
    typedef T_Labels::const_iterator     CIT_Labels;
    //@}

private:
    //! @name Member data
    //@{
    const std::string            key_;
    const unsigned int           id_;
    std::string                  alias_;
    T_Labels                     labels_;
    //@}
};

}

#endif // __DictionaryEntryType_h_
