// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogConsignDisplayer_ABC_h_
#define __LogConsignDisplayer_ABC_h_

// =============================================================================
/** @class  LogConsignDisplayer_ABC
    @brief  LogConsignDisplayer_ABC
*/
// Created: MMC 2012-10-29
// =============================================================================
class LogConsignDisplayer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             LogConsignDisplayer_ABC() {};
    virtual ~LogConsignDisplayer_ABC() {};
    //@}

    //! @name Types
    //@{
    typedef std::vector< std::pair< QString, unsigned int > > T_OrderedValues;
    //@}

    //! @name Operations
    //@{
    virtual void DisplayTitle( const QString& key, const QString& value ) = 0;
    virtual void DisplayItem( const QString& key, const QString& value ) = 0;
    virtual void DisplaySubItemValues( const QString& key, const QString& subKey, 
                                       const QMap< QString, T_OrderedValues >& subValues ) = 0;
    //@}
};

#endif // __LogConsignDisplayer_ABC_h_
