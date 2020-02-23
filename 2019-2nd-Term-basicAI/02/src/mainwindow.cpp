#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /* Set UI on Learning Graph. */
    learning_chart = new QChart();
    learning_chart->legend()->hide();
    learning_chart->setTitle("Learning Graph");
    QChartView *learning_view = new QChartView(learning_chart);
    learning_view->setRenderHint(QPainter::Antialiasing);

    QValueAxis *axisX = new QValueAxis();
    QValueAxis *axisY = new QValueAxis();
    axisX->setRange(-0.5, 2.0);
    axisY->setRange(-0.5, 2.0);
    axisX->setTickCount(6);
    axisY->setTickCount(6);
    axisX->setLabelFormat("%.1f");
    axisY->setLabelFormat("%.1f");
    learning_chart->addAxis(axisX, Qt::AlignBottom);
    learning_chart->addAxis(axisY, Qt::AlignLeft);

    QLineSeries *line_x = new QLineSeries();
    QLineSeries *line_y = new QLineSeries();
    *line_x << QPointF(-0.5, 0.0) << QPointF(2.0, 0.0);
    *line_y << QPointF(0.0, -5.0) << QPointF(0.0, 2.0);
    learning_chart->addSeries(line_x);
    learning_chart->addSeries(line_y);
    line_x->attachAxis(axisX);
    line_x->attachAxis(axisY);
    line_y->attachAxis(axisX);
    line_y->attachAxis(axisY);
    QPen pen(QRgb(0x00000000));
    pen.setWidth(2);
    line_x->setPen(pen);
    line_y->setPen(pen);

    QScatterSeries *points = new QScatterSeries();
    *points << QPointF(0.0, 0.0) << QPointF(0.0, 1.0) << QPointF(1.0, 0.0) << QPointF(1.0, 1.0);
    learning_chart->addSeries(points);
    points->attachAxis(axisX);
    points->attachAxis(axisY);
    points->setMarkerSize(10);
    points->setColor(QColor(QRgb(0xfabc210)));

    series = new QLineSeries();
    learning_chart->addSeries(series);
    series->attachAxis(axisX);
    series->attachAxis(axisY);


    /* Set UI on Error Graph. */
    error_chart = new QChart();
    error_chart->legend()->hide();
    error_chart->setTitle("Error Graph");
    error_chart->setAnimationOptions(QChart::SeriesAnimations);

    bar_set = new QBarSet("Error");
    QStackedBarSeries *bar_series = new QStackedBarSeries();
    bar_series->append(bar_set);
    error_chart->addSeries(bar_series);

    QChartView *error_view = new QChartView(error_chart);
    error_view->setRenderHint(QPainter::Antialiasing);
    error_view->setRubberBand(QChartView::HorizontalRubberBand);


    /* Set UI on Weight Chart. */
    weight_chart = new QTableWidget();
    weight_chart->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);
    weight_chart->horizontalScrollBar()->setVisible(true);
    weight_chart->setEditTriggers(QAbstractItemView::NoEditTriggers);

    start_btn = new QPushButton(tr("Start"));
    start_btn->setStyleSheet(FONT_STYLE_14PT);
    start_btn->setFixedSize(BUTTON_WIDTH, BUTTON_HEIGHT);
    start_btn->setCursor(QCursor(Qt::PointingHandCursor));
    connect(start_btn, SIGNAL(clicked()), this, SLOT(start()));

    random_input_btn = new QPushButton(tr("Random Input"));
    random_input_btn->setStyleSheet(FONT_STYLE_14PT);
    random_input_btn->setFixedSize(BUTTON_WIDTH, BUTTON_HEIGHT);
    random_input_btn->setCursor(QCursor(Qt::PointingHandCursor));
    connect(random_input_btn, SIGNAL(clicked()), this, SLOT(random_input()));

    init_input_btn = new QPushButton(tr("Initial Input"));
    init_input_btn->setStyleSheet(FONT_STYLE_14PT);
    init_input_btn->setFixedSize(BUTTON_WIDTH, BUTTON_HEIGHT);
    init_input_btn->setCursor(QCursor(Qt::PointingHandCursor));
    connect(init_input_btn, SIGNAL(clicked()), this, SLOT(init_input()));


    /* Set UI on RadioButton about Gate (AND, OR, XOR). */
    and_btn = new QRadioButton(tr("AND"));
    and_btn->setChecked(true);
    or_btn = new QRadioButton(tr("OR"));
    xor_btn = new QRadioButton(tr("XOR"));
    QVBoxLayout *group_layout = new QVBoxLayout();
    group_layout->addWidget(and_btn);
    group_layout->addWidget(or_btn);
    group_layout->addWidget(xor_btn);
    QGroupBox *gate_group = new QGroupBox(tr("Gate"));
    gate_group->setAlignment(Qt::AlignCenter);
    gate_group->setStyleSheet(FONT_STYLE_14PT);
    gate_group->setMinimumWidth(GROUP_WIDTH);
    gate_group->setMaximumWidth(GROUP_WIDTH);
    gate_group->setLayout(group_layout);


    /* Set UI on SpinBox about Input Dimension. */
    dimension_box = new QSpinBox();
    dimension_box->setStyleSheet(FONT_STYLE_14PT);
    dimension_box->setRange(3, 1000);
    dimension_box->setValue(3);
    dimension_box->setCursor(QCursor(Qt::PointingHandCursor));
    QLabel *dim_label = new QLabel(tr("Dimension : "));
    dim_label->setAlignment(Qt::AlignRight);
    dim_label->setFixedWidth(GROUP_WIDTH);


    /* Set UI on SpinBox about Learning Rate. */
    rate_box = new QDoubleSpinBox();
    rate_box->setStyleSheet(FONT_STYLE_14PT);
    rate_box->setRange(0.0, 1.0);
    rate_box->setValue(0.1);
    rate_box->setSingleStep(0.01);
    rate_box->setCursor(QCursor(Qt::PointingHandCursor));
    QLabel *rate_label = new QLabel(tr("Learning Rate : "));
    rate_label->setAlignment(Qt::AlignRight);
    rate_label->setFixedWidth(GROUP_WIDTH);


    /* Set UI on Layout of MainWindow. */
    QGridLayout *main_layout = new QGridLayout();
    main_layout->addWidget(learning_view, 0, 0, 6, 1);
    main_layout->addWidget(gate_group, 0, 1, 3, 1);
    main_layout->addWidget(start_btn, 0, 2);
    main_layout->addWidget(random_input_btn, 1, 2);
    main_layout->addWidget(init_input_btn, 2, 2);
    main_layout->addWidget(dim_label, 4, 1);
    main_layout->addWidget(dimension_box, 4, 2);
    main_layout->addWidget(rate_label, 5, 1);
    main_layout->addWidget(rate_box, 5, 2);
    main_layout->addWidget(error_view, 6, 0, 1, 3);
    main_layout->addWidget(weight_chart, 7, 0, 1, 3);
    QWidget *widget = new QWidget();
    widget->setLayout(main_layout);


    /* Set signal & slot for LinearClassifier. */
    classifier = new LinearClassifier(this);
    qRegisterMetaType<vector<double>>("vector<double>");
    qRegisterMetaType<vector<vector<double> >>("vector<vector<double> >");
    connect(classifier,
            SIGNAL(changed_weight(double, double, double, double, double)),
            this,
            SLOT(draw_line(double, double, double, double, double)));
    connect(classifier,
            SIGNAL(done(vector<double>, vector<vector<double> >)),
            this,
            SLOT(draw_error(vector<double>, vector<vector<double> >)));

    /* Set up UI of MainWindow. */
    resize(WINDOW_WIDTH, WINDOW_HEIGHT);
    setCentralWidget(widget);
    setWindowTitle(TITLE);

    /* Set Initial Data. */
    output.resize(4);
    input.push_back(vector<double> { 0, 0, 1, 1 });
    input.push_back(vector<double> { 0, 1, 0, 1 });
    input.push_back(vector<double> { -1, -1, -1, -1 });
    srand(static_cast<unsigned int>(time(nullptr)));
}


