// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Sensors_Postures_GUI_h_
#define __ADN_Sensors_Postures_GUI_h_

#include "ADN_Connector_Table_ABC.h"
#include "ADN_Sensors_ModificatorTable.h"

//-----------------------------------------------------------------------------
// Internal Table connector
//-----------------------------------------------------------------------------
class ADN_CT_Sensors_Postures : public ADN_Connector_Table_ABC
{
public:
    //! @name Constructor / Destructor
    //@{
    ADN_CT_Sensors_Postures( ADN_Table& table );
    //@}

    //! @name Operations
    //@{
    void AddSubItems( int i, void *pObj );
    //@}
};

// =============================================================================
/** @class  ADN_Sensors_Postures_GUI
    @brief  ADN_Sensors_Postures_GUI
*/
// Created: ABR 2012-01-16
// =============================================================================
class ADN_Sensors_Postures_GUI : public ADN_Sensors_ModificatorTable< ADN_CT_Sensors_Postures >
{
public:
    //! @name Constructor / Destructor
    //@{
    explicit ADN_Sensors_Postures_GUI( const QString& strColCaption, QWidget * parent = 0 );
    virtual ~ADN_Sensors_Postures_GUI();
    //@}

private:
    //! @name ADN_Sensors_MaterialsTable implementation
    //@{
    virtual void InternalEmit();
    //@}
};


//#include "ADN_CommonGfx.h"
//#include "ENT/ENT_Enums_Gen.h"
//#include <boost/noncopyable.hpp>
//
////*****************************************************************************
//// Created: JDY 03-09-29
////*****************************************************************************
//class ADN_Sensors_Postures_GUI : public ADN_Table2
//                               , private boost::noncopyable
//{
//    Q_OBJECT;
//
//public:
//    //! @name Constructors/Destructor
//    //@{
//    explicit ADN_Sensors_Postures_GUI( const QString& strColCaption, QWidget* pParent = 0 );
//    virtual ~ADN_Sensors_Postures_GUI();
//    //@}
//
//signals:
//    //! @name Signals
//    //@{
//    void PostureChanged( std::string posture, double coeff );
//    //@}
//
//private slots:
//    //! @name Slots
//    //@{
//    void OnCurrentChanged();
//    //@}
//};


#endif // __ADN_Sensors_Postures_GUI_h_