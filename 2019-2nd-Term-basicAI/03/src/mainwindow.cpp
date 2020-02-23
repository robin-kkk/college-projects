#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    _learning_graph = new LearningGraph();
    _learning_graph->set_play_enabled(false);

    /* Set UI on Error Graph. */
    _error_chart = new QChart();
    _error_chart->legend()->hide();
    _error_chart->setTitle("Error Graph");
    _error_chart->setAnimationOptions(QChart::SeriesAnimations);

    QChartView *error_view = new QChartView(_error_chart);
    error_view->setRenderHint(QPainter::Antialiasing);
    error_view->setRubberBand(QChartView::HorizontalRubberBand);


    /* Set UI of the result label of learning. */
    _hypothesis_label = new QLabel("Hypothesis: [\n]");
    _hypothesis_label->setFixedWidth(groupWidth);
    _hypothesis_label->setStyleSheet("color: tomato;");
    QGroupBox *label_box = new QGroupBox();
    QVBoxLayout *label_box_layout = new QVBoxLayout();
    label_box_layout->addWidget(_hypothesis_label);
    label_box->setLayout(label_box_layout);


    /* Set UI on required button. */
    _start_btn = new QPushButton(tr("Start"));
    _start_btn->setStyleSheet(fontStyle14);
    _start_btn->setFixedSize(buttonWidth, buttonHeight);
    _start_btn->setCursor(QCursor(Qt::PointingHandCursor));
    connect(_start_btn, SIGNAL(clicked()), this, SLOT(clicked_start_btn()));

    _show_weight_btn = new QPushButton(tr("Weight"));
    _show_weight_btn->setStyleSheet(fontStyle14);
    _show_weight_btn->setFixedSize(buttonWidth, buttonHeight);
    _show_weight_btn->setCursor(QCursor(Qt::PointingHandCursor));
    connect(_show_weight_btn, SIGNAL(clicked()), this, SLOT(clicked_weight_btn()));

    _set_layer_btn = new QPushButton(tr("Set Node Count"));
    _set_layer_btn->setStyleSheet(fontStyle14);
    _set_layer_btn->setFixedSize(buttonWidth, buttonHeight);
    _set_layer_btn->setCursor(QCursor(Qt::PointingHandCursor));
    connect(_set_layer_btn, SIGNAL(clicked()), this, SLOT(clicked_set_node_btn()));

    _see_more_btn = new QPushButton(tr("See More Graphs"));
    _see_more_btn->setStyleSheet(fontStyle14);
    _see_more_btn->setFixedSize(buttonWidth, buttonHeight);
    _see_more_btn->setCursor(QCursor(Qt::PointingHandCursor));
    connect(_see_more_btn, SIGNAL(clicked()), this, SLOT(clicked_see_more_btn()));

    _clear_btn = new QPushButton(tr("Clear"));
    _clear_btn->setStyleSheet(fontStyle14);
    _clear_btn->setFixedSize(buttonWidth, buttonHeight);
    _clear_btn->setCursor(QCursor(Qt::PointingHandCursor));
    connect(_clear_btn, SIGNAL(clicked()), this, SLOT(clicked_clear_btn()));


    /* Set UI on radio button. */
    _and_btn = new QRadioButton(tr("AND"));
    _and_btn->setChecked(true);
    _or_btn = new QRadioButton(tr("OR"));
    _xor_btn = new QRadioButton(tr("XOR"));
    _donut_btn = new QRadioButton(tr("Donut"));
    QVBoxLayout *group_layout = new QVBoxLayout();
    group_layout->addWidget(_and_btn);
    group_layout->addWidget(_or_btn);
    group_layout->addWidget(_xor_btn);
    group_layout->addWidget(_donut_btn);
    QGroupBox *gate_group = new QGroupBox(tr("Sample"));
    gate_group->setAlignment(Qt::AlignCenter);
    gate_group->setStyleSheet(fontStyle14);
    gate_group->setMinimumWidth(groupWidth);
    gate_group->setMaximumWidth(groupWidth);
    gate_group->setLayout(group_layout);


    /* Set UI on SpinBox about Learning Rate. */
    _rate_box = new QDoubleSpinBox();
    _rate_box->setStyleSheet(fontStyle14);
    _rate_box->setRange(0., 1.);
    _rate_box->setValue(0.1);
    _rate_box->setSingleStep(0.01);
    _rate_box->setCursor(QCursor(Qt::PointingHandCursor));
    QLabel *rate_label = new QLabel(tr("Learning Rate: "));
    rate_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    rate_label->setFixedWidth(groupWidth);


    /* Set UI on SpinBox about Learning Rate. */
    _iter_cnt_box = new QSpinBox();
    _iter_cnt_box->setStyleSheet(fontStyle14);
    _iter_cnt_box->setRange(5000, 1000000);
    _iter_cnt_box->setValue(100000);
    _iter_cnt_box->setCursor(QCursor(Qt::PointingHandCursor));
    QLabel *iter_label = new QLabel(tr("Iteration Count: "));
    iter_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    iter_label->setFixedWidth(groupWidth);


    /* Set UI on Layout of MainWindow. */
    QGridLayout *main_layout = new QGridLayout();
    main_layout->addWidget(_learning_graph, 0, 0, 7, 1);
    main_layout->addWidget(label_box, 0, 1, 7, 1);
    main_layout->addWidget(gate_group, 0, 2, 5, 1);
    main_layout->addWidget(_start_btn, 0, 3);
    main_layout->addWidget(_show_weight_btn, 1, 3);
    main_layout->addWidget(_set_layer_btn, 2, 3);
    main_layout->addWidget(_see_more_btn, 3, 3);
    main_layout->addWidget(_clear_btn, 4, 3);
    main_layout->addWidget(rate_label, 5, 2);
    main_layout->addWidget(_rate_box, 5, 3);
    main_layout->addWidget(iter_label, 6, 2);
    main_layout->addWidget(_iter_cnt_box, 6, 3);
    main_layout->addWidget(error_view, 7, 0, 1, 4);
    QWidget *widget = new QWidget();
    widget->setLayout(main_layout);


    /* Set signal & slot for Linear_classifier. */
    _classifier = new Classifier(this);
    qRegisterMetaType<vector<double>>("vector<double>");
    qRegisterMetaType<vector<vector<vector<double> > >*>("vector<vector<vector<double> > >*");
    connect(_classifier,
            SIGNAL(changed_weight(vector<vector<vector<double> > >*, vector<vector<vector<double> > >*, double, QString*)),
            this,
            SLOT(iterated_once(vector<vector<vector<double> > >*, vector<vector<vector<double> > >*, double, QString*)));
    connect(_classifier,
            SIGNAL(done(vector<double>, bool)),
            this,
            SLOT(iterated_all(vector<double>, bool)));

    /* Set up UI of MainWindow. */
    resize(windowWidth, windowHeight);
    setCentralWidget(widget);
    setWindowTitle(title);
}


