// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __InitialStateResource_h_
#define __InitialStateResource_h_

namespace xml
{
    class xistream;
    class xostream;
}

// =============================================================================
/** @class  InitialStateResource
    @brief  InitialStateResource
*/
// Created: ABR 2011-03-07
// =============================================================================
class InitialStateResource
{
public:
    //! @name Constructors/Destructor
    //@{
             InitialStateResource( const QString& name, const QString& category, unsigned int number, unsigned int maximum, double threshold, double consumption );
    explicit InitialStateResource( xml::xistream& xis );
    virtual ~InitialStateResource();
    //@}

    //! @name Operations
    //@{
    void Serialize( xml::xostream& xos, double defaultLowThreshold ) const;
    bool operator==( const InitialStateResource& object ) const;
    bool operator!=( const InitialStateResource& object ) const;
    //@}

public:
    //! @name Member data
    //@{
    QString      name_;
    QString      category_;
    unsigned int number_;
    unsigned int maximum_;
    double       threshold_;
    double       consumption_;
    //@}
};

#endif // __InitialStateResource_h_
