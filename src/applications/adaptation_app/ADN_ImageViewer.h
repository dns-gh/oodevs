// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2003-11-18 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_ImageViewer.h $
// $Author: Ape $
// $Modtime: 11/01/05 10:11 $
// $Revision: 2 $
// $Workfile: ADN_ImageViewer.h $
//
// *****************************************************************************

#ifndef __ADN_ImageViewer_h_
#define __ADN_ImageViewer_h_

// =============================================================================
/** @class  ADN_ImageViewer
    @brief  ADN_ImageViewer
    @par    Using example
    @code
    ADN_ImageViewer;
    @endcode
*/
// Created: AGN 2003-11-18
// =============================================================================
class ADN_ImageViewer
: public QWidget
{
public:
    //! @name Constructors/Destructor
    //@{
             ADN_ImageViewer( const char* fileName, bool bFromRessouce, QWidget* pParent = 0, const char* name = 0 );
    virtual ~ADN_ImageViewer();
    //@}

protected:
    void    paintEvent( QPaintEvent * );
    void    resizeEvent( QResizeEvent * );

private:
    //! @name Copy/Assignment
    //@{
    ADN_ImageViewer( const ADN_ImageViewer& );            //!< Copy constructor
    ADN_ImageViewer& operator=( const ADN_ImageViewer& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    bool LoadImage( const char *fileName, bool bFromRessource );
    bool ReconvertImage();

    void Scale();
    //@}

private:
    //! @name Member data
    //@{
    QImage image_;          // the loaded image
    QPixmap    pm_;            // the converted pixmap
    QPixmap    pmScaled_;            // the converted pixmap
    int alloc_context_;
//@}
};


#endif // __ADN_ImageViewer_h_
