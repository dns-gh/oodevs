// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ADN_MultiPercentage_h_
#define __ADN_MultiPercentage_h_

#include <boost/noncopyable.hpp>
#include "ADN_EditLine.h"

class ADN_GuiBuilder;
class ADN_Connector_ABC;

// =============================================================================
/** @class  ADN_MultiPercentage
    @brief  ADN_MultiPercentage
*/
// Created: ABR 2011-03-18
// =============================================================================
class ADN_MultiPercentage : public QObject
                          , private boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ADN_MultiPercentage( Q3GroupBox* pParent, ADN_GuiBuilder& builder );
    virtual ~ADN_MultiPercentage();
    //@}

    //! @name Operations
    //@{
    void AddLine( const char* szName, ADN_Connector_ABC*& itemConnector );
    void AddWarning();
    //@}

private slots:
    //! @name slots
    //@{
    void PercentageChanged();
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< ADN_EditLine_Double* > T_EditLines;
    typedef T_EditLines::iterator              IT_EditLines;
    //@}

private:
    //! @name Member data
    //@{
    Q3GroupBox*                          parent_;
    ADN_GuiBuilder&                     builder_;
    std::vector< ADN_EditLine_Double* > lines_;
    QLabel*                             warning_;
    //@}
};

#endif // __ADN_MultiPercentage_h_
