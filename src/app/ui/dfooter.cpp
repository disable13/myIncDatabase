#include "dfooter.h"
//
#include "../../3rdparty/QProgressIndicator.h"
//
#ifndef HAVE_QT5
# include <QtGui/QGridLayout>
# include <QtGui/QProgressBar>
# include <QtGui/QLabel>
#else
# include <QtWidgets/QGridLayout>
# include <QtWidgets/QProgressBar>
# include <QtWidgets/QLabel>
#endif
//
DFooter::DFooter(QWidget *parent) :
    QWidget(parent), max(-1)
{
    setObjectName( "Footer" );
    setMinimumHeight( 33 );

    l = new QGridLayout( this );
    setLayout( l );

    lText = new QLabel( this );
    progressBar = new QProgressBar( this );
    progressInicator = new QProgressIndicator(this);

    l->addWidget( lText );
    l->addWidget( progressBar, 0, 1);
    l->addWidget( progressInicator, 0, 2);

    progressBar->setVisible( false );
    progressBar->setMinimum( 0 );
    progressBar->setValue( 0 );
    progressBar->setMaximum( 1 );
    progressInicator->setAnimationDelay( 70 );
}
//
DFooter::~DFooter()
{
    FREE_MEM(progressInicator);
    FREE_MEM(progressBar);
    FREE_MEM(lText);

    FREE_MEM(l);
}
//
void DFooter::setText( const QString &text )
{
    lText->setText( text );
}
//
void DFooter::progressStart()
{
    progressInicator->startAnimation();
}
//
void DFooter::progressStart(const int &mx)
{
    max = mx;
    progressBar->setMaximum( max );
    progressBar->setVisible( true );
    if (!progressInicator->isAnimated())
        progressInicator->startAnimation();
}
//
void DFooter::progressStart(const QString &text )
{
    lText->setText( text );
    progressInicator->startAnimation();
}
//
void DFooter::progressStart(const QString &text, const int &mx)
{
    lText->setText( text );
    max = mx;
    progressBar->setMaximum( max );
    progressBar->setVisible( true );
    if (!progressInicator->isAnimated())
        progressInicator->startAnimation();
}
//
void DFooter::progressChange(const int &cr)
{
    if (max > cr ) { // then stop
        progressBar->setValue( 0 );
        progressBar->setMaximum( 1 );
        progressBar->setVisible( false );
        progressInicator->stopAnimation();
        return;
    }
    progressBar->setValue( cr );
}
//
void DFooter::progressStop()
{
    progressInicator->stopAnimation();
}
//
void DFooter::progressStop(const bool &state)
{
    if (state) {
        lText->setText( tr("Complete!") );
    } else{
        lText->setText( tr("Uncomplete...") );
    }
    progressInicator->stopAnimation();
}
