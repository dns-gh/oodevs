// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __XmliaReport_h_
#define __XmliaReport_h_

#include <qdom.h>

// =============================================================================
/** @class  XmliaReport
    @brief  XmliaReport
*/
// Created: SBO 2008-06-11
// =============================================================================
class XmliaReport : public QListViewItem
{

public:
    //! @name Constructors/Destructor
    //@{
    XmliaReport( QListView* parent, const QString& report );
    virtual ~XmliaReport();
    //@}

public:
    //! @name Operations
    //@{
    void show();
    //@}

private slots:
    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignment
    //@{
    XmliaReport( const XmliaReport& );            //!< Copy constructor
    XmliaReport& operator=( const XmliaReport& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    QDomDocument report_;
    //@}
};

#endif // __XmliaReport_h_
