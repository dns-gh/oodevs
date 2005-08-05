// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2005-08-05 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifndef __MT_ArchiveDirectoryHelper_h_
#define __MT_ArchiveDirectoryHelper_h_

// =============================================================================
/** @class  MT_ArchiveDirectoryHelper
    @brief  Crappy Name ! 
            Changes current directory on creation. Pop back on initial directory on destruction
    @par    Using example
    @code
    MT_ArchiveDirectoryHelper directoryChanger( strNextArchiveFile );
    @endcode
*/
// Created: AGE 2005-08-05
// =============================================================================
class MT_ArchiveDirectoryHelper
{

public:
    //! @name Constructors/Destructor
    //@{
             MT_ArchiveDirectoryHelper( const std::string& strNextArchiveFile );
    virtual ~MT_ArchiveDirectoryHelper();
    //@}
   
private:
    //! @name Copy/Assignement
    //@{
    MT_ArchiveDirectoryHelper( const MT_ArchiveDirectoryHelper& );            //!< Copy constructor
    MT_ArchiveDirectoryHelper& operator=( const MT_ArchiveDirectoryHelper& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    const std::string strInitialDirectory_;
    //@}
};

#endif // __MT_ArchiveDirectoryHelper_h_
