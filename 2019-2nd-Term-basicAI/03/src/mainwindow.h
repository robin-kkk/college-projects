#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "classifier.h"
#include "layersettingswindow.h"
#include "learninggraphs.h"

using namespace QtCharts;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void clicked_start_btn();
    void clicked_weight_btn();
    void clicked_save_btn();
    void clicked_set_node_btn();
    void clicked_see_more_btn();
    void clicked_clear_btn();
    void applied_node_info(vector<int> node_cnt);
    void iterated_once(vector<vector<vector<double>>>* weights,
                       vector<vector<double>>* nodes,
                       double bias, int iter, int input_order,
                       QString* h_text);
    void iterated_all(vector<double> errors, bool not_broken);

private:
    const int windowWidth = 760;
    const int windowHeight = 800;
    const int buttonWidth = 150;
    const int buttonHeight = 50;
    const int groupWidth = 100;
    const char *title = "Multilayer Perceptron - Eunjin, Kwon";
    const char *fontStyle14 = "font-size: 14pt";

    vector<int> _node_count = {2, 3, 1};

    QPushButton *_start_btn;
    QPushButton *_show_weight_btn;
    QPushButton *_set_layer_btn;
    QPushButton *_see_more_btn;
    QPushButton *_clear_btn;
    QChart *_error_chart;
    QRadioButton *_and_btn;
    QRadioButton *_or_btn;
    QRadioButton *_xor_btn;
    QRadioButton *_donut_btn;
    QDoubleSpinBox *_rate_box;
    QSpinBox *_iter_cnt_box;
    QLabel *_hypothesis_label;

    Classifier *_classifier;
    LearningGraph *_learning_graph;
    LayerSettingsWindow *_layersettingswindow;
    vector<vector<vector<vector<double>>>> _w4d;
    vector<vector<vector<vector<double>>>> _i4d;
};
#endif // MAINWINDOW_H
