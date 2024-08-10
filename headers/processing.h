#ifndef PROCESSING_H
#define PROCESSING_H

#include <QWidget>
#include <QFileDialog>


namespace Ui {
class Processing;
}

class Processing : public QWidget
{
    Q_OBJECT

public:
    explicit Processing(QWidget *parent = nullptr);
    ~Processing();
    void ChangeTitle(QString newtitle);
    bool Folder = false;
    bool Encode = false;
    void print(std::string str);
    void decode(std::filesystem::path input_path,std::filesystem::path output_path);
    void encode(std::filesystem::path input_path,std::filesystem::path output_path);
    void print(QString str);
private slots:
    void on_Select1_clicked();

    void on_Select2_clicked();

    void on_Run_clicked();

    void on_ExitBtn_clicked();

private:
    Ui::Processing *ui;
};
#endif // PROCESSING_H
