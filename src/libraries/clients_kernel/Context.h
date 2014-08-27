// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __Translations_h_
#define __Translations_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>

namespace kernel
{
    class LocalizedString;

// =============================================================================
/** @class  Context
    @brief  Context
*/
// Created: ABR 2013-07-15
// =============================================================================
class Context : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Context();
    virtual ~Context();
    //@}

    //! @name Operations
    //@{
    bool Apply( const std::function< bool( LocalizedString& ) >& functor );
    const boost::shared_ptr< LocalizedString >& operator[]( const std::string& key );

    bool CheckUniqueTranslation( const LocalizedString& translation );
    bool HasDuplicateErrors() const;
    void CleanTranslations();
    void MergeDuplicateTranslations();
    bool IsEmpty() const;
    void Serialize( xml::xostream& xos, const std::string& languageCode );
    void SetKey( const boost::shared_ptr< LocalizedString >& translation, const std::string& key );
    bool SwapKey( const std::string& oldKey, const std::string& newKey );
    //@}

private:
    //! @name Member data
    //@{
    std::vector< boost::shared_ptr< LocalizedString > > strings_;
    //@}
};

} //! Namespace kernel

#endif // __Translations_h_
