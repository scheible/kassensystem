#include "keyeventdetector.h"
#include <QDebug>
#include <QEvent>
#include <QKeyEvent>

keyEventDetector::keyEventDetector(MainWindow *w, QObject *parent) : QObject(parent)
{
    mWindow = w;
}

bool keyEventDetector::eventFilter(QObject *Object, QEvent *Event)
{
  if (Event->type() == QEvent::KeyPress)
  {
    QKeyEvent *KeyEvent = (QKeyEvent*)Event;
    mWindow->keyPressEvent(KeyEvent);
  }

  if (Event->type() == QEvent::KeyRelease) {
      QKeyEvent *KeyEvent = (QKeyEvent*)Event;
      if (!KeyEvent->isAutoRepeat()) {
        mWindow->keyReleaseEvent(KeyEvent);
        return true;
      }
  }
  return false;
}
