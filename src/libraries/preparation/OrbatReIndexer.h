// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __OrbatReIndexer_h_
#define __OrbatReIndexer_h_

namespace xml
{
    class xistream;
    class xostream;
}

class IdManager;
class OrbatImportFilter;

// =============================================================================
/** @class  OrbatReIndexer
    @brief  OrbatReIndexer
*/
// Created: SBO 2008-04-08
// =============================================================================
class OrbatReIndexer
{

public:
    //! @name Constructors/Destructor
    //@{
             OrbatReIndexer( xml::xistream& input, xml::xostream& ouput, IdManager& idManager, const OrbatImportFilter& filter );
    virtual ~OrbatReIndexer();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    OrbatReIndexer( const OrbatReIndexer& );            //!< Copy constructor
    OrbatReIndexer& operator=( const OrbatReIndexer& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadField( const std::string& name, xml::xistream& xis, const OrbatImportFilter& filter );
    void ReadAttribute( const std::string& name, xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    const unsigned long shift_;
    xml::xostream& output_;
    //@}
};

#endif // __OrbatReIndexer_h_
