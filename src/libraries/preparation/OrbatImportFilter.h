// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __OrbatImportFilter_h_
#define __OrbatImportFilter_h_

// =============================================================================
/** @class  OrbatImportFilter
    @brief  OrbatImportFilter
*/
// Created: SBO 2008-04-08
// =============================================================================
class OrbatImportFilter
{

public:
    //! @name Constructors/Destructor
    //@{
             OrbatImportFilter( bool objects, bool populations );
    virtual ~OrbatImportFilter();
    //@}

    //! @name Operations
    //@{
    bool Filters( const std::string& field ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    OrbatImportFilter( const OrbatImportFilter& );            //!< Copy constructor
    OrbatImportFilter& operator=( const OrbatImportFilter& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const bool objects_;
    const bool populations_;
    //@}
};

#endif // __OrbatImportFilter_h_
