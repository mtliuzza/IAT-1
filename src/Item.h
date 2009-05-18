#ifndef ITEM_H
#define ITEM_H

#include "uic/ui_ItemDialog.h"

#include <QtCore>
#include <QtGui>

class Item
  : public QWidget,
    public Ui_ItemDialog
{
  Q_OBJECT

  public:
    Item();
    virtual ~Item();

    void init(
      QString leftCategory1,
      QString leftCategory2,
      QString rightCategory1,
      QString rightCategory2,
      QString word
    );
    virtual void keyPressEvent( QKeyEvent* event );

  signals:
    void answered( bool left, int msecs );

  private:
    int getTime();

    QTime timer;
};

#endif

