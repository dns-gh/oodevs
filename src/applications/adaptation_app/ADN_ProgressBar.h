//*****************************************************************************
//
// $Created: JDY 03-07-17 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_ProgressBar.h $
// $Author: Ape $
// $Modtime: 11/04/05 17:57 $
// $Revision: 5 $
// $Workfile: ADN_ProgressBar.h $
//
//*****************************************************************************

#ifndef __ADN_ProgressBar_h_
#define __ADN_ProgressBar_h_

#include "ADN_ProgressIndicator_ABC.h"

// =============================================================================
/** @class  ADN_ProgressBar
    @brief  ADN_ProgressBar
*/
// Created: APE 2005-03-18
// =============================================================================
class ADN_ProgressBar : public Q3HBox
                      , public ADN_ProgressIndicator_ABC
{
public:
    explicit ADN_ProgressBar( QWidget* pParent = 0, const char* szName = 0, Qt::WFlags f = 0 );
    virtual ~ADN_ProgressBar();

    //! @name Modifiers
    //@{
    void SetNbrOfSteps( int n );
    //@}

    //! @name Operations
    //@{
    void Increment( int n = 1 );
    void Increment( const char* szText, int n = 1 );
    void Reset( const char* szMsg = 0 );
    //@}

private:
    QLabel* label_;
    Q3ProgressBar* bar_;
};


#endif // __ADN_ProgressBar_h_