// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2005-03-07 $
// $Archive: /MVW_v10/Build/SDK/HLA/src/HLA_MainWindow.h $
// $Author: Age $
// $Modtime: 7/03/05 16:18 $
// $Revision: 1 $
// $Workfile: HLA_MainWindow.h $
//
// *****************************************************************************

#ifndef __HLA_MainWindow_h_
#define __HLA_MainWindow_h_

#include <qmainwindow.h>
class QPushButton;
class HLA_CreateFederationBox;

class TimeFactory_ABC;
class TimeIntervalFactory_ABC;
class RtiAmbassador_ABC;
class Federate;

// =============================================================================
/** @class  HLA_MainWindow
    @brief  HLA_MainWindow
*/
// Created: AGE 2005-03-07
// =============================================================================
class HLA_MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             HLA_MainWindow();
    virtual ~HLA_MainWindow();
    //@}

    //! @name Operations
    //@{
    //@}

    //! @name Accessors
    //@{
    //@}

    //! @name Modifiers
    //@{
    //@}

private slots:
    //! @name Slots
    //@{
    void OnCreate();
    void OnDestroy();
    void OnStart();
    void ChangeFederationSettings( const std::string& strName, const std::string& strFOM );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    HLA_MainWindow( const HLA_MainWindow& );            //!< Copy constructor
    HLA_MainWindow& operator=( const HLA_MainWindow& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void LoadSettings( const std::string& strConfigFile );
    void LoadHlaSettings( const std::string& strHlaFile );
    //@}

private:
    //! @name Member data
    //@{
    QPushButton*             pCreateDestroyButton_;
    QPushButton*             pStartButton_;
    HLA_CreateFederationBox* pCreateBox_;
    
    unsigned int             nTimeStep_;
    bool                     bJoined_;
    TimeFactory_ABC*         pFactory_;
    TimeIntervalFactory_ABC* pIntervalFactory_;
    RtiAmbassador_ABC*       pAmbassador_;
    Federate*                pFederate_;

    std::string strFederateName_;
    std::string strFederationName_;
    std::string strConfigFile_;
    std::string strFOMFile_;
    //@}
};

#endif // __HLA_MainWindow_h_
