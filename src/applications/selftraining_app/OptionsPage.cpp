// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "OptionsPage.h"
#include "moc_OptionsPage.cpp"
#include "Application.h"
#include "clients_gui/Tools.h"
#include "tools/GeneralConfig.h"
#include <qcombobox.h>
#include <qfiledialog.h>
#include <qsettings.h>
#include <qtextcodec.h>
#include <qcheckbox.h>
#include <qspinbox.h>
#include <boost/foreach.hpp>

namespace
{
    QString ReadLang()
    {
        QSettings settings;
        settings.setPath( "MASA Group", qApp->translate( "Application", "SWORD" ) );
        return settings.readEntry( "/Common/Language", QTextCodec::locale() );
    }

    void ReadGaussian( bool* distributions )
    {
        QSettings settings;
        settings.setPath( "MASA Group", qApp->translate( "Application", "SWORD" ) );
        for( int i = 0; i < OptionsPage::eContextsNbr; ++i )
            distributions[ i ] = settings.readBoolEntry( QString( "/Common/RandomDistribution" ) + QString::number( i ), false );
    }

    void ReadDeviation( int* deviations )
    {
        QSettings settings;
        settings.setPath( "MASA Group", qApp->translate( "Application", "SWORD" ) );
        for( int i = 0; i < OptionsPage::eContextsNbr; ++i )
            deviations[ i ] = settings.readNumEntry( QString( "/Common/RandomDeviation" ) + QString::number( i ), 5 );
    }

    void ReadMean( int* means )
    {
        QSettings settings;
        settings.setPath( "MASA Group", qApp->translate( "Application", "SWORD" ) );
        for( int i = 0; i < OptionsPage::eContextsNbr; ++i )
            means[ i ] = settings.readNumEntry( QString( "/Common/RandomMean" ) + QString::number( i ), 5 );
    }

    bool ReadHasSeed()
    {
        QSettings settings;
        settings.setPath( "MASA Group", qApp->translate( "Application", "SWORD" ) );
        return settings.readBoolEntry( "/Common/RandomHasSeed", false );
    }

    int ReadSeed()
    {
        QSettings settings;
        settings.setPath( "MASA Group", qApp->translate( "Application", "SWORD" ) );
        return settings.readNumEntry( "/Common/RandomSeed", 1 );
    }

    class DecimalSpinBox : public QSpinBox
    {
    public:
        DecimalSpinBox( int minValue, int maxValue, int step, QWidget* parent )
            : QSpinBox( minValue, maxValue, step, parent ) {}

    private:
        virtual QString	mapValueToText( int value )
        {
            if( value == 0 )
                return QString( "0" );
            if( value == 10 )
                return QString( "1" );
            return QString( "0.%1" ).arg( value );
        }

        virtual int mapTextToValue( bool* /*ok*/ )
        {
            return (int) ( 10 * text().toFloat() );
        }
    };
}