MainWindow::~MainWindow()
{
    delete _start_btn;
    delete _show_weight_btn;
    delete _set_layer_btn;
    delete _see_more_btn;
    delete _clear_btn;
    delete _error_chart;
    delete _and_btn;
    delete _or_btn;
    delete _xor_btn;
    delete _donut_btn;
    delete _rate_box;
    delete _iter_cnt_box;
    delete _classifier;
    _w4d.clear();
    _i4d.clear();
}


void MainWindow::clicked_start_btn()
{
    if (_classifier->doing())
    {
        _start_btn->setText(tr("Start"));
        _show_weight_btn->setEnabled(true);
        _set_layer_btn->setEnabled(true);
        _see_more_btn->setEnabled(true);
        _clear_btn->setEnabled(true);
    }
    else
    {
        _learning_graph->clear();
        _w4d.clear();
        _i4d.clear();
        vector<vector<double> > input;
        vector<double> output;
        if (_donut_btn->isChecked()) {
            input = _classifier->sampleInputDonut;
            output = _classifier->sampleOutputDonut;
        } else {
            input = _classifier->sampleInputGate;
            if (_and_btn->isChecked())
            {
                output = _classifier->sampleOutputAND;
            }
            else if (_or_btn->isChecked())
            {
                output = _classifier->sampleOutputOR;
            }
            else if (_xor_btn->isChecked())
            {
                output = _classifier->sampleOutputXOR;
            }
        }

        _start_btn->setText(tr("Stop"));
        _show_weight_btn->setEnabled(false);
        _set_layer_btn->setEnabled(false);
        _see_more_btn->setEnabled(false);
        _clear_btn->setEnabled(false);
        _learning_graph->set_play_enabled(false);

        _learning_graph->change_line_color(false);
        _classifier->ready(_node_count, _rate_box->value(), input, output, _iter_cnt_box->value());
        _classifier->start();
    }
    _classifier->change_state();
}