MainWindow::~MainWindow()
{
    delete start_btn;
    delete random_input_btn;
    delete init_input_btn;
    delete series;
    delete learning_chart;
    delete bar_set;
    delete error_chart;
    delete weight_chart;
    delete and_btn;
    delete or_btn;
    delete xor_btn;
    delete dimension_box;
    delete rate_box;
    delete classifier;
}


void MainWindow::start()
{
    if (classifier->is_doing())
    {
        start_btn->setText(tr("Start"));
        random_input_btn->setEnabled(true);
        init_input_btn->setEnabled(true);
    }
    else
    {
        if (and_btn->isChecked())
        {
            output[0] = output[1] = output[2] = 0;
            output[3] = 1;
        }
        else if (or_btn->isChecked())
        {
            output[0] = 0;
            output[1] = output[2] = output[3] = 1;
        }
        else
        {
            output[0] = output[3] = 0;
            output[1] = output[2] = 1;
        }
        start_btn->setText(tr("Stop"));
        random_input_btn->setEnabled(false);
        init_input_btn->setEnabled(false);

        if (bar_set->count()) {
            clear_ui();
        }

        int dim = dimension_box->value();
        initial_weight.resize(dim);
        for (int i=0; i<dim; i++)
        {
            initial_weight[i] = get_random_value(-1 ,1);
        }

        classifier->ready(input, output, initial_weight, rate_box->value());
        classifier->start();
    }
    classifier->change_state();
}


