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
    //! @name Types
    //@{
    typedef std::vector< kernel::Language > T_Languages;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             ADN_Languages_Data();
    virtual ~ADN_Languages_Data();
    //@}

    //! @name Operations
    //@{
    void FilesNeeded( tools::Path::T_Paths& files ) const;
    //@}

    //! @name Static methods
    //@{
    static const std::string& Master();
    static bool IsMaster( const std::string& language );
    static bool IsCurrentMaster();
    //@}

private:
    //! @name Helpers
    //@{
    void ReadArchive( xml::xistream& input );
    void ReadLanguage( xml::xistream& input );
    void WriteArchive( xml::xostream& output );
    //@}

public:
    //! @name Member data
    //@{
    T_Languages languages_;
    //@}

private:
    //! @name Static member data
    //@{
    static const std::string master_;
    //@}
};

#endif // __ADN_Languages_Data_h_
