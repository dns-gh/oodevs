// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Languages_Data_h_
#define __ADN_Languages_Data_h_

#include "ADN_Data_ABC.h"
#include "clients_kernel/Languages.h"

namespace kernel
{
    class Language;
}

// =============================================================================
/** @class  ADN_Languages_Data
    @brief  ADN_Languages_Data
*/
// Created: ABR 2013-07-08
// =============================================================================
class ADN_Languages_Data : public ADN_Data_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ADN_Languages_Data();
    virtual ~ADN_Languages_Data();
    //@}

    //! @name Operations
    //@{
    void FilesNeeded( tools::Path::T_Paths& files ) const;
    void PurgeActiveLanguages();
    void AddActiveLanguage( const std::string& language );
    bool HasActiveLanguage( const std::string& language ) const;
    void CleanLocalDirectories() const;
    //@}

    //! @name Accessors
    //@{
    const std::string& Master() const;
    void SetMaster( const std::string& language );
    void SwapMaster();

    bool IsMaster( const std::string& language ) const;
    bool IsMasterEmpty() const;
    bool IsCurrentMaster() const;

    const kernel::Languages& GetAllLanguages() const;
    const kernel::Languages::T_Languages& GetActiveLanguages() const;
    //@}

private:
    //! @name Helpers
    //@{
    void ReadArchive( xml::xistream& input );
    void ReadLanguage( xml::xistream& input );
    void WriteArchive( xml::xostream& output );

    void InternalSetMaster( const std::string& language );
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< const kernel::Languages > allLanguages_;
    kernel::Languages::T_Languages activeLanguages_;
    std::string master_;
    //@}
};

#endif // __ADN_Languages_Data_h_
