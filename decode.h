#ifndef DECODE_H
#define DECODE_H

#include <QWidget>

namespace Ui {
class Decode;
}

class Decode : public QWidget
{
    Q_OBJECT

public:
    explicit Decode(QWidget *parent = nullptr);
    ~Decode();

private slots:
    void on_Menu_clicked();
    void on_select1_clicked();

    void on_select2_clicked();

    void on_ActionButton_clicked();
    std::string time(std::chrono::time_point<std::chrono::high_resolution_clock> start,
                     std::chrono::time_point<std::chrono::high_resolution_clock> end);
    void print(QString string);
    bool hasEnding(std::string const& fullString, std::string const& ending);
    void decode (QString filepathstring, QString folderpathstring);
    void decodefolder(QString infolderpathstring, QString outfolderpathstring);
    void encode(QString folderpathstring, QString filepathstring);
    void encodefolder(QString infolderpathstring, QString outfolderpathstring);
public slots:
    void changetitel(QString titel);
public:
    Ui::Decode *ui;
};

#endif // DECODE_H