// -----------------------------------------------------------------------------
// Name: OptionsPage constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
OptionsPage::OptionsPage( QWidgetStack* pages, Page_ABC& previous, const tools::GeneralConfig& config )
    : ContentPage( pages, tools::translate( "OptionsPage", "Options" ), previous, eButtonBack | eButtonQuit )
    , selectedLanguage_( ReadLang() )
{
    languages_[ tools::translate( "OptionsPage", "English" ) ]  = "en";
    languages_[ tools::translate( "OptionsPage", "Français" ) ] = "fr";

    QVBox* mainBox = new QVBox( this );
    mainBox->setBackgroundOrigin( QWidget::WindowOrigin );
    mainBox->setMargin( 10 );
    QGroupBox* box = new QGroupBox( 2, Qt::Horizontal, mainBox );
    box->setFrameShape( NoFrame );
    box->setBackgroundOrigin( QWidget::WindowOrigin );
    {
        QLabel* label = new QLabel( tools::translate( "OptionsPage", "Language: " ), box );
        label->setBackgroundOrigin( QWidget::WindowOrigin );
        QComboBox* combo = new QComboBox( box );
        BOOST_FOREACH( const T_Languages::value_type& lang, languages_ )
        {
            combo->insertItem( lang.first );
            if( lang.second == selectedLanguage_ )
                combo->setCurrentText( lang.first );
        }
        connect( combo, SIGNAL( activated( const QString& ) ), SLOT( OnChangeLanguage( const QString& ) ) );
    }
    {
        QLabel* label = new QLabel( tools::translate( "OptionsPage", "Data directory: " ), box );
        label->setBackgroundOrigin( QWidget::WindowOrigin );
        QHBox* hbox = new QHBox( box );
        hbox->setBackgroundOrigin( QWidget::WindowOrigin );
        dataDirectory_ = new QLineEdit( hbox );
        dataDirectory_->setText( config.GetRootDir().c_str() );
        QButton* browse = new QPushButton( tools::translate( "OptionsPage", "..." ), hbox );
        connect( browse, SIGNAL( clicked() ), SLOT( OnChangeDataDirectory() ) );
    }
    {
        QGroupBox* hbox = new QGroupBox( 2, Qt::Horizontal, tools::translate( "OptionsPage", "Random Generator" ), mainBox );
        hbox->setBackgroundOrigin( QWidget::WindowOrigin );

        hasSeed_ = new QCheckBox( tools::translate( "OptionsPage", "Seed:" ), hbox );
        hasSeed_->setBackgroundOrigin( QWidget::WindowOrigin );
        connect( hasSeed_, SIGNAL( toggled( bool ) ), SLOT( OnSeedToggled() ) );
        bool bHasSeed = ReadHasSeed();
        seed_ = new QSpinBox( 1, std::numeric_limits< int >::max(), 1, hbox );
        seed_->setValue( ReadSeed() );
        seed_->setEnabled( bHasSeed );
        hasSeed_->setChecked( bHasSeed );

        QLabel* label1 = new QLabel( tools::translate( "OptionsPage", "Context:" ), hbox );
        label1->setBackgroundOrigin( QWidget::WindowOrigin );
        contextList_ = new QComboBox( hbox );
        contextList_->insertItem( tools::translate( "OptionsPage", "Fire" ), eFire );
        contextList_->insertItem( tools::translate( "OptionsPage", "Wounds" ), eWounds );
        contextList_->insertItem( tools::translate( "OptionsPage", "Perception" ), ePerception );
        contextList_->insertItem( tools::translate( "OptionsPage", "Breakdowns" ), eBreakdowns );
        contextList_->setCurrentItem( 0 );
        connect( contextList_, SIGNAL( activated( int ) ), SLOT( OnContextChanged( int ) ) );

        QLabel* label2 = new QLabel( tools::translate( "OptionsPage", "Distribution:" ), hbox );
        label2->setBackgroundOrigin( QWidget::WindowOrigin );
        distributionList_ = new QComboBox( hbox );
        distributionList_->insertItem( tools::translate( "OptionsPage", "Linear" ) );
        distributionList_->insertItem( tools::translate( "OptionsPage", "Gaussian" ) );
        connect( distributionList_, SIGNAL( activated( int ) ), SLOT( OnDistributionChanged( int ) ) );

        QLabel* label3 = new QLabel( tools::translate( "OptionsPage", "Standard deviation:" ), hbox );
        label3->setBackgroundOrigin( QWidget::WindowOrigin );
        deviation_ = new DecimalSpinBox( 0, 10, 1, hbox );
        connect( deviation_, SIGNAL( valueChanged( int ) ), SLOT( OnDeviationChanged( int ) ) );

        QLabel* label4 = new QLabel( tools::translate( "OptionsPage", "Mean:" ), hbox );
        label4->setBackgroundOrigin( QWidget::WindowOrigin );
        mean_ = new DecimalSpinBox( 0, 10, 1, hbox );
        connect( mean_, SIGNAL( valueChanged( int ) ), SLOT( OnMeanChanged( int ) ) );
    }
    ReadRandomValues();
    OnContextChanged( 0 );

    AddContent( mainBox );
}

// -----------------------------------------------------------------------------
// Name: OptionsPage destructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
OptionsPage::~OptionsPage()
{
    Commit();
}

// -----------------------------------------------------------------------------
// Name: OptionsPage::OnChangeLanguage
// Created: SBO 2009-04-08
// -----------------------------------------------------------------------------
void OptionsPage::OnChangeLanguage( const QString& lang )
{
    selectedLanguage_ = languages_[ lang ];
    Commit();
    static_cast< Application* >( qApp )->CreateTranslators();
}

