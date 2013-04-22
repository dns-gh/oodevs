// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __InitialStateEquipment_h_
#define __InitialStateEquipment_h_

#include <boost/optional.hpp>
#include <vector>

namespace xml
{
    class xistream;
    class xostream;
}

enum E_EquipmentState;

// =============================================================================
/** @class  InitialStateEquipment
    @brief  InitialStateEquipment
*/
// Created: ABR 2011-03-07
// =============================================================================
class InitialStateEquipment
{
public:
    //! @name Constructors/Destructor
    //@{
             InitialStateEquipment( const QString& name, E_EquipmentState state, const QStringList& breakdowns, const boost::optional< QString >& group = boost::none );
    explicit InitialStateEquipment( xml::xistream& xis );
    virtual ~InitialStateEquipment();
    //@}

    //! @name Operations
    //@{
    void Serialize( xml::xostream& xos ) const;
    bool operator==( const InitialStateEquipment& object ) const;
    bool operator!=( const InitialStateEquipment& object ) const;
    //@}

public:
    //! @name Member data
    //@{
    QString          name_;
    E_EquipmentState state_;
    QStringList      breakdowns_;
    unsigned int     currentBreakdown_;
    boost::optional< QString > group_;
    //@}
};

#endif // __InitialStateEquipment_h_
