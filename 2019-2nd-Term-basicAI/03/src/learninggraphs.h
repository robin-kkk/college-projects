#ifndef LEARNINGGRAPHS_H
#define LEARNINGGRAPHS_H

#include <QtWidgets>
#include <QtCharts>
#include <QThread>
#include <vector>
#include <iostream>

using namespace std;

class ReplayThread : public QThread
{
    Q_OBJECT

public:
    ReplayThread() {}

    ~ReplayThread() {
        clear();
    }

    void clear() {
        _input_per_iter.clear();
        _weight_per_iter.clear();
    }

    void set(int layer_number, int input_node_cnt) {
        _layer_number = layer_number;
        _input_node_cnt = input_node_cnt;
    }

    void add_data(vector<double> *weights, vector<double> *inputs) {
        _weight_per_iter.push_back(*weights);
        _input_per_iter.push_back(*inputs);
    }

    void set_data(vector<vector<double> > *weights, vector<vector<double> > *inputs) {
        _weight_per_iter = *weights;
        _input_per_iter = *inputs;
    }

signals:
    void drawn(double slope, double intercept);
    void done(bool red);

public slots:
    void on_x0_idx_changed(int idx) {
        _input_node_number.first = idx;
    }
    void on_x1_idx_changed(int idx) {
        _input_node_number.second = idx;
    }

private:
    const double _bias = 1.;
    int _layer_number;
    int _input_node_cnt;
    pair<int, int> _input_node_number = {0, 1};
    vector<vector<double> > _input_per_iter;
    vector<vector<double> > _weight_per_iter;

    void run() {
        int iter_count = _weight_per_iter.size();
        int x0_idx = _input_node_number.first;
        int x1_idx = _input_node_number.second;
        for (int i=0; i<iter_count; i++) {
            double intercept = _bias;
            for (int j=0; j<_input_node_cnt; j++) {
                if (j == x0_idx || j == x1_idx) {
                    continue;
                }
                intercept = _weight_per_iter[i][j] * _input_per_iter[i][j];
            }
            double slope = -(_weight_per_iter[i][x0_idx] / _weight_per_iter[i][x1_idx]);
            intercept /= -_weight_per_iter[i][x1_idx];
            drawn(slope, intercept);
            QThread::msleep(10);
        }
        done(true);
    }
};

