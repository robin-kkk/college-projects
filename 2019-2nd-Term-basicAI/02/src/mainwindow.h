#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts>
#include <cstdlib>
#include <ctime>

#include "linearclassifier.h"

using namespace QtCharts;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void start();
    void random_input();
    void init_input();
    void draw_line(double net, double x0, double w0, double x1, double w1);
    void draw_error(vector<double> errors, vector<vector<double> > weights);

private:
    const int WINDOW_WIDTH = 600;
    const int WINDOW_HEIGHT = 800;
    const int BUTTON_WIDTH = 140;
    const int BUTTON_HEIGHT = 60;
    const int GROUP_WIDTH = 100;
    const char *TITLE = "Linear Classifier - Eunjin, Kwon";
    const char *FONT_STYLE_14PT = "font-size: 14pt";

    bool is_running = false;
    vector<vector<double> > input;
    vector<int> output;
    vector<double> initial_weight;

    QPushButton *start_btn;
    QPushButton *random_input_btn;
    QPushButton *init_input_btn;
    QLineSeries *series;
    QBarSet *bar_set;
    QChart *learning_chart;
    QChart *error_chart;
    QTableWidget *weight_chart;
    QRadioButton *and_btn;
    QRadioButton *or_btn;
    QRadioButton *xor_btn;
    QSpinBox *dimension_box;
    QDoubleSpinBox *rate_box;

    LinearClassifier *classifier;

    double get_random_value(int min, int max);
    QTableWidget* get_table_widget(int row, int column);
    void clear_ui();
};
#endif // MAINWINDOW_H
