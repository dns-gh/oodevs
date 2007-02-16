// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __InfoStancesWidget_h_
#define __InfoStancesWidget_h_

class Attributes;

// =============================================================================
/** @class  InfoStancesWidget
    @brief  InfoStancesWidget
*/
// Created: SBO 2007-02-09
// =============================================================================
class InfoStancesWidget : public QHBox
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit InfoStancesWidget( QWidget* parent );
    virtual ~InfoStancesWidget();
    //@}

    //! @name Operations
    //@{
    void Update( const Attributes& attributes );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    InfoStancesWidget( const InfoStancesWidget& );            //!< Copy constructor
    InfoStancesWidget& operator=( const InfoStancesWidget& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void InitializeIcons();
    //@}

    //! @name Types
    //@{
    typedef std::vector< QPixmap > T_Pixmaps;
    //@}

private:
    //! @name Member data
    //@{
    QButton* previous_;
    QButton* next_;
    QLabel*  progress_;
    T_Pixmaps pixmaps_;
    //@}
};

#endif // __InfoStancesWidget_h_