// -----------------------------------------------------------------------------
// Name: OptionsPage::Commit
// Created: SBO 2009-04-08
// -----------------------------------------------------------------------------
void OptionsPage::Commit()
{
    QSettings settings;
    settings.setPath( "MASA Group", qApp->translate( "Application", "SWORD" ) );
    settings.writeEntry( "/Common/Language", selectedLanguage_.c_str() );
    settings.writeEntry( "/Common/DataDirectory", dataDirectory_->text() );
    for( int i = 0; i < eContextsNbr; ++i )
    {
        settings.writeEntry( QString( "/Common/RandomDistribution" ) + QString::number( i ), bDistributions_[ i ] );
        settings.writeEntry( QString( "/Common/RandomDeviation" ) + QString::number( i ), nDeviations_[ i ] );
        settings.writeEntry( QString( "/Common/RandomMean" ) + QString::number( i ), nMeans_[ i ] );
    }
    settings.writeEntry( "/Common/RandomHasSeed", hasSeed_->isChecked() );
    settings.writeEntry( "/Common/RandomSeed", seed_->value() );
}

// -----------------------------------------------------------------------------
// Name: OptionsPage::ReadRandomValues
// Created: JSR 2010-07-05
// -----------------------------------------------------------------------------
void OptionsPage::ReadRandomValues()
{
    ReadGaussian( bDistributions_ );
    ReadDeviation( nDeviations_ );
    ReadMean( nMeans_ );
}

// -----------------------------------------------------------------------------
// Name: OptionsPage::OnChangeDataDirectory
// Created: SBO 2010-03-26
// -----------------------------------------------------------------------------
void OptionsPage::OnChangeDataDirectory()
{
    const QString directory = QFileDialog::getExistingDirectory( dataDirectory_->text(), this );
    if( directory.isEmpty() )
        return;
    dataDirectory_->setText( directory );
    Commit();
}

// -----------------------------------------------------------------------------
// Name: OptionsPage::OnContextChanged
// Created: JSR 2010-07-05
// -----------------------------------------------------------------------------
void OptionsPage::OnContextChanged( int index )
{
    ERandomContexts context = ( ERandomContexts ) index;
    if( context >= eFire && context < eContextsNbr )
    {
        distributionList_->setCurrentItem( bDistributions_[ context ] ? 1 : 0 );
        deviation_->setValue( nDeviations_[ context ] );
        mean_->setValue( nMeans_[ context ] );
        deviation_->setEnabled( bDistributions_[ context ] );
        mean_->setEnabled( bDistributions_[ context ] );
    }
}

// -----------------------------------------------------------------------------
// Name: OptionsPage::OnDistributionChanged
// Created: JSR 2010-07-02
// -----------------------------------------------------------------------------
void OptionsPage::OnDistributionChanged( int index )
{
    deviation_->setEnabled( index != 0 );
    mean_->setEnabled( index != 0 );
    ERandomContexts context = ( ERandomContexts ) contextList_->currentItem();
    if( context >= eFire && context < eContextsNbr )
    {
        bDistributions_[ context ] = ( index != 0 );
    }
}

// -----------------------------------------------------------------------------
// Name: OptionsPage::OnDeviationChanged
// Created: JSR 2010-07-05
// -----------------------------------------------------------------------------
void OptionsPage::OnDeviationChanged( int )
{
    ERandomContexts context = ( ERandomContexts ) contextList_->currentItem();
    if( context >= eFire && context < eContextsNbr )
        nDeviations_[ context ] = deviation_->value();
}

// -----------------------------------------------------------------------------
// Name: OptionsPage::OnMeanChanged
// Created: JSR 2010-07-05
// -----------------------------------------------------------------------------
void OptionsPage::OnMeanChanged( int )
{
    ERandomContexts context = ( ERandomContexts ) contextList_->currentItem();
    if( context >= eFire && context < eContextsNbr )
        nMeans_[ context ] = mean_->value();
}

// -----------------------------------------------------------------------------
// Name: OptionsPage::OnSeedToggled
// Created: JSR 2010-07-01
// -----------------------------------------------------------------------------
void OptionsPage::OnSeedToggled()
{
    seed_->setEnabled( hasSeed_->isChecked() );
}
