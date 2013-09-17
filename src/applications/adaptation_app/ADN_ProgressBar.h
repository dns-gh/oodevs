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

// =============================================================================
/** @class  ADN_ProgressBar
    @brief  ADN_ProgressBar
*/
// Created: APE 2005-03-18
// =============================================================================
class ADN_ProgressBar : public QWidget
{
public:
    explicit ADN_ProgressBar( QWidget* parent );
    virtual ~ADN_ProgressBar();

    //! @name Modifiers
    //@{
    void Reset();
    void SetMaximum( int n );
    void Increment( const QString& text );
    //@}

private:
    QLabel* label_;
    QProgressBar* bar_;
};

#endif // __ADN_ProgressBar_h_