void MainWindow::clicked_weight_btn()
{
    QString *s = _classifier->get_weight_info_str();
    if (s == nullptr) {
        QLabel *label = new QLabel("Please click the start button.");
        label->setFixedSize(200, 80);
        label->show();
        return;
    }
    QWidget *w = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout();
    QPushButton *save_btn = new QPushButton(tr("Save as"));
    QScrollArea *scrollarea = new QScrollArea();
    QLabel *label = new QLabel(*s);
    save_btn->setFixedHeight(40);
    save_btn->setCursor(QCursor(Qt::PointingHandCursor));
    connect(save_btn, SIGNAL(clicked()), this, SLOT(clicked_save_btn()));
    scrollarea->setWidget(label);
    scrollarea->setMinimumSize(400, 400);
    layout->addWidget(save_btn);
    layout->addWidget(scrollarea);
    w->setWindowTitle("Weight Matrix");
    w->setLayout(layout);
    w->show();
}

void MainWindow::clicked_save_btn()
{
    QString *s = _classifier->get_weight_info_str();
    QString filename = QFileDialog::getSaveFileName(this, "Save as");
    QFile f(filename);
    f.open(QIODevice::WriteOnly);
    f.write(s->toStdString().c_str(), s->length());
    f.close();
}

void MainWindow::clicked_set_node_btn()
{
    vector<int> node_cnt(_node_count.begin()+1, _node_count.end()-1);
    _layersettingswindow = new LayerSettingsWindow(&node_cnt);
    connect(_layersettingswindow, SIGNAL(applied(vector<int>)), this, SLOT(applied_node_info(vector<int>)));
}


void MainWindow::clicked_see_more_btn()
{
    new ReplayWindow(&_w4d, &_i4d, &_node_count);
}


void MainWindow::clicked_clear_btn()
{
    _learning_graph->clear();
    _error_chart->removeAllSeries();
    _error_chart->clearFocus();
    _error_chart->removeAxis(_error_chart->axisX());
    _error_chart->removeAxis(_error_chart->axisY());
    _hypothesis_label->clear();
}


void MainWindow::applied_node_info(vector<int> node_cnt)
{
    _node_count = node_cnt;
    _layersettingswindow->close();
}


void MainWindow::iterated_once(vector<vector<vector<double> > > *weights,
                               vector<vector<vector<double> > > *inputs,
                               double bias,
                               QString* h_text)
{
    vector<vector<vector<double> > > copied_w;
    vector<vector<vector<double> > > copied_i;
    copy(weights->begin(), weights->end(), back_inserter(copied_w));
    copy(inputs->begin(), inputs->end(), back_inserter(copied_i));

    int layer_count = _node_count.size();

    // _w4d[i][j][k][l] : i = layer number, j = output node number, k = iter cnt, l = input node number
    // _i4d[i][j][k][l]

    for (int i=0; i<layer_count-2; i++) {
        int output_node_number = _node_count[i+1];
        int input_node_number = _node_count[i];
        _w4d.push_back(vector<vector<vector<double> > > (output_node_number));
        _i4d.push_back(vector<vector<vector<double> > > (output_node_number));
        for (int j=0; j<output_node_number; j++) {
            _w4d[i][j].push_back(copied_w[i][j]);
            vector<double> *nodes = new vector<double>(input_node_number);
            for (int k=0; k<input_node_number; k++) {
                nodes->push_back(copied_i[i][k].back());
            }
            _i4d[i][j].push_back(*nodes);
        }
    }

    int last_hidden_node_cnt = _node_count[layer_count-2];
    vector<double> w(last_hidden_node_cnt); // = new vector<double>(last_hidden_node_cnt);
    copy(copied_w[layer_count-2][0].begin(), copied_w[layer_count-2][0].end(), w.begin());
    vector<double> i(last_hidden_node_cnt); // = new vector<double>(last_hidden_node_cnt);
    i[0] = copied_i[layer_count-2][0].back();
    i[1] = copied_i[layer_count-2][1].back();

    double intercept = bias;
    for (int n=2; n<last_hidden_node_cnt; n++) {
        double x = copied_i[layer_count-2][n].back();
        i[n] = x;
        intercept = w[n] * x;
    }
    double slope = -(w[0]/w[1]);
    intercept /= -w[1];
    _learning_graph->add_data(&w, &i);
    _learning_graph->draw_line(slope, intercept);
    _hypothesis_label->setText(*h_text);
}


void MainWindow::iterated_all(vector<double> errors, bool not_broken)
{
    QSplineSeries *spline_series = new QSplineSeries();
    for (int i=0; i<errors.size(); i++) {
        spline_series->append(i, errors[i]);
    }
    _error_chart->addSeries(spline_series);
    _error_chart->createDefaultAxes();
    _error_chart->axisX()->setTitleText("Iteration Count");
    _learning_graph->change_line_color(true);
    _learning_graph->set_play_enabled(true);
    int layer_number = _node_count.size();
    _learning_graph->set(layer_number, _node_count[layer_number-2]);

    if (not_broken) {
        this->clicked_start_btn();
    }
}
