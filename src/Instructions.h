#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "uic/ui_InstructionsDialog.h"

#include <QtCore>
#include <QtGui>

class Instructions
  : public QWidget,
    public Ui_InstructionsDialog
{
  Q_OBJECT

  public:
    Instructions();
    void setText( QString text );

    virtual void keyPressEvent( QKeyEvent* event );

  signals:
    void read();
};

#endif

