// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ADN_MissionParameter_GroupBox_h_
#define __ADN_MissionParameter_GroupBox_h_

#include <boost/noncopyable.hpp>
#include "ADN_Enums.h"

// =============================================================================
/** @class  ADN_MissionParameter_GroupBox
    @brief  ADN_MissionParameter_GroupBox
*/
// Created: LGY 2012-04-18
// =============================================================================
class ADN_MissionParameter_GroupBox : public Q3GroupBox
                                    , private boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ADN_MissionParameter_GroupBox( int strips, Qt::Orientation orientation, const QString& title,
                                            E_MissionParameterType authorized );
             ADN_MissionParameter_GroupBox( int strips, Qt::Orientation orientation, const QString& title,
                                            const std::vector< E_MissionParameterType >& authorized );
    virtual ~ADN_MissionParameter_GroupBox();
    //@}

public slots:
    //! @name Slots
    //@{
    void OnTypeChanged( E_MissionParameterType type );
    //@}

private:
    //! @name Member Data
    //@{
    const std::vector< E_MissionParameterType > authorized_;
    //@}
};

#endif // __ADN_MissionParameter_GroupBox_h_
