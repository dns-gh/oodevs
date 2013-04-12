// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ADN_MainTabWidget_h_
#define __ADN_MainTabWidget_h_

#include <boost/noncopyable.hpp>
#include "ADN_Workspace.h"
#include "ADN_Enums.h"

// =============================================================================
/** @class  ADN_MainTabWidget
    @brief  ADN_MainTabWidget
*/
// Created: ABR 2012-01-18
// =============================================================================
class ADN_MainTabWidget : public QTabWidget
                        , private boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_MainTabWidget( QWidget* parent );
    virtual ~ADN_MainTabWidget();
    //@}

    //! @name Operations
    //@{
    void AddPage( E_WorkspaceElements target, QWidget& page, const QString& title );
    //@}

signals:
    //! @name Signals
    //@{
    void BackEnabled( bool );
    void ForwardEnabled( bool );
    //@}

public slots:
    //! @name Slots
    //@{
    void OnBack();
    void OnForward();
    void OnCurrentChanged( int index );
    void OnChangeTab( E_WorkspaceElements target );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< E_WorkspaceElements, int > T_ElementIndexMap;
    //@}

private:
    //! @name Member data
    //@{
    T_ElementIndexMap  elementIndexMap_;
    std::vector< int > history_;
    int                currentHistoryIndex_; // where we are in the history vector
    bool               navigating_;
    //@}
};

#endif // __ADN_MainTabWidget_h_
