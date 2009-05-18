#include <QtCore>

#define WORDS_PER_ROUND 20
#define WORDS_TO_SKIP 2

class Item;
class Instructions;

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
    void writeOverallResults();

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
    Instructions* currentInstructions;
    bool versionA;
    int round;
    int wordsLeft;
    int wordsLeftToSkip;
    bool showingLeftWord;
    int correctAnswers;
    int wrongAnswers;
    int correctOverall;
    int wrongOverall;
    QVector<int> results;
    QVector<int> overallResults;
    QFile logFile;
    QTextStream* logStream;

  public slots:
    void getAnswer( bool left, int msecs );
    void instructionsRead();
};