class LearningGraph : public QWidget
{
    Q_OBJECT

public:
    LearningGraph(QString title = "Learning Graph in Output Layer") {
        _replay = new ReplayThread();
        connect(_replay, SIGNAL(drawn(double, double)), this, SLOT(draw_line(double, double)));
        connect(_replay, SIGNAL(done(bool)), this, SLOT(change_line_color(bool)));

        /* Set UI on Learning Graph. */
        _learning_chart = new QChart();
        _learning_chart->legend()->hide();
        _learning_chart->setTitle(title);
        _learning_chart->setMinimumSize(280, 260);
        QChartView *learning_view = new QChartView(_learning_chart);
        learning_view->setRenderHint(QPainter::Antialiasing);

        QValueAxis *axisX = new QValueAxis();
        QValueAxis *axisY = new QValueAxis();
        axisX->setRange(-0.5, 2.0);
        axisY->setRange(-0.5, 2.0);
        axisX->setTickCount(6);
        axisY->setTickCount(6);
        axisX->setLabelFormat("%.1f");
        axisY->setLabelFormat("%.1f");
        _learning_chart->addAxis(axisX, Qt::AlignBottom);
        _learning_chart->addAxis(axisY, Qt::AlignLeft);

        QLineSeries *line_x = new QLineSeries();
        QLineSeries *line_y = new QLineSeries();
        *line_x << QPointF(-0.5, 0.0) << QPointF(2.0, 0.0);
        *line_y << QPointF(0.0, -5.0) << QPointF(0.0, 2.0);
        _learning_chart->addSeries(line_x);
        _learning_chart->addSeries(line_y);
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
        _learning_chart->addSeries(points);
        points->attachAxis(axisX);
        points->attachAxis(axisY);
        points->setMarkerSize(10);
        points->setColor(QColor(QRgb(0xfabc210)));

        _series = new QLineSeries();
        _learning_chart->addSeries(_series);
        _series->attachAxis(axisX);
        _series->attachAxis(axisY);


        /* Set UI on options of Learning Graph. */
        QLabel *x0_label = new QLabel(tr("x0 : "));
        QLabel *x1_label = new QLabel(tr("x1 : "));
        x0_label->setFixedSize(30, 15);
        x0_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
        x1_label->setFixedSize(30, 15);
        x1_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

        _x0_combobox = new QComboBox();
        _x1_combobox = new QComboBox();
        _x0_combobox->setEditable(false);
        _x1_combobox->setEditable(false);
        _x1_combobox->setFixedSize(90, 20);
        _x0_combobox->setFixedSize(90, 20);
        _x0_combobox->addItem(tr("Node #"));
        _x1_combobox->addItem(tr("Node #"));
        _x1_combobox->setCursor(QCursor(Qt::PointingHandCursor));
        _x0_combobox->setCursor(QCursor(Qt::PointingHandCursor));
        _x0_combobox->setToolTip(QString("Input-node number on x0 axis"));
        _x1_combobox->setToolTip(QString("Input-node number on x1 axis"));
        connect(_x0_combobox, SIGNAL(currentIndexChanged(int)), _replay, SLOT(on_x0_idx_changed(int)));
        connect(_x1_combobox, SIGNAL(currentIndexChanged(int)), _replay, SLOT(on_x1_idx_changed(int)));

        _play_btn = new QPushButton(playStr);
        _play_btn->setFixedSize(40, 30);
        _play_btn->setCursor(QCursor(Qt::PointingHandCursor));
        connect(_play_btn, SIGNAL(clicked()), this, SLOT(on_clicked__play_btn()));

        QLabel *slope_label = new QLabel("Slope : ");
        QLabel *intercept_label = new QLabel("Intercept : ");
        slope_label->setFixedSize(50, 15);
        intercept_label->setFixedSize(60, 15);
        _slope = new QLabel();
        _intercept = new QLabel();
        _slope->setFixedSize(80, 15);
        _slope->setFixedSize(80, 15);

        QVBoxLayout *vlayout = new QVBoxLayout();
        QHBoxLayout *h1layout = new QHBoxLayout();
        QHBoxLayout *h2layout = new QHBoxLayout();
        h1layout->addWidget(x0_label);
        h1layout->addWidget(_x0_combobox);
        h1layout->addWidget(x1_label);
        h1layout->addWidget(_x1_combobox);
        h1layout->addWidget(_play_btn);
        h1layout->setContentsMargins(0,0,0,0);
        h2layout->addWidget(slope_label);
        h2layout->addWidget(_slope);
        h2layout->addWidget(intercept_label);
        h2layout->addWidget(_intercept);
        h2layout->setContentsMargins(0,0,0,0);
        vlayout->addWidget(learning_view, 1, Qt::AlignCenter);
        vlayout->addLayout(h1layout);
        vlayout->addLayout(h2layout);
        vlayout->setContentsMargins(0,0,0,0);
        setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        setLayout(vlayout);
        setFixedSize(300, 310);
    }

    ~LearningGraph() {
        delete _series;
        delete _learning_chart;
        delete _x0_combobox;
        delete _x1_combobox;
        delete _play_btn;
        delete _replay;
    }

    void clear() {
        _series->clear();
        _x0_combobox->clear();
        _x1_combobox->clear();
        _slope->clear();
        _intercept->clear();
        _replay->clear();
    }

    void set_play_enabled(bool enabled) {
        _play_btn->setEnabled(enabled);
    }

    void add_data(vector<double> *weights, vector<double> *inputs) {
        _replay->add_data(weights, inputs);
    }

    void set_data(vector<vector<double> > *weights, vector<vector<double> > *inputs) {
        _replay->set_data(weights, inputs);
    }

    void set(int layer_number, int node_cnt) {
        _replay->set(layer_number, node_cnt);
        _x0_combobox->removeItem(0);
        _x1_combobox->removeItem(0);
        for (int i=0; i<node_cnt; i++) {
            _x0_combobox->addItem(QString::number(i));
            _x1_combobox->addItem(QString::number(i));
        }
        _x1_combobox->setCurrentIndex(1);
    }


signals:

public slots:
    void draw_line(double slope, double intercept) {
        pair<double, double> p1 = { -0.5, -0.5*slope + intercept };
        pair<double, double> p2 = { 2.0, 2.0*slope + intercept };

        if (_series->points().isEmpty())
        {
            *_series << QPointF(p1.first, p1.second) << QPointF(p2.first, p2.second);
        }
        else
        {
            _series->replace(0, p1.first, p1.second);
            _series->replace(1, p2.first, p2.second);
        }

        _slope->setText(QString::number(slope));
        _intercept->setText(QString::number(intercept));
    }

    void change_line_color(bool red=false) {
        if (red) {
            _series->setColor(QColor(255, 0, 0));
        } else {
            _series->setColor(QColor(0, 0, 255));
        }
    }

