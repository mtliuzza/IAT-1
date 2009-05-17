#ifndef ITEM_H
#define ITEM_H

#include "uic/ui_ItemDialog.h"

#include <QWidget>

class Item
  : public QWidget,
    public Ui_ItemDialog
{
  public:
    Item();
};

#endif

