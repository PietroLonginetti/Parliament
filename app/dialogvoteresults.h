#ifndef DIALOGVOTERESULTS_H
#define DIALOGVOTERESULTS_H

#include <QDialog>

namespace Ui {
class DialogVoteResults;
}

class DialogVoteResults : public QDialog {
    Q_OBJECT

private:
    Ui::DialogVoteResults *ui;

public:
    explicit DialogVoteResults(QWidget *parent = nullptr);
    void setLabel(QString text);
    ~DialogVoteResults();


};

#endif // DIALOGVOTERESULTS_H