    void on_clicked__play_btn() {
        if (_drawing) {
            if (_replay->isRunning()) {
                _replay->terminate();
                _replay->wait();
            }
            change_line_color(true);
            _x0_combobox->setEnabled(true);
            _x1_combobox->setEnabled(true);
            _play_btn->setText(playStr);
        } else {
            _x0_combobox->setEnabled(false);
            _x1_combobox->setEnabled(false);
            _play_btn->setText(stopStr);
            change_line_color(false);
            _replay->start();
        }
        _drawing = !_drawing;
    }

private:
    const QString playStr = "▶︎";
    const QString stopStr = "◼︎";
    QLineSeries *_series;
    QChart *_learning_chart;
    QComboBox *_x0_combobox;
    QComboBox *_x1_combobox;
    QPushButton *_play_btn;
    QLabel *_slope;
    QLabel *_intercept;
    ReplayThread *_replay;

    bool _drawing = false;
    vector<vector<vector<vector<double> > > > *w4d;
    vector<vector<vector<vector<double> > > > *i4d;
};


class ReplayWindow : public QScrollArea
{
    Q_OBJECT

public:
    ReplayWindow(vector<vector<vector<vector<double> > > > *w4d,
                    vector<vector<vector<vector<double> > > > *i4d,
                    vector<int> *node_cnt) {
        if (w4d->empty() || i4d->empty()) {
            QLabel *l = new QLabel("Please click the start button.");
            setWidget(l);
            show();
            return;
        }
        _w4d = w4d;
        _i4d = i4d;
        _node_cnt = *node_cnt;
        _hidden_layer_cnt = _node_cnt.size() - 1;

        QVBoxLayout *vlayout = new QVBoxLayout();
        QHBoxLayout *hlayout = new QHBoxLayout();
        _play_btn = new QPushButton(playStr);
        _play_btn->setFixedSize(120, 40);
        _play_btn->setStyleSheet(fontStyle20);
        _play_btn->setCursor(QCursor(Qt::PointingHandCursor));
        connect(_play_btn, SIGNAL(clicked()), this, SLOT(on_clicked_play_btn()));
        hlayout->addWidget(_play_btn, 1, Qt::AlignVCenter);
        vlayout->addLayout(hlayout);

        for (int i=1; i<_hidden_layer_cnt; i++) {
            QString s = "Hidden\nLayer ";
            s.append(QString::number(i));
            QLabel *label = new QLabel(s);
            label->setFixedWidth(50);
            label->setAlignment(Qt::AlignCenter);
            label->setStyleSheet(fontStyle20);
            QHBoxLayout *layout = new QHBoxLayout();
            layout->addWidget(label);
            for (int j=0; j<_node_cnt[i]; j++) {
                QString title = "Learning Graph in Node ";
                title.append(QString::number(j+1));
                LearningGraph *graph = new LearningGraph(title);
                graph->set_data(&((*_w4d)[i-1][j]), &((*_i4d)[i-1][j]));
                graph->set(i, _node_cnt[i-1]);
                _graph_list.push_back(graph);
                layout->addWidget(graph);
            }
            vlayout->addLayout(layout);
        }

        QWidget *w = new QWidget();
        w->setLayout(vlayout);
        setWidget(w);
        setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        show();
    }

    ~ReplayWindow() {
        delete _play_btn;
    }

    void add_data(vector<vector<vector<double> > > w3d, vector<vector<vector<double> > > i3d) {
        _w4d->push_back(w3d);
        _i4d->push_back(i3d);
    }

signals:

public slots:
    void on_clicked_play_btn() {
        if (_drawing) {
            _play_btn->setText(playStr);
            _play_btn->setEnabled(false);
            for (int i=0; i<_graph_list.size(); i++) {
                _graph_list[i]->on_clicked__play_btn();
                _graph_list[i]->set_play_enabled(true);
            }
            _play_btn->setEnabled(true);
        } else {
            _play_btn->setText(stopStr);
            _play_btn->setEnabled(false);
            for (int i=0; i<_graph_list.size(); i++) {
                _graph_list[i]->set_play_enabled(false);
                _graph_list[i]->on_clicked__play_btn();
            }
            _play_btn->setEnabled(true);
        }
        _drawing = !_drawing;
    }


private:
    const QString fontStyle20 = "font-style:20pt;";
    const QString playStr = "PLAY ALL";
    const QString stopStr = "STOP";
    QPushButton *_play_btn;

    int _hidden_layer_cnt;
    bool _drawing = false;
    vector<vector<vector<vector<double> > > > *_w4d; // weight matrix arr
    vector<vector<vector<vector<double> > > > *_i4d; // input matrix arr
    vector<int> _node_cnt;
    vector<LearningGraph*> _graph_list;
};


#endif // LEARNINGGRAPHS_H