double MainWindow::get_random_value(int min, int max)
{
    static const double fraction = 1.0 / (RAND_MAX + 1.0);
    return min + static_cast<double>((max - min + 1) * (rand() * fraction));
}


QTableWidget* MainWindow::get_table_widget(int row, int column)
{
    QTableWidget *tb = new QTableWidget();
    tb->setWindowTitle("Input 'X' Array");
    tb->setRowCount(row);
    tb->setColumnCount(column);
    tb->setMinimumWidth(400);
    tb->setFixedHeight(150);
    for (int i=0; i<column; i++)
    {
        QString h = tr("x");
        h.append(QString::number(i));
        tb->setHorizontalHeaderItem(i, new QTableWidgetItem(h));
    }
    tb->setEditTriggers(QAbstractItemView::NoEditTriggers);

    for (int i=0; i<2; i++)
    {
        for (int j=0; j<4; j++)
        {
            tb->setItem(j, i, new QTableWidgetItem(QString::number(input[i][j])));
        }
    }

    return tb;
}


void MainWindow::clear_ui()
{
    series->setColor(QColor(0, 0, 255));
    bar_set->remove(0, bar_set->count());
    weight_chart->clear();
    weight_chart->setRowCount(0);
    weight_chart->setColumnCount(0);
}

void MainWindow::random_input()
{
    int dim = dimension_box->value();
    input.resize(dim);

    QTableWidget *tb = get_table_widget(4, dim);

    for (int i=2; i<dim; i++)
    {
        vector<double> input_x(4);
        for (int j=0; j<4; j++)
        {
            input_x[j] = get_random_value(-1, 0);
            tb->setItem(j, i, new QTableWidgetItem(QString::number(input_x[j])));
        }
        input[i] = input_x;
    }

    tb->show();
}


void MainWindow::init_input()
{
    input.resize(0);
    input.push_back(vector<double> { 0, 0, 1, 1 });
    input.push_back(vector<double> { 0, 1, 0, 1 });

    int dim = dimension_box->value();
    input.resize(dim);

    QTableWidget *tb = get_table_widget(4, dim);

    for (int i=2; i<dim; i++)
    {
        vector<double> input_x{ -1, -1, -1, -1 };
        tb->setItem(0, i, new QTableWidgetItem(QString::number(input_x[0])));
        tb->setItem(1, i, new QTableWidgetItem(QString::number(input_x[1])));
        tb->setItem(2, i, new QTableWidgetItem(QString::number(input_x[2])));
        tb->setItem(3, i, new QTableWidgetItem(QString::number(input_x[3])));
        input[i] = input_x;
    }

    tb->show();
}


void MainWindow::draw_line(double net, double x0, double w0, double x1, double w1)
{
    double tmp = net - w0*x0 - w1*x1;
    double x0_1 = -0.5;
    double x1_1 = - (w0 / w1) * x0_1 - (tmp / w1);
    double x0_2 = 2.0;
    double x1_2 = - (w0 / w1) * x0_2 - (tmp / w1);

    if (series->points().isEmpty())
    {
        *series << QPointF(x0_1, x1_1) << QPointF(x0_2, x1_2);
    }
    else
    {
        series->replace(0, x0_1, x1_1);
        series->replace(1, x0_2, x1_2);
    }
}


void MainWindow::draw_error(vector<double> errors, vector<vector<double> > weights)
{
    QStringList categories;

    int try_cnt = errors.size();
    int dim = input.size();

    weight_chart->setRowCount(dim);
    weight_chart->setColumnCount(try_cnt);

    for (int i=0; i<dim; i++)
    {
        QString s = "w";
        s.append(QString::number(i));
        weight_chart->setVerticalHeaderItem(i, new QTableWidgetItem(s));
    }

    for (int c=0; c<try_cnt; c++)
    {
        categories.append(QString::number(c+1));
        vector<double> w_entry = weights[c];

        weight_chart->setHorizontalHeaderItem(c, new QTableWidgetItem(QString::number(c+1)));
        weight_chart->horizontalHeaderItem(c)->setTextAlignment(Qt::AlignCenter);
        for (int r=0; r<dim; r++)
        {
            weight_chart->setItem(r, c, new QTableWidgetItem(QString::number(w_entry[r])));
        }
    }

    QList<qreal> error_list = QList<qreal>::fromVector(QVector<double>::fromStdVector(errors));
    bar_set->append(error_list);
    error_chart->createDefaultAxes();
    error_chart->axisX()->setTitleText("Try Count");
    error_chart->axisY()->setRange(-1, 1);

    series->setColor(QColor(255, 0, 0));
    this->start();
}
