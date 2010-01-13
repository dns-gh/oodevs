// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ADN_ActiveProtections_Data_h_
#define __ADN_ActiveProtections_Data_h_

// =============================================================================
/** @class  ADN_ActiveProtections_Data
    @brief  ADN_ActiveProtections_Data
*/
// Created: LDC 2010-01-13
// =============================================================================
class ADN_ActiveProtections_Data : public ADN_Data_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ADN_ActiveProtections_Data();
    virtual ~ADN_ActiveProtections_Data();
    //@}

    //! @name Operations
    //@{
    virtual void FilesNeeded( T_StringList& vFiles ) const;
    virtual void Reset();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ADN_ActiveProtections_Data( const ADN_ActiveProtections_Data& );            //!< Copy constructor
    ADN_ActiveProtections_Data& operator=( const ADN_ActiveProtections_Data& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void ReadArchive( xml::xistream& xis );
    virtual void WriteArchive( xml::xostream& xos );
    virtual void ReadProtection( xml::xistream& xis );
    //@}

private:
    //! @name Types
    //@{
    class Protection
    {
    public:
        Protection( xml::xistream& xis );
        void WriteArchive( xml::xostream& xos ) const;
    private:
        void ReadWeapon( xml::xistream& xis );
        std::string name_;
        double coefficient_;
        bool hardKill_;
        std::string strDotationName_;
        double usage_;
        std::vector< std::pair<std::string, double> > weapons_;
    };
    //@}

    //! @name Member data
    //@{
    std::vector< Protection > protections_;
    //@}
};

#endif // __ADN_ActiveProtections_Data_h_
