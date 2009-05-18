#include <QtCore>

#define NUMBER_OF_ROUNDS 2
#define WORDS_PER_ROUND 3

class Item;

class Controller : public QObject {
  Q_OBJECT

  public:
    Controller();
    ~Controller();
    bool init();
    void start();

  private:
    bool readFile(
      QString fileName, QString& categoryName, QStringList& categoryItems
    );
    QString getRandomWord();
    void prepareNewWord();
    void prepareNewRound();
    void writeRoundResults();

    QString leftCategory1Name;
    QString leftCategory2Name;
    QString rightCategory1Name;
    QString rightCategory2Name;
    QStringList leftCategory1Words;
    QStringList leftCategory2Words;
    QStringList rightCategory1Words;
    QStringList rightCategory2Words;

    QString l1n, l2n, r1n, r2n;
    QStringList *l1w, *l2w, *r1w, *r2w;

    Item* currentItem;
    int round;
    int wordsLeft;
    bool showingLeftWord;
    int correctAnswers;
    int wrongAnswers;
    QVector<int> results;
    QFile logFile;
    QTextStream* logStream;

  public slots:
    void getAnswer( bool left, int msecs );
};

